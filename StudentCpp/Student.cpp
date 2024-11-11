// Student.cpp
#include "Student.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace std;

// Constructor
Student::Student(string n, string i, string p, string phone, string mail, int sid)
    : User(n, i, p, phone, mail, "Student"), studentID(sid) {}

// Getters
int Student::getStudentID() const { return studentID; }
const vector<Subject>& Student::getSubjects() const { return subjects; }
const map<int, double>& Student::getGrades() const { return grades; }
const map<int, double>& Student::getScores() const { return scores; }
const map<int, string>& Student::getLetterGrades() const { return letterGrades; }

// Setters
void Student::setStudentID(int id) { studentID = id; }
void Student::setSubjects(const vector<Subject>& subs) { subjects = subs; }

string Student::getUserType() const { return "Student"; }

// ���� ���� �Լ�
void Student::setGrades(int subjectID, double gradeVal) {
    if (grades.find(subjectID) != grades.end()) {
        grades[subjectID] = gradeVal;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }

}
void Student::setScores(int subjectID, double scoreVal) {
	if (scores.find(subjectID) != scores.end()) {
		scores[subjectID] = scoreVal;
	}
	else {
		cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
	}
}

// ���� ���� ���� �Լ�
void Student::setLetterGrade(int subjectID, const string& letterGradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = letterGradeVal;
        //cout << "���� ID " << subjectID << "�� ���� ������ " << letterGradeVal << "(��)�� �����Ǿ����ϴ�.\n"; //������ �����
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }
}

void Student::updateLetterGrade(int subjectID, const string& gradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = gradeVal;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }
}

void Student::enrollSubject(const Subject& subject) {
    // �л��� �̹� ���� ��ϵǾ� �ִ��� Ȯ��
    for (const auto& sub : subjects) {
        if (sub.getID() == subject.getID()) {
            cout << "����: �̹� " << subject.getName() << " ���� ��ϵǾ� �ֽ��ϴ�.\n";
            return;
        }
    }

    subjects.push_back(subject);
    scores[subject.getID()] = 0.0;           // �ʱ� ������ 0.0���� ����
    grades[subject.getID()] = 0.0;           // �ʱ� ������ 0.0���� ����
    letterGrades[subject.getID()] = "N/A";  // �⺻ ���� ����
    //cout << "���������� " << subject.getName() << " ���� ��ϵǾ����ϴ�.\n"; // ����� ���

}

void Student::viewSubjects() const {
    cout << "\n��ϵ� �����:\n";
    for (const auto& subject : subjects) {
        cout << "- " << subject.getName() << " (" << subject.getCredit()
            << " ����, �б�: " << subject.getTerm() << " " << subject.getYear() << ")\n";
    }
}

void Student::viewGrades() const {
    cout << "\n��ġ ����:\n";
    for (const auto& gradeEntry : grades) {
        cout << "- ���� ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}

void Student::viewLetterGrades() const {
    cout << "\n���� ����:\n";
    for (const auto& gradeEntry : letterGrades) {
        cout << "- ���� ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}


// Ư�� ������ ���� ���� ��ȸ
string Student::getLetterGrade(int subjectID) const {
    auto it = letterGrades.find(subjectID);
    if (it != letterGrades.end()) {
        return it->second;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� ���� ������ �������� �ʽ��ϴ�.\n";
        return "N/A"; // �⺻ �� ��ȯ
    }
}

// ��ü ���� ��� ���
double Student::calculateAverageScore() const {
    double totalScore = 0.0;
    int count = 0;
    for (const auto& entry : scores) {
        totalScore += entry.second;
        count++;
    }
    if (count == 0) return 0.0; // ������ ������ 0 ��ȯ
    return totalScore / count; // ��� ��ȯ
}
