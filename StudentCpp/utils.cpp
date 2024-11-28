// utils.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "Student.h"
#include "Professor.h"
#include "Administrator.h"
#include "Subject.h"
#include "StudentRecord.h"
#include "User.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <iomanip> 

#include <string>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

#endif

using namespace std;

// Function to mask password input
string getMaskedInput() {
    string password;
    char ch;

#ifdef _WIN32
    while ((ch = _getch()) != '\r') { // 엔터키를 누를 때까지 반복
        if (ch == '\b') { // 백스페이스 처리
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
#else
    while (cin.get(ch) && ch != '\n') {
        if (ch == '\b' || ch == 127) {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
#endif
    cout << endl;
    return password;
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

// Function to load users from CSV
vector<unique_ptr<User>> loadUsers(const string& filename) {
    vector<unique_ptr<User>> users;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return users;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, password, name, phoneNumber, email, userType, studentID;
        getline(ss, id, ',');
        getline(ss, password, ',');
        getline(ss, name, ',');
        getline(ss, phoneNumber, ',');
        getline(ss, email, ',');
        getline(ss, userType, ',');
        getline(ss, studentID, ',');

        if (userType == "Student") {
            users.push_back(make_unique<Student>(name, id, password, phoneNumber, email, stoi(studentID)));
        }
        else if (userType == "Professor") {
            users.push_back(make_unique<Professor>(name, id, password, phoneNumber, email));
        }
        else if (userType == "Administrator") {
            users.push_back(make_unique<Administrator>(name, id, password, phoneNumber, email));
        }
    }

    return users;
}

// Function to load subjects from CSV
vector<Subject> loadSubjects(const string& filename) {
    vector<Subject> subjects;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return subjects;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id, year;
        double credit;
        string name, type, termStr, professorID;
        Term term;

        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        ss >> credit;
        ss.ignore();
        getline(ss, type, ',');
        ss >> year;
        ss.ignore();
        getline(ss, termStr, ',');
        getline(ss, professorID, ',');

        if (termStr == "FIRST_TERM") {
            term = Term::FIRST_TERM;
        }
        else if (termStr == "SECOND_TERM") {
            term = Term::SECOND_TERM;
        }
        else {
            continue;
        }

        subjects.emplace_back(id, name, credit, type, year, term, professorID);
    }

    return subjects;
}

// // Function to load student records from CSV
// vector<StudentRecord> loadStudentRecords(const string& filename) {
//     vector<StudentRecord> records;
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Error opening file: " << filename << endl;
//         return records;
//     }

//     string line;
//     while (getline(file, line)) {
//         stringstream ss(line);
//         int studentID, subjectID;
//         double score, grade;
//         string letterGrade, professorID;

//         ss >> studentID;
//         ss.ignore();
//         ss >> subjectID;
//         ss.ignore();
//         ss >> score;
//         ss.ignore();
//         ss >> grade;
//         ss.ignore();
//         getline(ss, letterGrade, ',');
//         getline(ss, professorID, ',');

//         records.emplace_back(studentID, subjectID, score, grade, letterGrade, professorID);
//     }

//     return records;
// }



void updateStudentRecordsCSV(const vector<StudentRecord>& updatedRecords) {
    const string filename = "student_records.csv";
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    vector<string> fileLines;    // 기존 파일의 모든 라인을 저장
    string line;
    getline(inputFile, line);    // 헤더 라인 읽기
    fileLines.push_back(line);   // 헤더 유지

    // 모든 파일 라인을 읽어서 벡터에 저장
    while (getline(inputFile, line)) {
        fileLines.push_back(line);
    }
    inputFile.close();

    // studentID와 subjectID 기준으로 수정된 레코드 반영
    for (auto& recordLine : fileLines) {
        stringstream ss(recordLine);
        string studentID, subjectID, score, grade, letterGrade, professorID;

        getline(ss, studentID, ',');
        getline(ss, subjectID, ',');
        getline(ss, score, ',');
        getline(ss, grade, ',');
        getline(ss, letterGrade, ',');
        getline(ss, professorID);

        // 수정할 레코드를 찾기 위해 studentID와 subjectID 비교
        for (const auto& updatedRecord : updatedRecords) {
            if (studentID == to_string(updatedRecord.getStudentID()) && subjectID == to_string(updatedRecord.getSubjectID())) {
                stringstream updatedLine;
                updatedLine << updatedRecord.getStudentID() << ","
                            << updatedRecord.getSubjectID() << ","
                            << fixed << setprecision(2) << updatedRecord.getScore() << ","
                            << updatedRecord.getGrade() << ","
                            << updatedRecord.getLetterGrade() << ","
                            << updatedRecord.getProfessorID();
                recordLine = updatedLine.str();  // 기존 라인 덮어쓰기
                break;
            }
        }
    }

    // 파일 덮어쓰기
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    // 업데이트된 데이터를 파일에 저장
    for (const auto& fileLine : fileLines) {
        outputFile << fileLine << "\n";
    }

    outputFile.close();
    cout << filename << " 파일이 성공적으로 업데이트되었습니다." << endl;
}
string mapGradeToLetter(double grade) {
    if (grade >= 4.5) {
        return "A+";
    }
    else if (grade >= 4.0) {
        return "A";
    }
    else if (grade >= 3.5) {
        return "B+";
    }
    else if (grade >= 3.0) {
        return "B";
    }
    else if (grade >= 2.5) {
        return "C+";
    }
    else if (grade >= 2.0) {
        return "C";
    }
	else if (grade >= 1.5) {
		return "D+";
	}
	else if (grade >= 1.0) {
		return "D";
	}
    else {
        return "F";
    }
}

vector<StudentRecord> loadStudentRecords(const string& filename) {
    vector<StudentRecord> studentRecords;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return studentRecords;
    }

    string line;
    getline(file, line);  // 헤더 라인 건너뛰기

    while (getline(file, line)) {
        stringstream ss(line);
        string studentID, subjectID, score, grade, letterGrade, professorID;

        getline(ss, studentID, ',');
        getline(ss, subjectID, ',');
        getline(ss, score, ',');
        getline(ss, grade, ',');
        getline(ss, letterGrade, ',');
        getline(ss, professorID);

        try {
            int sid = stoi(studentID);     // 학생 ID
            int subid = stoi(subjectID);   // 과목 ID
            double scr = stod(score);      // 점수
            double grd = stod(grade);      // 평점
            string letter = letterGrade.empty() ? "F" : letterGrade; // 문자 성적
            string profID = professorID;   // 교수 ID

            // StudentRecord 객체 생성 및 벡터에 추가
            StudentRecord record(sid, subid, scr, grd, letter, profID);
            studentRecords.push_back(record);

        } catch (const invalid_argument& e) {
            cerr << "유효하지 않은 데이터 형식 (행: " << line << "): " << e.what() << endl;
        } catch (const out_of_range& e) {
            cerr << "값이 범위를 초과했습니다 (행: " << line << "): " << e.what() << endl;
        }
    }

    file.close();
    return studentRecords;
}



// Helper function to find a subject by ID
Subject* findSubject(vector<Subject>& subjects, int subjectId) {
    auto it = find_if(subjects.begin(), subjects.end(),
        [subjectId](const Subject& s) { return s.getID() == subjectId; });
    return it != subjects.end() ? &(*it) : nullptr;
}

// User login function
User* login(vector<unique_ptr<User>>& users) {
    string inputID, inputPassword;
    cout << "=== 로그인 ===\n";
    cout << "ID: ";
    cin >> inputID;
    cout << "비밀번호: ";
    inputPassword = getMaskedInput(); // 수정된 부분

    for (auto& user : users) {
        if (user->getID() == inputID && user->getPassword() == inputPassword) {
            return user.get();
        }
    }
    return nullptr;
}

// Display menu based on user type
void displayMenu(const string& userType) {
    cout << "\n=== 메뉴 ===\n";
    if (userType == "Student") {
        cout << "1. 금학기 수강목록 조회\n";
        cout << "2. 지난학기 수강목록 조회\n";
        cout << "3. 졸업요건 조회\n";
        cout << "4. 내 정보 수정\n";
    }
    else if (userType == "Professor") {
        cout << "1. 금학기 담당 과목 조회 \n";
        cout << "2. 지난 학기 담당 과목 조회(2024년 2학기 이전)\n";
        cout << "3. 내 정보 수정\n";
    }
    else if (userType == "Administrator") {
        cout << "1. 과목 관리\n";
        cout << "2. 교수 관리\n";
        cout << "3. 학생 관리\n";
    }
    cout << "0. 로그아웃\n";
}

vector<StudentRecord> filterRecordsBySubject(const vector<StudentRecord>& records, int subjectID) {
    vector<StudentRecord> filteredRecords;
    for (const auto& record : records) {
        if (record.getSubjectID() == subjectID) {
            filteredRecords.push_back(record);
        }
    }
    return filteredRecords;
}


void displayHistogramForSubject(const vector<StudentRecord>& records, int subjectID) {
    vector<StudentRecord> filteredRecords = filterRecordsBySubject(records, subjectID);

    if (filteredRecords.empty()) {
        cout << "선택한 과목에 대한 학생 기록이 없습니다.\n";
        return;
    }

    const int maxScore = 100; // 점수 범위 0-100 가정
    const int numBins = 11;   // 0-9, 10-19, ..., 100
    vector<int> scoreBins(numBins, 0); // 각 점수 구간별 학생 수 저장

    // 점수를 각 구간에 분류
    for (const auto& record : filteredRecords) {
        int score = static_cast<int>(record.getScore());
        int index = score / 10;
        if (index >= numBins) index = numBins - 1; // 100점 처리
        scoreBins[index]++;
    }

    // 히스토그램 출력
    cout << "\n================= 히스토그램 =================\n";
    cout << "과목 ID: " << subjectID << "\n";
    cout << "총 학생 수: " << filteredRecords.size() << "명\n";
    cout << "\n성적 분포 히스토그램\n";
    for (int i = 0; i < numBins; ++i) {
        int lowerBound = i * 10;
        int upperBound = (i == numBins - 1) ? 100 : lowerBound + 9;
        string range = to_string(lowerBound) + "-" + to_string(upperBound);
        cout << setw(10) << range << " | ";
        cout << "(" << scoreBins[i] << ") ";
        for (int j = 0; j < scoreBins[i]; ++j) {
            cout << "+";
        }
        cout << "\n";
    }
    cout << "=============================================\n";
}


// Function to save users to CSV
void saveUsers(const string& filename, const vector<unique_ptr<User>>& users) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }
        
    // 헤더 작성
    file << "ID,Password,Name,PhoneNumber,Email,UserType,StudentID\n";

    // 사용자 데이터 작성
    for (const auto& user : users) {
        file << user->getID() << ","
            << user->getPassword() << ","
            << user->getName() << ","
            << user->getPhoneNumber() << ","
            << user->getEmail() << ","
            << user->getUserType();

        // Student일 경우 studentID 추가
        if (user->getUserType() == "Student") {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                file << "," << student->getStudentID();
            }
            else {
                file << ",";
            }
        }
        else {
            file << ",";
        }

        file << "\n";
    }

    file.close();
}


