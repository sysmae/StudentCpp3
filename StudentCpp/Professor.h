// Professor.h
#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "User.h"
#include "Subject.h"
#include "ENUMS.h"
#include <vector>
#include <string>
#include "Student.h"

using namespace std;

class Professor : public User {
private:
    vector<Subject> subjects; // 교수의 수업 과목 리스트
    GradingMethod gradingMethod; // 성적 평가 방식 변수 추가

public:
    Professor(string n, string i, string p, string phone, string mail);

    vector<Subject> getSubjects() const;
    void setSubjects(const vector<Subject>& subs);

    string getUserType() const override;

    void setGradingMethod(GradingMethod method);
    GradingMethod getGradingMethod() const;

    void viewSubjects(int year, int term) const;

    void printStudentGrades(const vector<Student*>& enrolledStudents, const Subject* subject, int subjectId);

    //void viewSubjects() const;

    void viewPreviousSubjects(int year, int term) const;

    // 교수의 수업 과목을 로드하는 함수
    void loadTeachingSubjects(const vector<Subject>& allSubjects);

    void inputStudentScores(vector<Student*>& students, Subject& subject);

    // 성적 부여 함수
    void assignGrades(vector<class Student*>& students, Subject& subject);
};

#endif // PROFESSOR_H
