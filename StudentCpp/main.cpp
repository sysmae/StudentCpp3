// main.cpp
#include "utils.h"
#include "Student.h"
#include "Professor.h"
#include "Administrator.h"
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <regex>

#define CUR_YEAR 2024
#define CUR_TERM 2

using namespace std;



int main() {
    string filename = "users.csv";
    string subjectsFile = "subjects.csv";
    string studentRecordsFile = "student_records.csv";

    // 사용자, 과목, 학생 기록 로드
    vector<unique_ptr<User>> users = loadUsers(filename);
    vector<Subject> subjects = loadSubjects(subjectsFile);
    vector<StudentRecord> studentRecords = loadStudentRecords(studentRecordsFile);

    //cout << "로드된 과목 수: " << subjects.size() << endl; // 디버깅 출력
    //for (const auto& subject : subjects) {
    //    cout << "과목 ID: " << subject.getID() << ", 교수 ID: " << subject.getProfessorID() << endl; // 각 과목 정보 출력
    //}

    //if (users.empty()) {
    //    cout << "사용자가 로드되지 않았습니다. 프로그램을 종료합니다.\n";
    //    return 1;
    //}
    //if (subjects.empty()) {
    //    cout << "과목이 로드되지 않았습니다. 프로그램을 종료합니다.\n";
    //    return 1;
    //}
    //if (studentRecords.empty()) {
    //    cout << "학생 기록이 로드되지 않았습니다. 프로그램을 종료합니다.\n";
    //    return 1;
    //}

    // 학생 ID를 Student*로 매핑
    map<int, Student*> studentMap;
    for (auto& user : users) {
        if (user->getUserType() == "Student") {
            Student* studentPtr = dynamic_cast<Student*>(user.get());
            if (studentPtr) {
                studentMap[studentPtr->getStudentID()] = studentPtr;
                studentPtr->loadTaughtSubjects(studentRecords, subjects);
                //studentPtr->printSubjectsCount(); // Check how many subjects are loaded for each student

            }
        }
    }


    // 교수 ID를 Professor*로 매핑
    map<string, Professor*> professorMap;
    for (auto& user : users) {
        if (user->getUserType() == "Professor") {
            Professor* professorPtr = dynamic_cast<Professor*>(user.get());
            if (professorPtr) {
                professorMap[professorPtr->getID()] = professorPtr;
                // 교수의 수업 과목 로드
                professorPtr->loadTeachingSubjects(subjects);
            }
        }
    }

    // 학생들을 과목에 등록
    for (const auto& record : studentRecords) {
        int subjectID = record.getSubjectID();
        int studentID = record.getStudentID();
        double score = record.getScore();
        double grade = record.getGrade();
        string letterGrade = record.getLetterGrade();

        Subject* subjectPtr = findSubject(subjects, subjectID);
        if (subjectPtr && studentMap.find(studentID) != studentMap.end()) {
            Student* studentPtr = studentMap[studentID];


            // 과목에 등록되어 있지 않으면 등록
            auto& studentSubjects = studentPtr->getSubjects();
            if (find_if(studentSubjects.begin(), studentSubjects.end(),
                [subjectID](const Subject& s) { return s.getID() == subjectID; }) == studentSubjects.end()) {
                studentPtr->enrollSubject(*subjectPtr);
            }

            // score 설정
            studentPtr->setScores(subjectID, score);

            // 점수 및 문자 성적 설정
            studentPtr->setGrades(subjectID, grade);
            if (!letterGrade.empty()) {
                if (grade == 4.5) {
                    studentPtr->updateLetterGrade(subjectID, "A+");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 4.0) {
                    studentPtr->updateLetterGrade(subjectID, "A");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 3.5) {
                    studentPtr->updateLetterGrade(subjectID, "B+");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 3.0) {
                    studentPtr->updateLetterGrade(subjectID, "B");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 2.5) {
                    studentPtr->updateLetterGrade(subjectID, "C+");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 2.0) {
                    studentPtr->updateLetterGrade(subjectID, "C");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 1.5) {
                    studentPtr->updateLetterGrade(subjectID, "D+");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else if (grade >= 1.0) {
                    studentPtr->updateLetterGrade(subjectID, "D");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }
                else {
                    studentPtr->updateLetterGrade(subjectID, "F");
                    studentPtr->updateLetterGrade(subjectID, letterGrade);
                }

            }
        }
    }


    // 로그인 및 메뉴 루프
    while (true) {
        User* loggedInUser = nullptr;
        while (loggedInUser == nullptr) {
            loggedInUser = login(users);
            if (loggedInUser == nullptr) {
                cout << "유효하지 않은 ID 또는 비밀번호입니다. 다시 시도해주세요.\n";
            }
        }
        cout << "\n로그인 성공! 환영합니다, " << loggedInUser->getName() << "님.\n";
        string userType = loggedInUser->getUserType();
        string ID = loggedInUser->getID();
        cout << "사용자 유형: " << userType << ", ID: " << ID << endl;


        bool logout = false;
        while (!logout) {
            displayMenu(userType);
            int choice;
            cout << "\n선택을 입력하세요: ";
            cin >> choice;
            cout << endl;

            if (choice == 0) {
                logout = true;
                cout << "로그아웃 중...\n";
                continue;
            }
            // 사용자 유형과 선택에 따른 메뉴 동작 구현
            if (userType == "Student") {
                Student* student = dynamic_cast<Student*>(loggedInUser);
                Subject* subject = nullptr;

                switch (choice) {
                case 1:
                    //studen<t->printSubjectsCount();
					student->viewCurSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "과목 ID를 입력하세요:";
					cin >> subjectId;

					if (subjectId == 0) {
						break;
					}

					subject = findSubject(subjects, subjectId);
					if (subject && subject->getYear() == CUR_YEAR && subject->getTerm() == CUR_TERM) {


						int subChoice;
						cout << "==========================================================\n";
						cout << "1. 성적 확인하기\n";
						cout << "2. 성적 분포 확인하기\n";
						cout << "0. 이전 메뉴로 돌아가기\n";
						cout << "==========================================================\n";
						cout << "선택을 입력하세요: ";
						cin >> subChoice;
						cout << endl;

						switch (subChoice) {
						case 1:
							cout << "==========================================================\n";
							cout << "성적 확인하기" << endl;
							cout << "----------------------------------------------------------\n";
							student->viewGradeBySubjectID(subjectId);
							break;
                        
						case 2:
							cout << "==========================================================\n";
							cout << "성적 분포 확인하기" << endl;
							cout << "----------------------------------------------------------\n";
							displayHistogramForSubject(studentRecords, subjectId);
							break;
						case 0:
							break;

						default:
							cout << "잘못된 선택입니다.\n";
							break;
						}
					}
					else {
						cout << "해당 ID의 과목을 찾을 수 없습니다.\n";
					}
					break;

                case 2:
                    student->viewAllPreviousSubjects(CUR_YEAR, CUR_TERM);
                    cout << "과목 ID를 입력하세요:";
                    cin >> subjectId;

                    if (subjectId == 0) {
                        break;
                    }

                    subject = findSubject(subjects, subjectId);
                    if (subject && subject->getYear() < CUR_YEAR || (subject->getYear() == CUR_YEAR && subject->getTerm() < CUR_TERM)) {


                        int subChoice;
                        cout << "==========================================================\n";
                        cout << "1. 성적 확인하기\n";
                        cout << "2. 성적 분포 확인하기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        cin >> subChoice;
                        cout << endl;

                        switch (subChoice) {
                        case 1:
                            cout << "==========================================================\n";
                            cout << "성적 확인하기" << endl;
                            cout << "----------------------------------------------------------\n";
                            student->viewGradeBySubjectID(subjectId);
                            break;

                        case 2:
                            cout << "==========================================================\n";
                            cout << "성적 분포 확인하기" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;
                        case 0:
                            break;

                        default:
                            cout << "잘못된 선택입니다.\n";
                            break;
                        }
                    }
                    else {
                        cout << "해당 ID의 과목을 찾을 수 없습니다.\n";
                    }
                break;
                case 3:
                    student->checkGraduationRequirements(subjects, studentRecords,CUR_YEAR,CUR_TERM);
                    break;
                case 4: {
                    string enteredPassword;

                    // Prompt the user for the current password
                    cout << "현재 비밀번호를 입력하세요: ";
                    enteredPassword = getMaskedInput();  // Use the utility function to get masked input
                    //cout << "입력된 비밀번호는: " << enteredPassword << endl;

                    // Check if the entered password matches the stored password
                    if (enteredPassword == student->getPassword()) {
                        // If password matches, show current information
                        cout << "==========================================================\n";
                        cout << "현재 개인정보" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "이름: " << student->getName() << endl;
                        cout << "전화번호: " << student->getPhoneNumber() << endl;
                        cout << "이메일: " << student->getEmail() << endl;
                        cout << "----------------------------------------------------------\n";

                        bool isValid = true; // Flag to track if all validations pass
                        cout << "내 정보 수정" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "1. 이름 변경" << endl;
                        cout << "2. 전화번호 변경" << endl;
                        cout << "3. 이메일 변경" << endl;
                        cout << "4. 비밀번호 변경" << endl;
                        cout << "0. 이전 메뉴로 돌아가기" << endl;
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        int subChoice;
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice
                        cout << endl;

                        switch (subChoice) {
                        case 1: {
                            cout << "새 이름을 입력하세요: ";
                            string newName;
                            cin >> ws;  // 선행 공백 문자 제거
                            getline(cin, newName);

                            if (!newName.empty()) {  // 빈 문자열이 아닌 경우에만 저장
                                student->setName(newName);
                                cout << "성공적으로 이름이 변경되었습니다.\n";
                            }
                            else {
                                cout << "유효하지 않은 이름입니다.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 2: {
                            cout << "새 전화번호를 입력하세요(000-0000-0000 형식으로 입력): ";
                            string newPhone;
                            cin >> newPhone;

                            bool isValidFormat = true;
                            if (newPhone.length() != 13 || newPhone[3] != '-' || newPhone[8] != '-') {
                                isValidFormat = false;
                            }
                            for (int i = 0; i < newPhone.length(); i++) {
                                if (i != 3 && i != 8 && !isdigit(newPhone[i])) {
                                    isValidFormat = false;
                                    break;
                                }
                            }

                            if (isValidFormat) {
                                student->setPhoneNumber(newPhone);
                                cout << "성공적으로 전화번호가 변경되었습니다.\n";
                            }
                            else {
                                cout << "올바르지 않은 전화번호 형식입니다. 000-0000-0000 형식으로 입력해주세요.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 3: {
                            cout << "새 이메일을 입력하세요: ";
                            string newEmail;
                            cin >> newEmail;

                            // Email validation logic
                            const regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");
                            if (regex_match(newEmail, emailRegex)) {
                                student->setEmail(newEmail);
                                cout << "성공적으로 이메일이 변경되었습니다.\n";
                            }
                            else {
                                cout << "올바르지 않은 이메일 형식입니다.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 4: {
                            cout << "새 비밀번호를 입력하세요: ";
                            string newPassword;
                            cin >> newPassword;
                            student->setPassword(newPassword);
                            cout << "성공적으로 비밀번호가 변경되었습니다.\n";
                            break;
                        }

                        case 0:
                            break;

                        default:
                            cout << "잘못된 선택입니다.\n";
                            isValid = false; // Mark as invalid
                            break;
                        }

                        if (isValid) {
                            // If all updates are valid, update the users CSV file
                            vector<User*> userPtrs;
                            for (const auto& user : users) {
                                userPtrs.push_back(user.get());  // unique_ptr을 raw pointer로 변환하여 추가
                            }
                            updateUsersCSV(userPtrs);
                            cout << "사용자 정보가 CSV 파일에 성공적으로 업데이트되었습니다.\n";
                        }
                        else {
                            cout << "정보가 유효하지 않아 변경되지 않았습니다.\n";
                        }
                    }
                    else {
                        cout << "비밀번호가 일치하지 않습니다.\n";
                    }
                    break;
                }
                default:
                    cout << "잘못된 선택입니다.\n";
                }
            }
            
            else if (userType == "Professor") {
                Professor* professor = dynamic_cast<Professor*>(loggedInUser);
                Subject* subject = nullptr;  // Declare subject once here

                switch (choice) {
                case 1: {
                    professor->viewSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "과목 ID를 입력하세요:";
                    cin >> subjectId;
                    cin.ignore();  // Clear the input buffer to prevent issues

                    if (subjectId == 0) {
                        break;
                    }

                    subject = findSubject(subjects, subjectId);  // Use the existing subject variable
                    if (subject && subject->getYear() == CUR_YEAR && subject->getTerm() == CUR_TERM) {
                        vector<Student*> enrolledStudents;
                        for (auto& studentPair : studentMap) {
                            Student* studentPtr = studentPair.second;
                            const auto& enrolledSubjects = studentPtr->getSubjects();
                            if (find_if(enrolledSubjects.begin(), enrolledSubjects.end(),
                                [subjectId](const Subject& s) { return s.getID() == subjectId; }) != enrolledSubjects.end()) {
                                enrolledStudents.push_back(studentPtr);
                            }
                        }

                        if (enrolledStudents.empty()) {
                            cout << "해당 과목에 등록된 학생이 없습니다.\n";
                            break;
                        }
                        professor->printStudentGrades(enrolledStudents, subject, subjectId);

                        int subChoice;
                        cout << "==========================================================\n";
                        cout << "1. 성적 입력 및 수정 \n";
                        cout << "2. 평점 부여 방식 선택\n";
                        cout << "3. 성적(분포) 확인하기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";
                        cout << "==========================================================\n";

                        cout << "선택을 입력하세요: ";
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice

                        switch (subChoice) {
                        case 1:
                            professor->printStudentGrades(enrolledStudents, subject, subjectId);
                            professor->inputStudentScores(enrolledStudents, *subject);
                            break;

                        case 2:
                            professor->assignGrades(enrolledStudents, *subject);
                            break;

                        case 3:
                            cout << "==========================================================\n";
                            cout << "성적(분포) 확인하기" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;

                        case 0:
                            break;

                        default:
                            cout << "잘못된 선택입니다.\n";
                            break;
                        }
                    }
                    else {
                        cout << "해당 ID의 과목을 찾을 수 없습니다.\n";
                    }
                    break;
                }

                case 2: {
                    professor->viewAllPreviousSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "과목 ID를 입력하세요:";
                    cin >> subjectId;
                    cin.ignore();  // Clear the input buffer after subjectId

                    if (subjectId == 0) {
                        break;
                    }

                    subject = findSubject(subjects, subjectId);  // Use the existing subject variable
                    if (subject && subject->getYear() < CUR_YEAR || (subject->getYear() == CUR_YEAR && subject->getTerm() < CUR_TERM)) {
                        vector<Student*> enrolledStudents;
                        for (auto& studentPair : studentMap) {
                            Student* studentPtr = studentPair.second;
                            const auto& enrolledSubjects = studentPtr->getSubjects();
                            if (find_if(enrolledSubjects.begin(), enrolledSubjects.end(),
                                [subjectId](const Subject& s) { return s.getID() == subjectId; }) != enrolledSubjects.end()) {
                                enrolledStudents.push_back(studentPtr);
                            }
                        }
                        professor->printStudentGrades(enrolledStudents, subject, subjectId);

                        int subChoice;
                        cout << "==========================================================\n";
                        cout << "1. 성적(분포) 확인하기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";

                        cout << "선택을 입력하세요: ";
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice

                        switch (subChoice) {
                        case 1:
                            cout << "==========================================================\n";
                            cout << "성적(분포) 확인하기" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;

                        case 0:
                            break;

                        default:
                            cout << "잘못된 선택입니다.\n";
                            break;
                        }
                        break;
                    }
                    else {
                        cout << "해당 ID의 과목을 찾을 수 없습니다.\n";
                        break;
                    }
                }

                case 3: {
                    string enteredPassword;

                    // Prompt the user for the current password
                    cout << "현재 비밀번호를 입력하세요: ";
                    enteredPassword = getMaskedInput();  // Use the utility function to get masked input
                    //cout << "입력된 비밀번호는: " << enteredPassword << endl;

                    // Check if the entered password matches the stored password
                    if (enteredPassword == professor->getPassword()) {
                        // If password matches, show current information
                        cout << "==========================================================\n";
                        cout << "현재 개인정보" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "이름: " << professor->getName() << endl;
                        cout << "전화번호: " << professor->getPhoneNumber() << endl;
                        cout << "이메일: " << professor->getEmail() << endl;
                        cout << "----------------------------------------------------------\n";

                        bool isValid = true; // Flag to track if all validations pass
                        cout << "내 정보 수정" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "1. 이름 변경" << endl;
                        cout << "2. 전화번호 변경" << endl;
                        cout << "3. 이메일 변경" << endl;
                        cout << "4. 비밀번호 변경" << endl;
                        cout << "0. 이전 메뉴로 돌아가기" << endl;
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        int subChoice;
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice
                        cout << endl;

                        switch (subChoice) {
                        case 1: {
                            cout << "새 이름을 입력하세요: ";
                            string newName;
                            cin >> ws;  // 선행 공백 문자 제거
                            getline(cin, newName);

                            if (!newName.empty()) {  // 빈 문자열이 아닌 경우에만 저장
                                professor->setName(newName);
                                cout << "성공적으로 이름이 변경되었습니다.\n";
                            }
                            else {
                                cout << "유효하지 않은 이름입니다.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 2: {
                            cout << "새 전화번호를 입력하세요(000-0000-0000 형식으로 입력): ";
                            string newPhone;
                            cin >> newPhone;

                            bool isValidFormat = true;
                            if (newPhone.length() != 13 || newPhone[3] != '-' || newPhone[8] != '-') {
                                isValidFormat = false;
                            }
                            for (int i = 0; i < newPhone.length(); i++) {
                                if (i != 3 && i != 8 && !isdigit(newPhone[i])) {
                                    isValidFormat = false;
                                    break;
                                }
                            }

                            if (isValidFormat) {
                                professor->setPhoneNumber(newPhone);
                                cout << "성공적으로 전화번호가 변경되었습니다.\n";
                            }
                            else {
                                cout << "올바르지 않은 전화번호 형식입니다. 000-0000-0000 형식으로 입력해주세요.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 3: {
                            cout << "새 이메일을 입력하세요: ";
                            string newEmail;
                            cin >> newEmail;

                            // Email validation logic
                            const regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");
                            if (regex_match(newEmail, emailRegex)) {
                                professor->setEmail(newEmail);
                                cout << "성공적으로 이메일이 변경되었습니다.\n";
                            }
                            else {
                                cout << "올바르지 않은 이메일 형식입니다.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 4: {
                            cout << "새 비밀번호를 입력하세요: ";
                            string newPassword;
                            cin >> newPassword;
                            professor->setPassword(newPassword);
                            cout << "성공적으로 비밀번호가 변경되었습니다.\n";
                            break;
                        }

                        case 0:
                            break;

                        default:
                            cout << "잘못된 선택입니다.\n";
                            isValid = false; // Mark as invalid
                            break;
                        }

                        if (isValid) {
                            // If all updates are valid, update the users CSV file
                            vector<User*> userPtrs;
                            for (const auto& user : users) {
                                userPtrs.push_back(user.get());  // unique_ptr을 raw pointer로 변환하여 추가
                            }
                            updateUsersCSV(userPtrs);
                            cout << "사용자 정보가 CSV 파일에 성공적으로 업데이트되었습니다.\n";
                        }
                        else {
                            cout << "정보가 유효하지 않아 변경되지 않았습니다.\n";
                        }
                    }
                    else {
                        cout << "비밀번호가 일치하지 않습니다.\n";
                    }
                    break;
                }

                }
            }


            else if (userType == "Administrator") {
                Administrator* admin = dynamic_cast<Administrator*>(loggedInUser);
                if (!admin) {
                    cout << "오류: 관리자 정보가 올바르지 않습니다.\n";
                    break;
                }
                switch (choice) {
                case 1:
                    // 과목 관리 메뉴
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "과목 관리 메뉴\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. 과목 추가하기\n";
                        cout << "2. 과목 삭제하기\n";
                        cout << "3. 과목 수정하기\n";
                        cout << "4. 과목 목록 보기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        int subChoice;
                        cin >> subChoice;
                        cout << endl;

                        switch (subChoice) {
                        case 1:
                            admin->addSubject(subjects, users);
                            saveSubjects(subjectsFile, subjects); // 변경 사항 저장
                            break;
                        case 2:
                            admin->deleteSubject(subjects);
                            saveSubjects(subjectsFile, subjects); // 변경 사항 저장
                            break;
                        case 3:
                            admin->modifySubject(subjects, users);
                            saveSubjects(subjectsFile, subjects); // 변경 사항 저장
                            break;
                        case 4:
                            admin->viewSubjects(subjects);
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "잘못된 선택입니다.\n";
                        }
                    }
                }
                break;
                case 2:
                    // 교수 관리 메뉴
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "교수 관리 메뉴\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. 교수 정보 조회\n";
                        cout << "2. 교수 추가하기\n";
                        cout << "3. 교수 삭제하기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        int profChoice;
                        cin >> profChoice;
                        cout << endl;

                        switch (profChoice) {
                        case 1:
                            admin->viewProfessorInfo(users);
                            break;
                        case 2:
                            admin->addProfessor(users);
                            saveUsers(filename, users); // 변경 사항 저장
                            break;
                        case 3:
                            admin->deleteProfessor(users);
                            saveUsers(filename, users); // 변경 사항 저장
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "잘못된 선택입니다.\n";
                        }
                    }
                }
                break;
                case 3:
                    // 학생 관리 메뉴
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "학생 관리 메뉴\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. 학생 정보 조회\n";
                        cout << "2. 학생 추가하기\n";
                        cout << "3. 학생 삭제하기\n";
                        cout << "0. 이전 메뉴로 돌아가기\n";
                        cout << "==========================================================\n";
                        cout << "선택을 입력하세요: ";
                        int studentChoice;
                        cin >> studentChoice;
                        cout << endl;

                        switch (studentChoice) {
                        case 1:
                            admin->viewStudentInfo(users);
                            break;
                        case 2:
                            admin->addStudent(users);
                            saveUsers(filename, users); // 변경 사항 저장
                            break;
                        case 3:
                            admin->deleteStudent(users);
                            saveUsers(filename, users); // 변경 사항 저장
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "잘못된 선택입니다.\n";
                        }
                    }
                }
                break;
                default:
                    cout << "잘못된 선택입니다.\n";
                }
            }
        }
    }
    return 0;
}