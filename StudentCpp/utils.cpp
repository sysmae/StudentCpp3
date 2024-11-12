// utils.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "Student.h"
#include "Professor.h"
#include "Administrator.h"
#include "StudentRecord.h" 

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
std::string getMaskedInput() {
    std::string password;
    char ch;

#ifdef _WIN32
    while ((ch = _getch()) != '\r') { // 엔터키를 누를 때까지 반복
        if (ch == '\b') { // 백스페이스 처리
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }
#else
    // 에코 끄기
    struct termios tty_old, tty_new;
    tcgetattr(STDIN_FILENO, &tty_old);
    tty_new = tty_old;
    tty_new.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty_new);

    while (std::cin.get(ch) && ch != '\n') {
        if (ch == '\b' || ch == 127) { // 백스페이스 처리
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }

    // 에코 복원
    tcsetattr(STDIN_FILENO, TCSANOW, &tty_old);
#endif

    std::cout << std::endl;
    return password;
}

// Function to load users from CSV
vector<unique_ptr<User>> loadUsers(const string& filename) {
    vector<unique_ptr<User>> users;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return users;
    }

    string line;
    getline(file, line); // 헤더 라인 건너뛰기

    while (getline(file, line)) {
        stringstream ss(line);
        string id, password, name, phoneNumber, email, userType, studentID;

        getline(ss, id, ',');
        getline(ss, password, ',');
        getline(ss, name, ',');
        getline(ss, phoneNumber, ',');
        getline(ss, email, ',');
        getline(ss, userType, ',');

        try {
            if (userType == "Student") {
                getline(ss, studentID, ',');
                users.push_back(make_unique<Student>(
                    name, id, password, phoneNumber, email, stoi(studentID)));
            }
            else if (userType == "Professor") {
                users.push_back(make_unique<Professor>(
                    name, id, password, phoneNumber, email));
            }
            else if (userType == "Administrator") {
                users.push_back(make_unique<Administrator>(
                    name, id, password, phoneNumber, email));
            }
        }
        catch (const exception& e) {
            cerr << "사용자 데이터를 처리하는 중 오류가 발생했습니다: " << e.what() << endl;
        }
    }

    file.close();
    return users;
}

// Function to load subjects from CSV
vector<Subject> loadSubjects(const string& filename) {
    vector<Subject> subjects;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return subjects;
    }

    string line;
    getline(file, line); // 헤더 라인 건너뛰기

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, credit, type, year, termStr, professorID;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, credit, ',');
        getline(ss, type, ',');
        getline(ss, year, ',');
        getline(ss, termStr, ',');  // 학기 정보 읽기
        getline(ss, professorID);     // 교수 ID 읽기

        Term term;

        // 학기 정보 변환
        if (termStr == "FIRST_TERM") {
            term = Term::FIRST_TERM;
        }
        else if (termStr == "SECOND_TERM") {
            term = Term::SECOND_TERM;
        }
        else {
            cerr << "알 수 없는 학기: " << termStr << endl;
            continue; // 알 수 없는 학기면 해당 과목 건너뛰기
        }

        try {
            subjects.emplace_back(Subject(
                stoi(id),
                name,
                stod(credit),
                type,
                stoi(year),
                term,
                professorID
            ));
        }
        catch (const exception& e) {
            cerr << "과목 데이터를 처리하는 중 오류가 발생했습니다: " << e.what() << endl;
        }
    }

    file.close();
    return subjects;
}
std::string mapGradeToLetter(double grade) {
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
void updateStudentRecordsCSV(const vector<Student*>& students) {
    ofstream file("student_records.csv");

    if (!file.is_open()) {
        cerr << "student_records.csv 파일을 열 수 없습니다." << endl;
        return;
    }

    // 헤더 작성
    file << "studentID,subjectID,score,grade,letterGrade\n";

    // 레코드 작성
    for (const auto& student : students) {
        int studentID = student->getStudentID();
        const auto& grades = student->getGrades();
        const auto& scores = student->getScores();

        for (const auto& entry : grades) {
            int subjectID = entry.first;
            double grade = entry.second;
            double score = 0.0;

            // 점수 가져오기
            auto scoreIt = scores.find(subjectID);
            if (scoreIt != scores.end()) {
                score = scoreIt->second;
            }
            else {
                cerr << "오류: subjectID " << subjectID << "의 점수를 찾을 수 없습니다." << endl;
            }

            // 등급 매핑 함수 호출
            std::string letterGrade = mapGradeToLetter(grade);

            // 파일에 기록
            file << studentID << "," << subjectID << ","
                << fixed << setprecision(2) << score << "," << grade << ","
                << letterGrade << "\n";
        }
    }

    file.close();
    cout << "student_records.csv 파일이 성공적으로 업데이트되었습니다." << endl;
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
        string studentID, subjectID, score, grade, letterGrade;

        getline(ss, studentID, ',');
        getline(ss, subjectID, ',');
        getline(ss, score, ',');
        getline(ss, grade, ',');
        getline(ss, letterGrade, ',');

        try {
            int sid = stoi(studentID);     // 학생 ID
            int subid = stoi(subjectID);   // 과목 ID
            double scr = stod(score);      // 점수 (CSV에서 가져옴)
            double grd = stod(grade);      // 등급
            string letter = letterGrade.empty() ? "F" : letterGrade; // 문자 등급

            // 학생 기록 객체 생성 및 추가
            StudentRecord record(sid, subid, scr,letter,grd);  // 점수를 CSV에서 가져온 값으로 설정
			record.setScore(scr);				   // 점수 설정
            record.setGrade(grd);                   // 등급 설정
            record.setLetterGrade(letter);          // 문자 등급 설정
            studentRecords.push_back(record);       // 학생 기록 추가
        }
        catch (const invalid_argument& e) {
            cerr << "유효하지 않은 데이터 형식 (행: " << line << "): " << e.what() << endl;
        }
        catch (const out_of_range& e) {
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

// 히스토그램을 표시하는 함수
void displayHistogram(const vector<StudentRecord>& records) {
    const int maxScore = 100; // 점수 범위 0-100 가정
    const int numBins = 11; // 0-9, 10-19, ..., 100
    vector<int> scoreBins(numBins, 0); // 각 점수 구간별 학생 수 저장

    // 점수를 각 구간에 분류
    for (const auto& record : records) {
        int score = record.getScore();
        int index = score / 10;
        if (index >= numBins) index = numBins - 1; // 100점 처리
        scoreBins[index]++;
    }


    // 1. 총 학생 수 출력
    cout << "\n================= 히스토그램 =================\n";
    cout << "총 학생 수: " << records.size() << "명\n";



    // 2. 히스토그램 출력 (학생 수 먼저, 그 다음에 +)
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
void saveUsers(const std::string& filename, const std::vector<std::unique_ptr<User>>& users) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
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
        std::string password = user->getPassword();
        std::string name = user->getName();
        std::string phoneNumber = user->getPhoneNumber();
        std::string email = user->getEmail();
        std::string userType = user->getUserType();

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
    std::cout << "users.csv" << " 파일이 성공적으로 업데이트되었습니다." << std::endl;
}


// Function to save subjects to CSV
void saveSubjects(const std::string& filename, const std::vector<Subject>& subjects) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
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