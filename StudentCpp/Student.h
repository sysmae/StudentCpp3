// Student.h
#ifndef STUDENT_H
#define STUDENT_H

#include "user.h"
#include "subject.h"
#include "studentRecord.h"
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

    //void viewSubjects(int year, int term) const;

    bool viewSubjectsByTerm(int year, int term, bool showHeader) const;

    void viewCurSubjects(int year, int term) const;


    void viewAllPreviousSubjects(int year, int term) const;

    void viewGrades() const;

    void viewLetterGrades() const;

    // 특정 과목의 문자 성적 조회
    string getLetterGrade(int subjectID) const;

    void viewGradeBySubjectID(int subjectID) const;

    //졸업요건 조회

    // 전체 성적 평균 계산 일단 
    //double calculateAverageScore() const;

    void checkGraduationRequirements(const std::vector<Subject>& allSubjects, const std::vector<StudentRecord>& allStudentRecords, int currentYear, int currentTerm);


    //test
    void printSubjectsCount() const;

    void loadTaughtSubjects(const vector<StudentRecord>& studentRecords, const vector<Subject>& allSubjects);

    
};

#endif // STUDENT_H
