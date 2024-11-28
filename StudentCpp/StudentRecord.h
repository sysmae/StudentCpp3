#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include <string>

using namespace std;

class StudentRecord {
private:
    int studentID;         // �л� ID
    int subjectID;         // ���� ID
    double score;          // ���� (100�� ����)
    double grade;          // ���� (4.5 ����)
    string letterGrade;    // ���� ���� (A+...F)
    string professorID;    // ���� ID

public:
    // �⺻ ������
    StudentRecord();

    // ��ü ������
    StudentRecord(int sid, int subid, double sc, double gr, string lg, string pid);

    // Getter �� Setter ���� (�������)
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
