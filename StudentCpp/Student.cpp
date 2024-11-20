// Student.cpp
#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdexcept>
#include <map>
#include <vector>
#include <string>

#include "User.h"
#include "Student.h"
#include "StudentRecord.h"
#include "Subject.h"
#include "utils.h"

using namespace std;

// Constructor
Student::Student(string n, string i, string p, string phone, string mail, int sid)
    : User(n, i, p, phone, mail, "Student"), studentID(sid) {}

// Getters
int Student::getStudentID() const { return studentID; }
const vector<Subject>& Student::getSubjects() const { return subjects; }
const map<int, double>& Student::getGrades() const { return grades; }
const map<int, double>& Student::getScores() const { return scores; }
const map<int, string>& Student::getLetterGrades() const { return letterGrades; }

// Setters
void Student::setStudentID(int id) { studentID = id; }
void Student::setSubjects(const vector<Subject>& subs) { subjects = subs; }

string Student::getUserType() const { return "Student"; }

// ���� ���� �Լ�
void Student::setGrades(int subjectID, double gradeVal) {
    // Instead of checking if it exists, directly set or insert the value
    grades[subjectID] = gradeVal;
    //cout << "�л� " << studentID << " ���� ID " << subjectID << "�� ������ " << gradeVal << "(��)�� �����Ǿ����ϴ�.\n";
}

// ���� ���� �Լ�
void Student::setScores(int subjectID, double scoreVal) {
    // Instead of checking if it exists, directly set or insert the value
    scores[subjectID] = scoreVal;
	//cout << "�л� " << studentID << "���� ID " << subjectID << "�� ������ " << scoreVal << "(��)�� �����Ǿ����ϴ�.\n";
}

// ���� ���� ���� �Լ�
void Student::setLetterGrade(int subjectID, const string& letterGradeVal) {
    // Instead of checking if it exists, directly set or insert the value
    letterGrades[subjectID] = letterGradeVal;
    //cout << "�л� " << studentID << "���� ID " << subjectID << "�� ���� ������ " << letterGradeVal << "(��)�� �����Ǿ����ϴ�.\n";
}

void Student::updateLetterGrade(int subjectID, const string& gradeVal) {
        letterGrades[subjectID] = gradeVal;
		//cout << "�л� " << studentID << "���� ID " << subjectID << "�� ���� ������ " << gradeVal << "(��)�� ������Ʈ�Ǿ����ϴ�.\n";
}

void Student::enrollSubject(const Subject& subject) {
    // �л��� �̹� ���� ��ϵǾ� �ִ��� Ȯ��
    for (const auto& sub : subjects) {
        if (sub.getID() == subject.getID()) {
            cout << "����: �̹� " << subject.getName() << " ���� ��ϵǾ� �ֽ��ϴ�.\n";
            return;
        }
    }

    subjects.push_back(subject);
    scores[subject.getID()] = 0.0;           // �ʱ� ������ 0.0���� ����
    grades[subject.getID()] = 0.0;           // �ʱ� ������ 0.0���� ����
    letterGrades[subject.getID()] = "N/A";  // �⺻ ���� ����
    //cout << "���������� " << subject.getName() << " ���� ��ϵǾ����ϴ�.\n"; // ����� ���

}

/*Subject* findSubject(vector<Subject>& subjects, int subjectId)*/
//������ ����� �ִ��� Ȯ��
void Student::printSubjectsCount() const {
    cout << "���� ��ϵ� ���� ��: " << subjects.size() << endl;
}

void Student::loadTaughtSubjects(const vector<StudentRecord>& studentRecords, const vector<Subject>& allSubjects) {
    //cout << "Loading subjects for student ID: " << this->studentID << endl; // Debug output

    for (const auto& subject : allSubjects) {
        //cout << "Checking subject ID: " << subject.getID() << ", Name: " << subject.getName() << endl; // Debug output

        // Loop through student records to find matching student ID
        for (const auto& record : studentRecords) {
            //cout << "Checking student record: Student ID = " << record.getStudentID()
            //    << ", Subject ID = " << record.getSubjectID() << endl; // Debug output

            if (record.getStudentID() == this->studentID && record.getSubjectID() == subject.getID()) {
                // If the subject matches the ID, add it to the student's subjects
                subjects.push_back(subject);
                //cout << "Subject added: " << subject.getName() << " (ID: " << subject.getID() << ")" << endl; // Debug output
                break; // Stop once we've added the subject to avoid duplicates
            }
        }
    }

    //cout << "Total subjects loaded for student ID " << this->studentID << ": " << subjects.size() << endl; // Debug output
}


// ���� ǥ ��� ��� �Լ�
void printTableHeader(size_t nameWidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(nameWidth) << "�̸�"
        << setw(10) << "����"
        << setw(8) << "����"
        << setw(10) << "����"
        << setw(8) << "�б�"
        << '\n';
    cout << string(10 + nameWidth + 10 + 8 + 10 + 8, '-') << '\n';  // ���м� ���� ����
}

