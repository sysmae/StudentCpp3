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

    // �����, ����, �л� ��� �ε�
    vector<unique_ptr<User>> users = loadUsers(filename);
    vector<Subject> subjects = loadSubjects(subjectsFile);
    vector<StudentRecord> studentRecords = loadStudentRecords(studentRecordsFile);

    //cout << "�ε�� ���� ��: " << subjects.size() << endl; // ����� ���
    //for (const auto& subject : subjects) {
    //    cout << "���� ID: " << subject.getID() << ", ���� ID: " << subject.getProfessorID() << endl; // �� ���� ���� ���
    //}

    //if (users.empty()) {
    //    cout << "����ڰ� �ε���� �ʾҽ��ϴ�. ���α׷��� �����մϴ�.\n";
    //    return 1;
    //}
    //if (subjects.empty()) {
    //    cout << "������ �ε���� �ʾҽ��ϴ�. ���α׷��� �����մϴ�.\n";
    //    return 1;
    //}
    //if (studentRecords.empty()) {
    //    cout << "�л� ����� �ε���� �ʾҽ��ϴ�. ���α׷��� �����մϴ�.\n";
    //    return 1;
    //}

    // �л� ID�� Student*�� ����
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


    // ���� ID�� Professor*�� ����
    map<string, Professor*> professorMap;
    for (auto& user : users) {
        if (user->getUserType() == "Professor") {
            Professor* professorPtr = dynamic_cast<Professor*>(user.get());
            if (professorPtr) {
                professorMap[professorPtr->getID()] = professorPtr;
                // ������ ���� ���� �ε�
                professorPtr->loadTeachingSubjects(subjects);
            }
        }
    }

    // �л����� ���� ���
    for (const auto& record : studentRecords) {
        int subjectID = record.getSubjectID();
        int studentID = record.getStudentID();
        double score = record.getScore();
        double grade = record.getGrade();
        string letterGrade = record.getLetterGrade();

        Subject* subjectPtr = findSubject(subjects, subjectID);
        if (subjectPtr && studentMap.find(studentID) != studentMap.end()) {
            Student* studentPtr = studentMap[studentID];


            // ���� ��ϵǾ� ���� ������ ���
            auto& studentSubjects = studentPtr->getSubjects();
            if (find_if(studentSubjects.begin(), studentSubjects.end(),
                [subjectID](const Subject& s) { return s.getID() == subjectID; }) == studentSubjects.end()) {
                studentPtr->enrollSubject(*subjectPtr);
            }

            // score ����
            studentPtr->setScores(subjectID, score);

            // ���� �� ���� ���� ����
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


    // �α��� �� �޴� ����
    while (true) {
        User* loggedInUser = nullptr;
        while (loggedInUser == nullptr) {
            loggedInUser = login(users);
            if (loggedInUser == nullptr) {
                cout << "��ȿ���� ���� ID �Ǵ� ��й�ȣ�Դϴ�. �ٽ� �õ����ּ���.\n";
            }
        }
        cout << "\n�α��� ����! ȯ���մϴ�, " << loggedInUser->getName() << "��.\n";
        string userType = loggedInUser->getUserType();
        string ID = loggedInUser->getID();
        cout << "����� ����: " << userType << ", ID: " << ID << endl;


        bool logout = false;
        while (!logout) {
            displayMenu(userType);
            int choice;
            cout << "\n������ �Է��ϼ���: ";
            cin >> choice;
            cout << endl;

            if (choice == 0) {
                logout = true;
                cout << "�α׾ƿ� ��...\n";
                continue;
            }
            // ����� ������ ���ÿ� ���� �޴� ���� ����
            if (userType == "Student") {
                Student* student = dynamic_cast<Student*>(loggedInUser);
                Subject* subject = nullptr;

                switch (choice) {
                case 1:
                    //studen<t->printSubjectsCount();
					student->viewCurSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "���� ID�� �Է��ϼ���:";
					cin >> subjectId;

					if (subjectId == 0) {
						break;
					}

					subject = findSubject(subjects, subjectId);
					if (subject && subject->getYear() == CUR_YEAR && subject->getTerm() == CUR_TERM) {


						int subChoice;
						cout << "==========================================================\n";
						cout << "1. ���� Ȯ���ϱ�\n";
						cout << "2. ���� ���� Ȯ���ϱ�\n";
						cout << "0. ���� �޴��� ���ư���\n";
						cout << "==========================================================\n";
						cout << "������ �Է��ϼ���: ";
						cin >> subChoice;
						cout << endl;

						switch (subChoice) {
						case 1:
							cout << "==========================================================\n";
							cout << "���� Ȯ���ϱ�" << endl;
							cout << "----------------------------------------------------------\n";
							student->viewGradeBySubjectID(subjectId);
							break;
                        
						case 2:
							cout << "==========================================================\n";
							cout << "���� ���� Ȯ���ϱ�" << endl;
							cout << "----------------------------------------------------------\n";
							displayHistogramForSubject(studentRecords, subjectId);
							break;
						case 0:
							break;

						default:
							cout << "�߸��� �����Դϴ�.\n";
							break;
						}
					}
					else {
						cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
					}
					break;

                case 2:
                    student->viewAllPreviousSubjects(CUR_YEAR, CUR_TERM);
                    cout << "���� ID�� �Է��ϼ���:";
                    cin >> subjectId;

                    if (subjectId == 0) {
                        break;
                    }

                    subject = findSubject(subjects, subjectId);
                    if (subject && subject->getYear() < CUR_YEAR || (subject->getYear() == CUR_YEAR && subject->getTerm() < CUR_TERM)) {


                        int subChoice;
                        cout << "==========================================================\n";
                        cout << "1. ���� Ȯ���ϱ�\n";
                        cout << "2. ���� ���� Ȯ���ϱ�\n";
                        cout << "0. ���� �޴��� ���ư���\n";
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        cin >> subChoice;
                        cout << endl;

                        switch (subChoice) {
                        case 1:
                            cout << "==========================================================\n";
                            cout << "���� Ȯ���ϱ�" << endl;
                            cout << "----------------------------------------------------------\n";
                            student->viewGradeBySubjectID(subjectId);
                            break;

                        case 2:
                            cout << "==========================================================\n";
                            cout << "���� ���� Ȯ���ϱ�" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;
                        case 0:
                            break;

                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                            break;
                        }
                    }
                    else {
                        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
                    }
                break;
                case 3:
                    student->checkGraduationRequirements(subjects, studentRecords,CUR_YEAR,CUR_TERM);
                    break;
                case 4: {
                    string enteredPassword;

                    // Prompt the user for the current password
                    cout << "���� ��й�ȣ�� �Է��ϼ���: ";
                    enteredPassword = getMaskedInput();  // Use the utility function to get masked input
                    //cout << "�Էµ� ��й�ȣ��: " << enteredPassword << endl;

                    // Check if the entered password matches the stored password
                    if (enteredPassword == student->getPassword()) {
                        // If password matches, show current information
                        cout << "==========================================================\n";
                        cout << "���� ��������" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "�̸�: " << student->getName() << endl;
                        cout << "��ȭ��ȣ: " << student->getPhoneNumber() << endl;
                        cout << "�̸���: " << student->getEmail() << endl;
                        cout << "----------------------------------------------------------\n";

                        bool isValid = true; // Flag to track if all validations pass
                        cout << "�� ���� ����" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "1. �̸� ����" << endl;
                        cout << "2. ��ȭ��ȣ ����" << endl;
                        cout << "3. �̸��� ����" << endl;
                        cout << "4. ��й�ȣ ����" << endl;
                        cout << "0. ���� �޴��� ���ư���" << endl;
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        int subChoice;
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice
                        cout << endl;

                        switch (subChoice) {
                        case 1: {
                            cout << "�� �̸��� �Է��ϼ���: ";
                            string newName;
                            cin >> ws;  // ���� ���� ���� ����
                            getline(cin, newName);

                            if (!newName.empty()) {  // �� ���ڿ��� �ƴ� ��쿡�� ����
                                student->setName(newName);
                                cout << "���������� �̸��� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "��ȿ���� ���� �̸��Դϴ�.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 2: {
                            cout << "�� ��ȭ��ȣ�� �Է��ϼ���(000-0000-0000 �������� �Է�): ";
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
                                cout << "���������� ��ȭ��ȣ�� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "�ùٸ��� ���� ��ȭ��ȣ �����Դϴ�. 000-0000-0000 �������� �Է����ּ���.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 3: {
                            cout << "�� �̸����� �Է��ϼ���: ";
                            string newEmail;
                            cin >> newEmail;

                            // Email validation logic
                            const regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");
                            if (regex_match(newEmail, emailRegex)) {
                                student->setEmail(newEmail);
                                cout << "���������� �̸����� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "�ùٸ��� ���� �̸��� �����Դϴ�.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 4: {
                            cout << "�� ��й�ȣ�� �Է��ϼ���: ";
                            string newPassword;
                            cin >> newPassword;
                            student->setPassword(newPassword);
                            cout << "���������� ��й�ȣ�� ����Ǿ����ϴ�.\n";
                            break;
                        }

                        case 0:
                            break;

                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                            isValid = false; // Mark as invalid
                            break;
                        }

                        if (isValid) {
                            // If all updates are valid, update the users CSV file
                            vector<User*> userPtrs;
                            for (const auto& user : users) {
                                userPtrs.push_back(user.get());  // unique_ptr�� raw pointer�� ��ȯ�Ͽ� �߰�
                            }
                            updateUsersCSV(userPtrs);
                            cout << "����� ������ CSV ���Ͽ� ���������� ������Ʈ�Ǿ����ϴ�.\n";
                        }
                        else {
                            cout << "������ ��ȿ���� �ʾ� ������� �ʾҽ��ϴ�.\n";
                        }
                    }
                    else {
                        cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n";
                    }
                    break;
                }
                default:
                    cout << "�߸��� �����Դϴ�.\n";
                }
            }
            
            else if (userType == "Professor") {
                Professor* professor = dynamic_cast<Professor*>(loggedInUser);
                Subject* subject = nullptr;  // Declare subject once here

                switch (choice) {
                case 1: {
                    professor->viewSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "���� ID�� �Է��ϼ���:";
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
                            cout << "�ش� ���� ��ϵ� �л��� �����ϴ�.\n";
                            break;
                        }
                        professor->printStudentGrades(enrolledStudents, subject, subjectId);

                        int subChoice;
                        cout << "==========================================================\n";
                        cout << "1. ���� �Է� �� ���� \n";
                        cout << "2. ���� �ο� ��� ����\n";
                        cout << "3. ����(����) Ȯ���ϱ�\n";
                        cout << "0. ���� �޴��� ���ư���\n";
                        cout << "==========================================================\n";

                        cout << "������ �Է��ϼ���: ";
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
                            cout << "����(����) Ȯ���ϱ�" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;

                        case 0:
                            break;

                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                            break;
                        }
                    }
                    else {
                        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
                    }
                    break;
                }

                case 2: {
                    professor->viewAllPreviousSubjects(CUR_YEAR, CUR_TERM);

                    int subjectId;
                    cout << "���� ID�� �Է��ϼ���:";
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
                        cout << "1. ����(����) Ȯ���ϱ�\n";
                        cout << "0. ���� �޴��� ���ư���\n";

                        cout << "������ �Է��ϼ���: ";
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice

                        switch (subChoice) {
                        case 1:
                            cout << "==========================================================\n";
                            cout << "����(����) Ȯ���ϱ�" << endl;
                            cout << "----------------------------------------------------------\n";
                            displayHistogramForSubject(studentRecords, subjectId);
                            break;

                        case 0:
                            break;

                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                            break;
                        }
                        break;
                    }
                    else {
                        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
                        break;
                    }
                }

                case 3: {
                    string enteredPassword;

                    // Prompt the user for the current password
                    cout << "���� ��й�ȣ�� �Է��ϼ���: ";
                    enteredPassword = getMaskedInput();  // Use the utility function to get masked input
                    //cout << "�Էµ� ��й�ȣ��: " << enteredPassword << endl;

                    // Check if the entered password matches the stored password
                    if (enteredPassword == professor->getPassword()) {
                        // If password matches, show current information
                        cout << "==========================================================\n";
                        cout << "���� ��������" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "�̸�: " << professor->getName() << endl;
                        cout << "��ȭ��ȣ: " << professor->getPhoneNumber() << endl;
                        cout << "�̸���: " << professor->getEmail() << endl;
                        cout << "----------------------------------------------------------\n";

                        bool isValid = true; // Flag to track if all validations pass
                        cout << "�� ���� ����" << endl;
                        cout << "----------------------------------------------------------\n";
                        cout << "1. �̸� ����" << endl;
                        cout << "2. ��ȭ��ȣ ����" << endl;
                        cout << "3. �̸��� ����" << endl;
                        cout << "4. ��й�ȣ ����" << endl;
                        cout << "0. ���� �޴��� ���ư���" << endl;
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        int subChoice;
                        cin >> subChoice;
                        cin.ignore();  // Clear the input buffer after subChoice
                        cout << endl;

                        switch (subChoice) {
                        case 1: {
                            cout << "�� �̸��� �Է��ϼ���: ";
                            string newName;
                            cin >> ws;  // ���� ���� ���� ����
                            getline(cin, newName);

                            if (!newName.empty()) {  // �� ���ڿ��� �ƴ� ��쿡�� ����
                                professor->setName(newName);
                                cout << "���������� �̸��� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "��ȿ���� ���� �̸��Դϴ�.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 2: {
                            cout << "�� ��ȭ��ȣ�� �Է��ϼ���(000-0000-0000 �������� �Է�): ";
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
                                cout << "���������� ��ȭ��ȣ�� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "�ùٸ��� ���� ��ȭ��ȣ �����Դϴ�. 000-0000-0000 �������� �Է����ּ���.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 3: {
                            cout << "�� �̸����� �Է��ϼ���: ";
                            string newEmail;
                            cin >> newEmail;

                            // Email validation logic
                            const regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");
                            if (regex_match(newEmail, emailRegex)) {
                                professor->setEmail(newEmail);
                                cout << "���������� �̸����� ����Ǿ����ϴ�.\n";
                            }
                            else {
                                cout << "�ùٸ��� ���� �̸��� �����Դϴ�.\n";
                                isValid = false; // Mark as invalid
                            }
                            break;
                        }

                        case 4: {
                            cout << "�� ��й�ȣ�� �Է��ϼ���: ";
                            string newPassword;
                            cin >> newPassword;
                            professor->setPassword(newPassword);
                            cout << "���������� ��й�ȣ�� ����Ǿ����ϴ�.\n";
                            break;
                        }

                        case 0:
                            break;

                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                            isValid = false; // Mark as invalid
                            break;
                        }

                        if (isValid) {
                            // If all updates are valid, update the users CSV file
                            vector<User*> userPtrs;
                            for (const auto& user : users) {
                                userPtrs.push_back(user.get());  // unique_ptr�� raw pointer�� ��ȯ�Ͽ� �߰�
                            }
                            updateUsersCSV(userPtrs);
                            cout << "����� ������ CSV ���Ͽ� ���������� ������Ʈ�Ǿ����ϴ�.\n";
                        }
                        else {
                            cout << "������ ��ȿ���� �ʾ� ������� �ʾҽ��ϴ�.\n";
                        }
                    }
                    else {
                        cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n";
                    }
                    break;
                }

                }
            }


            else if (userType == "Administrator") {
                Administrator* admin = dynamic_cast<Administrator*>(loggedInUser);
                if (!admin) {
                    cout << "����: ������ ������ �ùٸ��� �ʽ��ϴ�.\n";
                    break;
                }
                switch (choice) {
                case 1:
                    // ���� ���� �޴�
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "���� ���� �޴�\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. ���� �߰��ϱ�\n";
                        cout << "2. ���� �����ϱ�\n";
                        cout << "3. ���� �����ϱ�\n";
                        cout << "4. ���� ��� ����\n";
                        cout << "0. ���� �޴��� ���ư���\n";
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        int subChoice;
                        cin >> subChoice;
                        cout << endl;

                        switch (subChoice) {
                        case 1:
                            admin->addSubject(subjects, users);
                            saveSubjects(subjectsFile, subjects); // ���� ���� ����
                            break;
                        case 2:
                            admin->deleteSubject(subjects);
                            saveSubjects(subjectsFile, subjects); // ���� ���� ����
                            break;
                        case 3:
                            admin->modifySubject(subjects, users);
                            saveSubjects(subjectsFile, subjects); // ���� ���� ����
                            break;
                        case 4:
                            admin->viewSubjects(subjects);
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                        }
                    }
                }
                break;
                case 2:
                    // ���� ���� �޴�
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "���� ���� �޴�\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. ���� ���� ��ȸ\n";
                        cout << "2. ���� �߰��ϱ�\n";
                        cout << "3. ���� �����ϱ�\n";
                        cout << "0. ���� �޴��� ���ư���\n";
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        int profChoice;
                        cin >> profChoice;
                        cout << endl;

                        switch (profChoice) {
                        case 1:
                            admin->viewProfessorInfo(users);
                            break;
                        case 2:
                            admin->addProfessor(users);
                            saveUsers(filename, users); // ���� ���� ����
                            break;
                        case 3:
                            admin->deleteProfessor(users);
                            saveUsers(filename, users); // ���� ���� ����
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                        }
                    }
                }
                break;
                case 3:
                    // �л� ���� �޴�
                {
                    bool back = false;
                    while (!back) {
                        cout << "==========================================================\n";
                        cout << "�л� ���� �޴�\n";
                        cout << "----------------------------------------------------------\n";
                        cout << "1. �л� ���� ��ȸ\n";
                        cout << "2. �л� �߰��ϱ�\n";
                        cout << "3. �л� �����ϱ�\n";
                        cout << "0. ���� �޴��� ���ư���\n";
                        cout << "==========================================================\n";
                        cout << "������ �Է��ϼ���: ";
                        int studentChoice;
                        cin >> studentChoice;
                        cout << endl;

                        switch (studentChoice) {
                        case 1:
                            admin->viewStudentInfo(users);
                            break;
                        case 2:
                            admin->addStudent(users);
                            saveUsers(filename, users); // ���� ���� ����
                            break;
                        case 3:
                            admin->deleteStudent(users);
                            saveUsers(filename, users); // ���� ���� ����
                            break;
                        case 0:
                            back = true;
                            break;
                        default:
                            cout << "�߸��� �����Դϴ�.\n";
                        }
                    }
                }
                break;
                default:
                    cout << "�߸��� �����Դϴ�.\n";
                }
            }
        }
    }
    return 0;
}