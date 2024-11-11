#include "Subject.h"

// 전체 초기화 생성자
Subject::Subject(int id, const std::string& n, double c, const std::string& t, int y, Term trm, const std::string& profID)
    : id(id), name(n), credit(c), type(t), year(y), term(trm), professorID(profID) {}

// 복사 생성자
Subject::Subject(const Subject& other)
    : id(other.id), name(other.name), credit(other.credit), type(other.type),
    year(other.year), term(other.term), professorID(other.professorID) {}

// 접근자 구현
int Subject::getID() const { return id; }
std::string Subject::getName() const { return name; }
double Subject::getCredit() const { return credit; }
std::string Subject::getType() const { return type; }
int Subject::getYear() const { return year; }
Term Subject::getTerm() const { return term; }
std::string Subject::getProfessorID() const { return professorID; }

// 설정자 구현
void Subject::setID(int newID) { id = newID; }
void Subject::setName(const std::string& n) { name = n; }
void Subject::setCredit(double c) { credit = c; }
void Subject::setType(const std::string& t) { type = t; }
void Subject::setYear(int y) { year = y; }
void Subject::setTerm(Term trm) { term = trm; }
void Subject::setProfessorID(const std::string& profID) { professorID = profID; }

// 맵 비교를 위한 연산자 오버로딩
bool Subject::operator<(const Subject& other) const {
    return id < other.id;  // 고유 ID를 기준으로 비교
}
