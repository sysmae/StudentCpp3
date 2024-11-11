// StudentRecord.cpp
#include "StudentRecord.h"
#include <stdexcept>
#include <string>

// 기본 생성자
StudentRecord::StudentRecord()
    : studentID(0), subjectID(0), score(0.0), letterGrade(""), grade(0.0) {}

// 전체 생성자
StudentRecord::StudentRecord(int sid, int subid, double sc, string lg, double grade)
    : studentID(sid), subjectID(subid), letterGrade(lg){
    setScore(sc);
    setGrade(grade);
}

// 학생 ID 가져오기
int StudentRecord::getStudentID() const { return studentID; }

// 성적 가져오기 (점수)
double StudentRecord::getScore() const { return score; }

// 성적 설정하기 (0.0에서 100.0 사이로 제한)
void StudentRecord::setScore(double sc) {
    if (sc < 0.0 || sc > 100.0) {
        throw std::invalid_argument("Score must be between 0.0 and 100.0");
    }
    score = sc;
}

// 문자 성적 가져오기
string StudentRecord::getLetterGrade() const { return letterGrade; }

// 문자 성적 설정하기
void StudentRecord::setLetterGrade(const string& lg) { letterGrade = lg; }

// 평점 가져오기
double StudentRecord::getGrade() const { return grade; }

// 평점 설정하기
void StudentRecord::setGrade(double g) {
    if (g < 0.0 || g > 4.5) {
        throw std::invalid_argument("Grade must be between 0.0 and 4.5");
    }
    grade = g;
}

// 과목 ID 가져오기
int StudentRecord::getSubjectID() const { return subjectID; }
