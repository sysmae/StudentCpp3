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


//professor.cpp ������ viewSubject
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

// Ư�� ������ �б⿡ �ش��ϴ� ���� ��� �Լ�
void Student::viewSubjectsByTerm(int year, int term, bool showHeader = true) const {
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
    if (!hasSubjects) {
        cout << year << "�� " << term << "�б⿡ �ش��ϴ� ������ �����ϴ�.\n";
    }
}

// ���� �б� ���� ��� �Լ�
void Student::viewCurSubjects(int year, int term) const {
    cout << "���� ���� ���� ���� :\n";
    printTableHeader(30);  // �⺻ �̸� �� �ʺ� 30���� ����
    viewSubjectsByTerm(year, term, false);
}

// ���� �б� ���� ��� �Լ�
void Student::viewPreviousSubjects(int year, int term) const {
    // ���� �б� �������� ���� �б⸦ ���
    int previousYear = year;
    int previousTerm = term - 1;

    if (previousTerm == 0) {  // 1�б⿡�� ���� �б�� 2�б��̹Ƿ�
        previousTerm = 2;
        previousYear--;
    }

    cout << "\n���� �б� ������ ����� (" << previousYear << "�� " << previousTerm << "�б�):\n";
    printTableHeader(30);  // �⺻ �̸� �� �ʺ� 30���� ����
    viewSubjectsByTerm(previousYear, previousTerm, false);
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
    cout << "================================\n";
    cout << "       ������������ ��ȸ�ϱ�:\n";
    cout << "================================\n";

    cout << left << setw(25) << "�����ʼ� (45): " << earnedRequiredMajorCredits << "\n";
    cout << left << setw(25) << "�������� (9): " << earnedSelectionMajorCredits << "\n";
    cout << left << setw(25) << "�������� (12): " << earnedMajorBasicsCredits << "\n";
    cout << left << setw(25) << "�����ּ��̼����� (66): " << earnedTotalCredits << "\n";
}


