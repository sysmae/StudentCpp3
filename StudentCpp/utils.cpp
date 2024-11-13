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
    while ((ch = _getch()) != '\r') { // ����Ű�� ���� ������ �ݺ�
        if (ch == '\b') { // �齺���̽� ó��
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
    // ���� ����
    struct termios tty_old, tty_new;
    tcgetattr(STDIN_FILENO, &tty_old);
    tty_new = tty_old;
    tty_new.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty_new);

    while (std::cin.get(ch) && ch != '\n') {
        if (ch == '\b' || ch == 127) { // �齺���̽� ó��
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

    // ���� ����
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
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return users;
    }

    string line;
    getline(file, line); // ��� ���� �ǳʶٱ�

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
            cerr << "����� �����͸� ó���ϴ� �� ������ �߻��߽��ϴ�: " << e.what() << endl;
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
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return subjects;
    }

    string line;
    getline(file, line); // ��� ���� �ǳʶٱ�

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, credit, type, year, termStr, professorID;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, credit, ',');
        getline(ss, type, ',');
        getline(ss, year, ',');
        getline(ss, termStr, ',');  // �б� ���� �б�
        getline(ss, professorID);     // ���� ID �б�

        Term term;

        // �б� ���� ��ȯ
        if (termStr == "FIRST_TERM") {
            term = Term::FIRST_TERM;
        }
        else if (termStr == "SECOND_TERM") {
            term = Term::SECOND_TERM;
        }
        else {
            cerr << "�� �� ���� �б�: " << termStr << endl;
            continue; // �� �� ���� �б�� �ش� ���� �ǳʶٱ�
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
            cerr << "���� �����͸� ó���ϴ� �� ������ �߻��߽��ϴ�: " << e.what() << endl;
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
        cerr << "student_records.csv ������ �� �� �����ϴ�." << endl;
        return;
    }

    // ��� �ۼ�
    file << "studentID,subjectID,score,grade,letterGrade\n";

    // ���ڵ� �ۼ�
    for (const auto& student : students) {
        int studentID = student->getStudentID();
        const auto& grades = student->getGrades();
        const auto& scores = student->getScores();

        for (const auto& entry : grades) {
            int subjectID = entry.first;
            double grade = entry.second;
            double score = 0.0;

            // ���� ��������
            auto scoreIt = scores.find(subjectID);
            if (scoreIt != scores.end()) {
                score = scoreIt->second;
            }
            else {
                cerr << "����: subjectID " << subjectID << "�� ������ ã�� �� �����ϴ�." << endl;
            }

            // ��� ���� �Լ� ȣ��
            std::string letterGrade = mapGradeToLetter(grade);

            // ���Ͽ� ���
            file << studentID << "," << subjectID << ","
                << fixed << setprecision(2) << score << "," << grade << ","
                << letterGrade << "\n";
        }
    }

    file.close();
    cout << "student_records.csv ������ ���������� ������Ʈ�Ǿ����ϴ�." << endl;
}