void printTableHeaderProfessor(size_t Emailwidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(20) << "�̸�"
        << setw(15) << "��ȭ��ȣ"
        << setw(Emailwidth) << "�̸���"
        << '\n';
    cout << string(10 + 20 + 15 + Emailwidth, '-') << '\n';  // ���м� ���� ����
}

void printTableHeaderStudent(size_t Emailwidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(20) << "�̸�"
        << setw(15) << "��ȭ��ȣ"
        << setw(Emailwidth) << "�̸���"
        << setw(10) << "�й�"
        << '\n';
    cout << string(10 + 20 + 15 + Emailwidth + 10, '-') << '\n';  // ���м� ���� ����
}

// Ư�� ������ �б⿡ �ش��ϴ� ���� ��� �Լ�
bool Student::viewSubjectsByTerm(int year, int term, bool showHeader = true) const {
    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 30; // �⺻ �̸� �� �ʺ� 30���� ����
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // ��� ���
    if (showHeader) {
        printTableHeader(maxNameLength);
    }

    // �Է��� ������ �б⿡ �ش��ϴ� ����鸸 ���
    bool hasSubjects = false;
    for (const auto& subject : subjects) {
        if (subject.getYear() == year && subject.getTerm() == term) {
            cout << setw(10) << subject.getID()
                << setw(maxNameLength) << subject.getName()
                << setw(10) << subject.getType()
                << setw(8) << subject.getCredit()
                << setw(10) << subject.getYear()
                << setw(8) << subject.getTerm()
                << '\n';
            hasSubjects = true;
        }
    }

    // ����� ������ ���� ��� �޽��� ���
    if (!hasSubjects && showHeader) {
        cout << year << "�� " << term << "�б⿡ �ش��ϴ� ������ �����ϴ�.\n";
    }

    return hasSubjects;
}

// ���� �б� ���� ��� �Լ�
void Student::viewCurSubjects(int year, int term) const {
    cout << "���� ���� ���� ���� :\n";
    printTableHeader(30);  // �⺻ �̸� �� �ʺ� 30���� ����
    viewSubjectsByTerm(year, term, false);
}

// ��� ���� �б� ���� ��� �Լ�
void Student::viewAllPreviousSubjects(int year, int term) const {
    cout << "\n���� �б� ������ ��� �����:\n";

    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 30; // �⺻ �̸� �� �ʺ� 30���� ����
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // ǥ ��� ���
    printTableHeader(maxNameLength);

    // ��� ���� �б� ���� ��� (���� �� �б� �������� �ݺ�)
    bool hasSubjects = false;
    for (int y = year; y >= 0; --y) {
        for (int t = (y == year ? term - 1 : 2); t > 0; --t) {
            if (viewSubjectsByTerm(y, t, false)) {
                hasSubjects = true;
            }
        }
    }

    // ����� ������ ���� ��� �޽��� ���
    if (!hasSubjects) {
        cout << "���� �б⿡ �ش��ϴ� ������ �����ϴ�.\n";
    }
}


