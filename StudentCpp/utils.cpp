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
    while ((ch = _getch()) != '\r') { // ����Ű�� ���� ������ �ݺ�
        if (ch == '\b') { // �齺���̽� ó��
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
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    vector<string> fileLines;    // ���� ������ ��� ������ ����
    string line;
    getline(inputFile, line);    // ��� ���� �б�
    fileLines.push_back(line);   // ��� ����

    // ��� ���� ������ �о ���Ϳ� ����
    while (getline(inputFile, line)) {
        fileLines.push_back(line);
    }
    inputFile.close();

    // studentID�� subjectID �������� ������ ���ڵ� �ݿ�
    for (auto& recordLine : fileLines) {
        stringstream ss(recordLine);
        string studentID, subjectID, score, grade, letterGrade, professorID;

        getline(ss, studentID, ',');
        getline(ss, subjectID, ',');
        getline(ss, score, ',');
        getline(ss, grade, ',');
        getline(ss, letterGrade, ',');
        getline(ss, professorID);

        // ������ ���ڵ带 ã�� ���� studentID�� subjectID ��
        for (const auto& updatedRecord : updatedRecords) {
            if (studentID == to_string(updatedRecord.getStudentID()) && subjectID == to_string(updatedRecord.getSubjectID())) {
                stringstream updatedLine;
                updatedLine << updatedRecord.getStudentID() << ","
                            << updatedRecord.getSubjectID() << ","
                            << fixed << setprecision(2) << updatedRecord.getScore() << ","
                            << updatedRecord.getGrade() << ","
                            << updatedRecord.getLetterGrade() << ","
                            << updatedRecord.getProfessorID();
                recordLine = updatedLine.str();  // ���� ���� �����
                break;
            }
        }
    }

    // ���� �����
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return;
    }

    // ������Ʈ�� �����͸� ���Ͽ� ����
    for (const auto& fileLine : fileLines) {
        outputFile << fileLine << "\n";
    }

    outputFile.close();
    cout << filename << " ������ ���������� ������Ʈ�Ǿ����ϴ�." << endl;
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
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
        return studentRecords;
    }

    string line;
    getline(file, line);  // ��� ���� �ǳʶٱ�

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
            int sid = stoi(studentID);     // �л� ID
            int subid = stoi(subjectID);   // ���� ID
            double scr = stod(score);      // ����
            double grd = stod(grade);      // ����
            string letter = letterGrade.empty() ? "F" : letterGrade; // ���� ����
            string profID = professorID;   // ���� ID

            // StudentRecord ��ü ���� �� ���Ϳ� �߰�
            StudentRecord record(sid, subid, scr, grd, letter, profID);
            studentRecords.push_back(record);

        } catch (const invalid_argument& e) {
            cerr << "��ȿ���� ���� ������ ���� (��: " << line << "): " << e.what() << endl;
        } catch (const out_of_range& e) {
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
        cout << "������ ���� ���� �л� ����� �����ϴ�.\n";
        return;
    }

    const int maxScore = 100; // ���� ���� 0-100 ����
    const int numBins = 11;   // 0-9, 10-19, ..., 100
    vector<int> scoreBins(numBins, 0); // �� ���� ������ �л� �� ����

    // ������ �� ������ �з�
    for (const auto& record : filteredRecords) {
        int score = static_cast<int>(record.getScore());
        int index = score / 10;
        if (index >= numBins) index = numBins - 1; // 100�� ó��
        scoreBins[index]++;
    }

    // ������׷� ���
    cout << "\n================= ������׷� =================\n";
    cout << "���� ID: " << subjectID << "\n";
    cout << "�� �л� ��: " << filteredRecords.size() << "��\n";
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
void saveUsers(const string& filename, const vector<unique_ptr<User>>& users) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
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
        string password = user->getPassword();
        string name = user->getName();
        string phoneNumber = user->getPhoneNumber();
        string email = user->getEmail();
        string userType = user->getUserType();

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
    cout << "users.csv" << " ������ ���������� ������Ʈ�Ǿ����ϴ�." << endl;
}


// Function to save subjects to CSV
void saveSubjects(const string& filename, const vector<Subject>& subjects) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filename << endl;
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