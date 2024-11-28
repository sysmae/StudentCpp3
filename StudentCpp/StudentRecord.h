#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include <string>

using namespace std;

class StudentRecord {
private:
    int studentID;         // 학생 ID
    int subjectID;         // 과목 ID
    double score;          // 성적 (100점 만점)
    double grade;          // 평점 (4.5 만점)
    string letterGrade;    // 문자 성적 (A+...F)
    string professorID;    // 교수 ID

public:
    // 기본 생성자
    StudentRecord();

    // 전체 생성자
    StudentRecord(int sid, int subid, double sc, double gr, string lg, string pid);

    // Getter 및 Setter 선언 (순서대로)
    int getStudentID() const;
    void setStudentID(int sid);

    int getSubjectID() const;
    void setSubjectID(int subid);

    double getScore() const;
    void setScore(double sc);

    double getGrade() const;
    void setGrade(double gr);

    string getLetterGrade() const;
    void setLetterGrade(const string& lg);

    string getProfessorID() const;
    void setProfessorID(const string& pid);
};

#endif // STUDENTRECORD_H
