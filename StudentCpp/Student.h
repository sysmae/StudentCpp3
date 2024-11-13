// Student.h
#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Subject.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class Student : public User {
private:
    int studentID;
    vector<Subject> subjects;            // �л��� �����ϴ� ���� ����Ʈ
    map<int, double> scores;             // ���� ID�� ����(100�� ����) ����
    map<int, double> grades;             // ���� ID�� ����(4.5 ����) ����
    map<int, string> letterGrades;       // ���� ID�� ���� ����(A+ ����) ����

public:
    Student(string n, string i, string p, string phone, string mail, int sid);

    int getStudentID() const;
    const vector<Subject>& getSubjects() const;
    const map<int, double>& getGrades() const;
    const map<int, double>& getScores() const;

    const map<int, string>& getLetterGrades() const;

    void setStudentID(int id);
    void setSubjects(const vector<Subject>& subs);

    string getUserType() const override;

    void setGrades(int subjectID, double gradeVal);
    void setScores(int subjectID, double scoreVal);

    // ���� ���� ���� �Լ�
    void setLetterGrade(int subjectID, const string& letterGradeVal);

    void updateLetterGrade(int subjectID, const string& grade);

    void enrollSubject(const Subject& subject);

    void viewSubjects() const;

    void viewPreviousSubjects(int year, int term) const;

    void viewGrades() const;

    void viewLetterGrades() const;

    // Ư�� ������ ���� ���� ��ȸ
    string getLetterGrade(int subjectID) const;

    //������� ��ȸ
    void displayGraduationRequirements();

    // ��ü ���� ��� ���
    double calculateAverageScore() const;

    //test
    void printSubjectsCount() const;

    void someFunction();

    
};

#endif // STUDENT_H
