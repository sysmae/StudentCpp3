// StudentRecord.h
#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include <string>

using namespace std;

class StudentRecord {
private:
    int studentID;       // �л� ID
    int subjectID;       // ���� ID
    double score;        // ���� (100�� ����)
    string letterGrade;  // ���� ���� (A+...F)
    double grade;        // ���� (4.5 ����)

public:
    // �⺻ ������
    StudentRecord();

    // ��ü ������
    StudentRecord(int sid, int subid, double sc, string lg, double grade);

    // �л� ID ��������
    int getStudentID() const;

    // ���� �������� (����)
    double getScore() const;

    // ���� �����ϱ� (0.0���� 100.0 ���̷� ����)
    void setScore(double sc);

    // ���� ���� ��������
    string getLetterGrade() const;

    // ���� ���� �����ϱ�
    void setLetterGrade(const string& lg);

    // ���� ��������
    double getGrade() const;

    // ���� �����ϱ�
    void setGrade(double g);

    // ���� ID ��������
    int getSubjectID() const;
};

#endif // STUDENTRECORD_H
