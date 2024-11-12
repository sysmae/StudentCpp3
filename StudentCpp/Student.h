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
    vector<Subject> subjects;            // 학생이 수강하는 과목 리스트
    map<int, double> scores;             // 과목 ID와 점수(100점 만점) 매핑
    map<int, double> grades;             // 과목 ID와 평점(4.5 만점) 매핑
    map<int, string> letterGrades;       // 과목 ID와 문자 성적(A+ 만점) 매핑

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

    // 문자 성적 설정 함수
    void setLetterGrade(int subjectID, const string& letterGradeVal);

    void updateLetterGrade(int subjectID, const string& grade);

    void enrollSubject(const Subject& subject);

    void viewSubjects() const;

    void viewPreviousSubjects(int year, int term) const;

    void viewGrades() const;

    void viewLetterGrades() const;

    // 특정 과목의 문자 성적 조회
    string getLetterGrade(int subjectID) const;

    //졸업요건 조회
    void displayGraduationRequirements();

    // 전체 성적 평균 계산
    double calculateAverageScore() const;

    //test
    void printSubjectsCount() const;

    void someFunction();

    
};

#endif // STUDENT_H
