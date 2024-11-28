#include "StudentRecord.h"
#include <stdexcept>
#include <string>


// �⺻ ������
StudentRecord::StudentRecord()
    : studentID(0), subjectID(0), score(0.0), grade(0.0), letterGrade("F"), professorID("") {}

// ��ü ������
StudentRecord::StudentRecord(int sid, int subid, double sc, double gr, string lg, string pid) 
    : studentID(sid), subjectID(subid), score(0.0), grade(0.0), letterGrade(lg), professorID(pid) {
    setScore(sc);  // ���� �˻� ����
    setGrade(gr);  // ���� �˻� ����
}

// �л� ID getter/setter
int StudentRecord::getStudentID() const { return studentID; }
void StudentRecord::setStudentID(int sid) { studentID = sid; }

// ���� ID getter/setter
int StudentRecord::getSubjectID() const { return subjectID; }
void StudentRecord::setSubjectID(int subid) { subjectID = subid; }

// ���� (����) getter/setter
double StudentRecord::getScore() const { return score; }
void StudentRecord::setScore(double sc) {
    if (sc < 0.0 || sc > 100.0) {
        throw invalid_argument("Score must be between 0.0 and 100.0");
    }
    score = sc;
}

// ���� getter/setter
double StudentRecord::getGrade() const { return grade; }
void StudentRecord::setGrade(double gr) {
    if (gr < 0.0 || gr > 4.5) {
        throw invalid_argument("Grade must be between 0.0 and 4.5");
    }
    grade = gr;
}

// ���� ���� getter/setter
string StudentRecord::getLetterGrade() const { return letterGrade; }
void StudentRecord::setLetterGrade(const string& lg) { letterGrade = lg; }

// ���� ID getter/setter
string StudentRecord::getProfessorID() const { return professorID; }
void StudentRecord::setProfessorID(const string& pid) { professorID = pid; }
