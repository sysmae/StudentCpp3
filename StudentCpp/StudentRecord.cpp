// StudentRecord.cpp
#include "StudentRecord.h"
#include <stdexcept>
#include <string>

// �⺻ ������
StudentRecord::StudentRecord()
    : studentID(0), subjectID(0), score(0.0), letterGrade(""), grade(0.0) {}

// ��ü ������
StudentRecord::StudentRecord(int sid, int subid, double sc, string lg, double grade)
    : studentID(sid), subjectID(subid), letterGrade(lg){
    setScore(sc);
    setGrade(grade);
}

// �л� ID ��������
int StudentRecord::getStudentID() const { return studentID; }

// ���� �������� (����)
double StudentRecord::getScore() const { return score; }

// ���� �����ϱ� (0.0���� 100.0 ���̷� ����)
void StudentRecord::setScore(double sc) {
    if (sc < 0.0 || sc > 100.0) {
        throw std::invalid_argument("Score must be between 0.0 and 100.0");
    }
    score = sc;
}

// ���� ���� ��������
string StudentRecord::getLetterGrade() const { return letterGrade; }

// ���� ���� �����ϱ�
void StudentRecord::setLetterGrade(const string& lg) { letterGrade = lg; }

// ���� ��������
double StudentRecord::getGrade() const { return grade; }

// ���� �����ϱ�
void StudentRecord::setGrade(double g) {
    if (g < 0.0 || g > 4.5) {
        throw std::invalid_argument("Grade must be between 0.0 and 4.5");
    }
    grade = g;
}

// ���� ID ��������
int StudentRecord::getSubjectID() const { return subjectID; }
