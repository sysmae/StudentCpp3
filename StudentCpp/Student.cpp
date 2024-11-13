// Student.cpp
#include "Student.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdexcept>
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
    if (grades.find(subjectID) != grades.end()) {
        grades[subjectID] = gradeVal;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }

}
void Student::setScores(int subjectID, double scoreVal) {
	if (scores.find(subjectID) != scores.end()) {
		scores[subjectID] = scoreVal;
	}
	else {
		cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
	}
}

// ���� ���� ���� �Լ�
void Student::setLetterGrade(int subjectID, const string& letterGradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = letterGradeVal;
        cout << "���� ID " << subjectID << "�� ���� ������ " << letterGradeVal << "(��)�� �����Ǿ����ϴ�.\n"; 
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }
}

void Student::updateLetterGrade(int subjectID, const string& gradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = gradeVal;
    }
    else {
        cerr << "����: ���� ID " << subjectID << "�� �������� �ʽ��ϴ�.\n";
    }
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
void Student::someFunction() {
    // ������ �߰��ϴ� �ڵ� �Ǵ� �ʱ�ȭ �ڵ尡 ���� ���� �� ����
    printSubjectsCount();  // viewSubjects ȣ�� �� Ȯ�ο� ���
    viewSubjects();        // ���� ����� ����ϴ� �Լ� ȣ��
}

//professor.cpp ������ viewSubject
void Student::viewSubjects() const {
   
    cout << "���� ���� ���� :\n";

    // ǥ ��� ��� (�� �ʺ� ����)
    cout << std::left;
    cout << std::setw(10) << "ID"
        << std::setw(20) << "�̸�"
        << std::setw(10) << "����"
        << std::setw(8) << "����"
        << std::setw(10) << "����"
        << std::setw(8) << "�б�"
        << '\n';
    cout << "---------------------------------------------------------------\n";


    bool hasSubjects = false;
    for (const auto& subject : subjects) {

        cout << std::setw(10) << subject.getID()
            << std::setw(20) << subject.getName()
            << std::setw(10) << subject.getType()
            << std::setw(8) << subject.getCredit()

            << '\n';
        hasSubjects = true;
    }
   

    // ����� ������ ���� ��� �޽��� ���
    if (!hasSubjects) {
        cout << "�ش��ϴ� ������ �����ϴ�.\n";
    }
}

//�Ʒ��� ���� �ִ� �ڵ�
/*void Student::viewSubjects() const {
    cout << "\n��ϵ� �����:\n";
   
    for (const auto& subject : subjects) {
        cout << "- " << subject.getName() << " (" << subject.getCredit()
            << " ����, �б�: " << subject.getTerm() << " " << subject.getYear() << ")\n";
    }
}
*/

void Student::viewPreviousSubjects(int year, int term) const {
    // ���� �б� �������� ���� �б⸦ ���
    int previousYear = year;
    int previousTerm = term - 1;

    if (previousTerm == 0) {  // 1�б⿡�� ���� �б�� 2�б��̹Ƿ�
        previousTerm = 2;
        previousYear--;
    }

    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 0;
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length());
    }


    // �Է��� ������ �б⿡ �ش��ϴ� ����鸸 ���
    bool hasSubjects = false;
    for (const auto& subject : subjects) {
        if (subject.getYear() == year && subject.getTerm() == term) {
            cout << std::setw(10) << subject.getID()
                << std::setw(maxNameLength + 2) << subject.getName()
                << std::setw(10) << subject.getType()
                << std::setw(8) << subject.getCredit()
                << std::setw(10) << subject.getYear()
                << std::setw(8) << subject.getTerm()
                << '\n';
            hasSubjects = true;
        }
    }

    // ����� ������ ���� ��� �޽��� ���
    if (!hasSubjects) {
        cout << year << "�� " << term << "�б⿡ �ش��ϴ� ������ �����ϴ�.\n";
    }

    // ���� �б� ���� ���
    cout << "\n������ ����� (" << previousYear << "�� " << previousTerm << "�б�):\n";

    // ǥ ��� ��� (�� �ʺ� ����)
    cout << std::left;
    cout << "ID"
        << std::setw(maxNameLength + 2) << "�̸�"  // ���� �̸��� �ִ� ���̿� ����
        << std::setw(10) << "����"
        << std::setw(8) << "����"
        << std::setw(10) << "����"
        << std::setw(8) << "�б�"
        << '\n';
    cout << string(10 + maxNameLength + 2 + 10 + 8 + 10 + 8, '-') << '\n';  // ���м� ���� ����

    // ���� �б⿡ �ش��ϴ� ����鸸 ���
    hasSubjects = false;
    for (const auto& subject : subjects) {
        if (subject.getYear() == previousYear && subject.getTerm() == previousTerm) {
            cout << std::setw(10) << subject.getID()
                << std::setw(maxNameLength + 2) << subject.getName()
                << std::setw(10) << subject.getType()
                << std::setw(8) << subject.getCredit()
                << std::setw(10) << subject.getYear()
                << std::setw(8) << subject.getTerm()
                << '\n';
            hasSubjects = true;
        }
    }

    // ����� ������ ���� ��� �޽��� ���
    if (!hasSubjects) {
        cout << previousYear << "�� " << previousTerm << "�б⿡ �ش��ϴ� ������ �����ϴ�.\n";
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

//������� ��ȸ
void Student::displayGraduationRequirements() {
    int earnedRequiredMajorCredits = 0;
    int earnedSelectionMajorCredits = 0;
    int earnedMajorBasicsCredits = 0;
    int earnedTotalCredits = 0;

    // Loop through each course the student has taken
    // Assuming the findSubject function is declared and defined properly elsewhere (in utils.cpp)

    //for (const auto& record : STUDENTRECORD_H) {
        // Find the subject using the subject ID in the student record
       /* Subject* subject = findSubject(subjects, record.getSubjectID());  // Correct way to call findSubject

        if (!subject) continue; //������ ã������ ������ �Ѿ��

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
        else if (subject->getType() == "Major Basics") {
            earnedMajorBasicsCredits += subject->getCredit();
        }
    }
     */

    // Calculate total earned credits
    earnedTotalCredits = earnedRequiredMajorCredits + earnedSelectionMajorCredits + earnedMajorBasicsCredits;

    // Display the graduation requirement summary
    std::cout << "================================\n";
    std::cout << "       ������������ ��ȸ�ϱ�:\n";
    std::cout << "================================\n";

    std::cout << std::left << std::setw(25) << "�����ʼ� (45): " << earnedRequiredMajorCredits << "\n";
    std::cout << std::left << std::setw(25) << "�������� (9): " << earnedSelectionMajorCredits << "\n";
    std::cout << std::left << std::setw(25) << "�������� (12): " << earnedMajorBasicsCredits << "\n";
    std::cout << std::left << std::setw(25) << "�����ּ��̼����� (66): " << earnedTotalCredits << "\n";
}


