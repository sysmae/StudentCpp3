#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include "enums.h"  // enums.h�� �����Ͽ� Term �������� ����մϴ�.

class Subject {
private:
    int id;                     // ���� ID
    std::string name;           // ���� �̸�
    double credit;              // ����
    std::string type;           // ���� ���� (��: "Required", "Elective")
    int year;                   // �⵵
    Term term;                  // �б� (enums.h���� ���ǵ� Term ���)
    std::string professorID;    // ���� ID

public:
    // ��ü �ʱ�ȭ ������
    Subject(int id, const std::string& n, double c, const std::string& t, int y, Term trm, const std::string& profID = "");

    // ���� ������
    Subject(const Subject& other);

    // ������
    int getID() const;
    std::string getName() const;
    double getCredit() const;
    std::string getType() const;
    int getYear() const;
    Term getTerm() const;
    std::string getProfessorID() const;

    // ������
    void setID(int newID);
    void setName(const std::string& n);
    void setCredit(double c);
    void setType(const std::string& t);
    void setYear(int y);
    void setTerm(Term trm);
    void setProfessorID(const std::string& profID);

    // �� �񱳸� ���� ������ �����ε�
    bool operator<(const Subject& other) const;
};

#endif // SUBJECT_H