void updateUsersCSV(const vector<User*>& users) {
    ofstream file("users.csv");

    if (!file.is_open()) {
        cerr << "users.csv 파일을 열 수 없습니다." << endl;
        return;
    }


    // 헤더 작성
    file << "ID,Password,Name,PhoneNumber,Email,UserType,StudentID\n";

    for (const auto& user : users) {
        string userID = user->getID();
        string password = user->getPassword();
        string name = user->getName();
        string phoneNumber = user->getPhoneNumber();
        string email = user->getEmail();
        string userType = user->getUserType();

        // 기본 정보 기록
        file << userID << "," << password << "," << name << "," << phoneNumber << ","
            << email << "," << userType;

        // Student일 경우 studentID 추가
        if (userType == "Student") {
            const Student* student = dynamic_cast<const Student*>(user);
            if (student) {
                file << "," << student->getStudentID();
            }
            else {
                file << ",";
            }
        }
        else {
            file << ",";
        }

        // 행 끝에 새 줄 추가
        file << "\n";
    }
    file.close();
    cout << "users.csv" << " 파일이 성공적으로 업데이트되었습니다." << endl;
}


// Function to save subjects to CSV
void saveSubjects(const string& filename, const vector<Subject>& subjects) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    // 헤더 작성
    file << "ID,Name,Credit,Type,Year,Term,ProfessorID\n";

    // 과목 데이터 작성
    for (const auto& subject : subjects) {
        file << subject.getID() << ","
            << subject.getName() << ","
            << subject.getCredit() << ","
            << subject.getType() << ","
            << subject.getYear() << ","
            << (subject.getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << ","
            << subject.getProfessorID() << "\n";
    }

    file.close();
}