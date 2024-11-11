#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include "enums.h"  // enums.h를 포함하여 Term 열거형을 사용합니다.

class Subject {
private:
    int id;                     // 과목 ID
    std::string name;           // 과목 이름
    double credit;              // 학점
    std::string type;           // 과목 유형 (예: "Required", "Elective")
    int year;                   // 년도
    Term term;                  // 학기 (enums.h에서 정의된 Term 사용)
    std::string professorID;    // 교수 ID

public:
    // 전체 초기화 생성자
    Subject(int id, const std::string& n, double c, const std::string& t, int y, Term trm, const std::string& profID = "");

    // 복사 생성자
    Subject(const Subject& other);

    // 접근자
    int getID() const;
    std::string getName() const;
    double getCredit() const;
    std::string getType() const;
    int getYear() const;
    Term getTerm() const;
    std::string getProfessorID() const;

    // 설정자
    void setID(int newID);
    void setName(const std::string& n);
    void setCredit(double c);
    void setType(const std::string& t);
    void setYear(int y);
    void setTerm(Term trm);
    void setProfessorID(const std::string& profID);

    // 맵 비교를 위한 연산자 오버로딩
    bool operator<(const Subject& other) const;
};

#endif // SUBJECT_H