vector<StudentRecord> loadStudentRecords(const string& filename) {
    vector<StudentRecord> studentRecords;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return studentRecords;
    }

    string line;
    getline(file, line);  // ��� ���� �ǳʶٱ�

    while (getline(file, line)) {
        stringstream ss(line);
        string studentID, subjectID, score, grade, letterGrade;

        getline(ss, studentID, ',');
        getline(ss, subjectID, ',');
        getline(ss, score, ',');
        getline(ss, grade, ',');
        getline(ss, letterGrade, ',');

        try {
            int sid = stoi(studentID);     // �л� ID
            int subid = stoi(subjectID);   // ���� ID
            double scr = stod(score);      // ���� (CSV���� ������)
            double grd = stod(grade);      // ���
            string letter = letterGrade.empty() ? "F" : letterGrade; // ���� ���

            // �л� ��� ��ü ���� �� �߰�
            StudentRecord record(sid, subid, scr,letter,grd);  // ������ CSV���� ������ ������ ����
			record.setScore(scr);				   // ���� ����
            record.setGrade(grd);                   // ��� ����
            record.setLetterGrade(letter);          // ���� ��� ����
            studentRecords.push_back(record);       // �л� ��� �߰�
        }
        catch (const invalid_argument& e) {
            cerr << "��ȿ���� ���� ������ ���� (��: " << line << "): " << e.what() << endl;
        }
        catch (const out_of_range& e) {
            cerr << "���� ������ �ʰ��߽��ϴ� (��: " << line << "): " << e.what() << endl;
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
    cout << "=== �α��� ===\n";
    cout << "ID: ";
    cin >> inputID;
    cout << "��й�ȣ: ";
    inputPassword = getMaskedInput(); // ������ �κ�

    for (auto& user : users) {
        if (user->getID() == inputID && user->getPassword() == inputPassword) {
            return user.get();
        }
    }
    return nullptr;
}

// Display menu based on user type
void displayMenu(const string& userType) {
    cout << "\n=== �޴� ===\n";
    if (userType == "Student") {
        cout << "1. ���б� ������� ��ȸ\n";
        cout << "2. �����б� ������� ��ȸ\n";
        cout << "3. ������� ��ȸ\n";
        cout << "4. �� ���� ����\n";
    }
    else if (userType == "Professor") {
        cout << "1. ���б� ��� ���� ��ȸ \n";
        cout << "2. ���� �б� ��� ���� ��ȸ(2024�� 2�б� ����)\n";
        cout << "3. �� ���� ����\n";
    }
    else if (userType == "Administrator") {
        cout << "1. ���� ����\n";
        cout << "2. ���� ����\n";
        cout << "3. �л� ����\n";
    }
    cout << "0. �α׾ƿ�\n";
}

// ������׷��� ǥ���ϴ� �Լ�
void displayHistogram(const vector<StudentRecord>& records) {
    const int maxScore = 100; // ���� ���� 0-100 ����
    const int numBins = 11; // 0-9, 10-19, ..., 100
    vector<int> scoreBins(numBins, 0); // �� ���� ������ �л� �� ����

    // ������ �� ������ �з�
    for (const auto& record : records) {
        int score = record.getScore();
        int index = score / 10;
        if (index >= numBins) index = numBins - 1; // 100�� ó��
        scoreBins[index]++;
    }


    // 1. �� �л� �� ���
    cout << "\n================= ������׷� =================\n";
    cout << "�� �л� ��: " << records.size() << "��\n";



    // 2. ������׷� ��� (�л� �� ����, �� ������ +)
    cout << "\n���� ���� ������׷�\n";
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
        std::cerr << "������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }
        
    // ��� �ۼ�
    file << "ID,Password,Name,PhoneNumber,Email,UserType,StudentID\n";

    // ����� ������ �ۼ�
    for (const auto& user : users) {
        file << user->getID() << ","
            << user->getPassword() << ","
            << user->getName() << ","
            << user->getPhoneNumber() << ","
            << user->getEmail() << ","
            << user->getUserType();

        // Student�� ��� studentID �߰�
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
        cerr << "users.csv ������ �� �� �����ϴ�." << endl;
        return;
    }


    // ��� �ۼ�
    file << "ID,Password,Name,PhoneNumber,Email,UserType,StudentID\n";

    for (const auto& user : users) {
        string userID = user->getID();
        std::string password = user->getPassword();
        std::string name = user->getName();
        std::string phoneNumber = user->getPhoneNumber();
        std::string email = user->getEmail();
        std::string userType = user->getUserType();

        // �⺻ ���� ���
        file << userID << "," << password << "," << name << "," << phoneNumber << ","
            << email << "," << userType;

        // Student�� ��� studentID �߰�
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

        // �� ���� �� �� �߰�
        file << "\n";
    }
    file.close();
    std::cout << "users.csv" << " ������ ���������� ������Ʈ�Ǿ����ϴ�." << std::endl;
}


// Function to save subjects to CSV
void saveSubjects(const std::string& filename, const std::vector<Subject>& subjects) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������ �� �� �����ϴ�: " << filename << std::endl;
        return;
    }

    // ��� �ۼ�
    file << "ID,Name,Credit,Type,Year,Term,ProfessorID\n";

    // ���� ������ �ۼ�
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