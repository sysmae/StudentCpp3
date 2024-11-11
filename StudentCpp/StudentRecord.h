// StudentRecord.h
#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include <string>

using namespace std;

class StudentRecord {
private:
    int studentID;       // 학생 ID
    int subjectID;       // 과목 ID
    double score;        // 성적 (100점 만점)
    string letterGrade;  // 문자 성적 (A+...F)
    double grade;        // 평점 (4.5 만점)

public:
    // 기본 생성자
    StudentRecord();

    // 전체 생성자
    StudentRecord(int sid, int subid, double sc, string lg, double grade);

    // 학생 ID 가져오기
    int getStudentID() const;

    // 성적 가져오기 (점수)
    double getScore() const;

    // 성적 설정하기 (0.0에서 100.0 사이로 제한)
    void setScore(double sc);

    // 문자 성적 가져오기
    string getLetterGrade() const;

    // 문자 성적 설정하기
    void setLetterGrade(const string& lg);

    // 평점 가져오기
    double getGrade() const;

    // 평점 설정하기
    void setGrade(double g);

    // 과목 ID 가져오기
    int getSubjectID() const;
};

#endif // STUDENTRECORD_H