void Student::viewGrades() const {
    cout << "\n��ġ ����:\n";
    for (const auto& gradeEntry : grades) {
        cout << "- ���� ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}

void Student::viewLetterGrades() const {
    cout << "\n���� ����:\n";
    for (const auto& gradeEntry : letterGrades) {
        cout << "- ���� ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}


// Ư�� ������ ���� ���� ��ȸ
string Student::getLetterGrade(int subjectID) const {
    auto it = letterGrades.find(subjectID);
    if (it != letterGrades.end()) {
        return it->second;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� ���� ������ �������� �ʽ��ϴ�.\n";
        return "N/A"; // �⺻ �� ��ȯ
    }
}

void Student::viewGradeBySubjectID(int subjectID) const {
    // ���� ID�� �����ϴ��� Ȯ��
    bool subjectFound = false;
    bool studentHasScore = false;

    for (const auto& subject : subjects) {
        if (subject.getID() == subjectID) {
            subjectFound = true;

            // ���� ���� ���
            string subjectName = subject.getName();
            double score = 0.0;
            string letterGrade = "N/A";
            double grade = 0.0;

            // �л��� ������ ������ Ȯ��
            if (scores.find(subjectID) != scores.end()) {
                score = scores.at(subjectID);
                studentHasScore = true;
            }
            if (letterGrades.find(subjectID) != letterGrades.end()) {
                letterGrade = letterGrades.at(subjectID);
            }
            if (grades.find(subjectID) != grades.end()) {
                grade = grades.at(subjectID);
            }

            // �л��� ������ �����ϴ� ��츸 ���
            if (studentHasScore) {
                cout << "\n" << subjectName << " (���� ID: " << subjectID << ")�� ����:\n";
                cout << left
                    << setw(12) << "����: " << score << "\n"
                    << setw(12) << "���� ����: " << letterGrade << "\n"
                    << setw(12) << "����: " << grade << "\n";
            }
            else {
                cout << "����: �ش� �л��� ���� ������ �������� �ʽ��ϴ�.\n";
            }

            break;
        }
    }

    // ������ �������� ������ �޽��� ���
    if (!subjectFound) {
        cout << "����: ���� ID " << subjectID << "�� �ش��ϴ� ������ ã�� �� �����ϴ�.\n";
    }
}


// ��ü ���� ��� ���
double Student::calculateAverageScore() const {
    double totalScore = 0.0;
    int count = 0;
    for (const auto& entry : scores) {
        totalScore += entry.second;
        count++;
    }
    if (count == 0) return 0.0; // ������ ������ 0 ��ȯ
    return totalScore / count; // ��� ��ȯ
}



// ������� ��ȸ
void Student::checkGraduationRequirements(const std::vector<Subject>& allSubjects, const std::vector<StudentRecord>& allStudentRecords, int currentYear, int currentTerm) {
    double earnedRequiredMajorCredits = 0.0;
    double earnedSelectionMajorCredits = 0.0;
    double earnedMajorBasicsCredits = 0.0;
    double earnedTotalCredits = 0.0;

    // ������ ���� �ϴ� ����
    const double requiredMajorCreditsRequired = 17.0; // �����ʼ� �ּ� 17����
    const double basicsCreditsRequired = 14.0; // �������� �ּ� 14����
    const double totalCreditsRequired = 66.0; // �����ʼ� + �������� �ּ� 66����

    // Loop through each course the student has taken in all previous terms
    for (const auto& record : allStudentRecords) {
        if (record.getStudentID() == this->getStudentID()) {
            // Find the subject using the subject ID in the student record
            const Subject* subject = nullptr;
            for (const auto& sub : allSubjects) {
                if (sub.getID() == record.getSubjectID()) {
                    subject = &sub;
                    break;
                }
            }

            if (!subject) continue; // ������ ã������ ������ �Ѿ��

            // Check if the subject is from the current semester, skip those subjects
            if (subject->getYear() == currentYear && subject->getTerm() == currentTerm) {
                continue;
            }

            // F�� ���� ������ ������ �ʴ´�
            if (record.getLetterGrade() == "F") {
                continue;
            }

            // ���������� Ÿ�Կ� ����
            if (subject->getType() == "Required") {
                earnedRequiredMajorCredits += subject->getCredit();
            }
            else if (subject->getType() == "Elective") {
                earnedSelectionMajorCredits += subject->getCredit();
            }
            else if (subject->getType() == "Basic") {
                earnedMajorBasicsCredits += subject->getCredit();
            }
        }
    }

    // �� �̼� ���� ���
    double totalEarnedCredits = earnedRequiredMajorCredits + earnedSelectionMajorCredits;

    // ���� ��� ���
    std::cout << "=====================================================================================\n";
    std::cout << "������� ��ȸ:\n";
    std::cout << "-------------------------------------------------------------------------------------\n";

    // ������ ���� �ϴ� ������ ���� ���� ���� ���
    std::cout << std::left << std::setw(25) << "�������� (�ʼ� 14����): " << earnedMajorBasicsCredits << " (���� ���� ����)\n";
    std::cout << std::left << std::setw(25) << "�����ʼ� (�ʼ� 17����): " << earnedRequiredMajorCredits << " (���� ���� ����)\n";
    std::cout << std::left << std::setw(25) << "�������� (�ʼ� 66���� �ջ�): " << earnedSelectionMajorCredits << " (���� ���� ����)\n";
    std::cout << std::left << std::setw(25) << "�����ʼ� + ���� ����: " << totalEarnedCredits << " (���� ���� ����)\n";

    // ���� ��� ���� ���� Ȯ��
    bool graduationMet = true;
    if (earnedMajorBasicsCredits < basicsCreditsRequired) {
        graduationMet = false;
        std::cout << "�������� ���� ����: " << basicsCreditsRequired - earnedMajorBasicsCredits << "���� ����\n";
    }
    if (earnedRequiredMajorCredits < requiredMajorCreditsRequired) {
        graduationMet = false;
        std::cout << "�����ʼ� ���� ����: " << requiredMajorCreditsRequired - earnedRequiredMajorCredits << "���� ����\n";
    }
    if (totalEarnedCredits < totalCreditsRequired) {
        graduationMet = false;
        std::cout << "�����ʼ� + �������� �ջ� ���� ����: " << totalCreditsRequired - totalEarnedCredits << "���� ����\n";
    }

    // ���� ��� ���� ���� ���
    if (graduationMet) {
        std::cout << "���� ����� �����߽��ϴ�.\n";
    }
    else {
        std::cout << "���� ����� �������� �ʾҽ��ϴ�.\n";
    }
}
