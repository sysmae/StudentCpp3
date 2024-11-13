#include "Professor.h"
#include "Student.h"
#include "utils.h" // Include for updateStudentRecordsCSV function

#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// Constructor
Professor::Professor(string n, string i, string p, string phone, string mail)
    : User(n, i, p, phone, mail, "Professor"), gradingMethod(METHOD_1) {}

// Getters and Setters
vector<Subject> Professor::getSubjects() const { return subjects; }
void Professor::setSubjects(const vector<Subject>& subs) { subjects = subs; }

string Professor::getUserType() const { return "Professor"; }

void Professor::setGradingMethod(GradingMethod method) {
    gradingMethod = method;
}

GradingMethod Professor::getGradingMethod() const {
    return gradingMethod;
}


void  Professor::printStudentGrades(const vector<Student*>& enrolledStudents, const Subject* subject, int subjectId) {
    // ����� ǥ ��� ����
    cout << std::left;
    cout << std::setw(10) << "ID"
        << std::setw(20) << "�����̸�"  // ���� �̸��� �ִ� ���̿� ����
        << std::setw(10) << "�й�"
        << std::setw(15) << "�л��̸�"
        << std::setw(12) << "����"
        << std::setw(12) << "���ڼ���"
        << std::setw(10) << "����"
        << endl;
    cout << string(10 + 20 + 10 + 15 + 12 + 10, '-') << endl;  // ���м� ���� ����

    // �� �л��� ���� ���
    for (const auto& student : enrolledStudents) {
        cout << std::setw(10) << student->getStudentID()
            << std::setw(20) << subject->getName()
            << std::setw(10) << student->getStudentID()
            << std::setw(15) << student->getName()
            << std::setw(12) << student->getScores().at(subjectId)
            << std::setw(10) << student->getLetterGrade(subjectId)
            << std::setw(10) << student->getGrades().at(subjectId)
            << endl;
    }
}


// Professor Ŭ������ ������ viewSubjects �Լ�
void Professor::viewSubjects(int year, int term) const {
    cout << "���� ���� ����� (" << year << "�� " << term << "�б�):\n";

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

    // �Է��� ������ �б⿡ �ش��ϴ� ����鸸 ���
    bool hasSubjects = false;
    for (const auto& subject : subjects) {
        if (subject.getYear() == year && subject.getTerm() == term) {
            cout << std::setw(10) << subject.getID()
                << std::setw(20) << subject.getName()
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
}



// Professor Ŭ������ ���� ��ȸ �Լ�
void Professor::viewPreviousSubjects(int year, int term) const {
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
    cout << "\n�����ߴ� ����� (" << previousYear << "�� " << previousTerm << "�б�):\n";

    // ǥ ��� ��� (�� �ʺ� ����)
    cout << std::left;
    cout << std::setw(10) << "ID"
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

void Professor::loadTeachingSubjects(const vector<Subject>& allSubjects) {
    for (const auto& subject : allSubjects) {

        // Check if the subject's professorID matches this professor's ID
        if (subject.getProfessorID() == getID()) {
            subjects.push_back(subject);
            //cout << "���� �߰���: " << subject.getName() << endl; // ����� ���
        }
    }
}

// ���� �ο� �Լ�
void Professor::inputStudentScores(vector<Student*>& students, Subject& subject) {
    cout << "==========================================================\n";
    cout << "\n������ �ο��� �л��� ID�� �Է��ϼ��� (�����Ϸ��� 0 �Է�):\n\n";
    cout << "==========================================================\n";

    while (true) {
        cout << "�л� ID: ";
        int studentIDInput;
        cin >> studentIDInput;

        if (studentIDInput == 0) {
            break;
        }

        // Find the student by ID
        auto it = find_if(students.begin(), students.end(),
            [studentIDInput](Student* s) { return s->getStudentID() == studentIDInput; });

        if (it != students.end()) {
            Student* student = *it;
            double score;
            cout << student->getName() << "�� ������ �Է��ϼ��� (0-100): ";
            while (true) {
                cin >> score;
                if (cin.fail() || score < 0.0 || score > 100.0) {
                    cin.clear(); // Clear input error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "�߸��� �Է��Դϴ�. 0���� 100 ������ ������ �Է��ϼ���: ";
                }
                else {
                    break;
                }
            }
            student->setScores(subject.getID(), score);
            cout << "������ " << student->getName() << "���� �Ҵ�Ǿ����ϴ�.\n";
            updateStudentRecordsCSV(students); // You can pass the entire list of students here


        }
        else {
            cout << "�л� ID�� ã�� �� �����ϴ�. �ٽ� �Է����ּ���.\n";
        }
        cout << "----------------------------------------------------------\n";
    }
}

void Professor::assignGrades(vector<Student*>& students, Subject& subject) {
    // ���� �� ��� ����
    cout << "==========================================================\n";
    cout << "\n���� �� ����� �����ϼ���:\n";
    cout << "\n==========================================================\n";
    cout << "1. A: ���� 35%, B: ���� 70%\n";
    cout << "2. A: ���� 50%, B: ���� 90%\n\n";

    int methodChoice;
    double maxAGrade, maxBGrade; // A��ް� B����� �ִ� ���� ����

    while (true) {
        cout << "����: ";
        cin >> methodChoice;
        if (methodChoice == 1) {
            setGradingMethod(METHOD_1);
            maxAGrade = 35.0;
            maxBGrade = 70.0;
            cout << "���� �� ����� ��� 1�� �����Ǿ����ϴ�.\n";
            cout << "(A��� ����: 35%, B��� ����: 70%)\n\n";
            break;
        }
        else if (methodChoice == 2) {
            setGradingMethod(METHOD_2);
            maxAGrade = 50.0;
            maxBGrade = 90.0;
            cout << "���� �� ����� ��� 2�� �����Ǿ����ϴ�.\n";
            cout << "(A��� ����: 50%, B��� ����: 90%)\n\n";
            break;
        }
        else {
            cout << "�߸��� �����Դϴ�. �ٽ� �Է����ּ���.\n\n";
        }
    }
    cout << "----------------------------------------------------------\n";

    // ���� ��� �й� �Է�
    vector<pair<string, double>> gradeThresholds;
    cout << "\n���� ��޺� ���� ������ �Է��ϼ��� (0-100):\n";

    // A��� �Է� (A+, A0)
    cout << "\n[A��� �Է� - �� " << maxAGrade << "% ����]\n";
    double currentATotal = 0.0;

    // A+ �Է�
    double aPlus;
    while (true) {
        cout << "A+ ����� ���� ���� ����: ";
        cin >> aPlus;
        if (aPlus <= 0 || aPlus > maxAGrade) {
            cout << "�߸��� �Է��Դϴ�. 0���� ũ�� " << maxAGrade << "���� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        currentATotal = aPlus;
        gradeThresholds.emplace_back("A+", aPlus);
        break;
    }

    // A0 �Է�
    double aZero;
    while (true) {
        cout << "A0 ����� ���� ���� ����: ";
        cin >> aZero;
        if (aZero < currentATotal || aZero > maxAGrade) {
            cout << "�߸��� �Է��Դϴ�. " << currentATotal << "���� ũ�ų� ���� " << maxAGrade << "���� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("A0", aZero);
        break;
    }

    // B��� �Է� (B+, B0)
    cout << "\n[B��� �Է� - �� " << maxBGrade << "% ����]\n";
    double bPlus;
    while (true) {
        cout << "B+ ����� ���� ���� ����: ";
        cin >> bPlus;
        if (bPlus < aZero || bPlus > maxBGrade) {
            cout << "�߸��� �Է��Դϴ�. " << aZero << "���� ũ�ų� ���� " << maxBGrade << "���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("B+", bPlus);
        break;
    }

    // B0 �Է�
    double bZero;
    while (true) {
        cout << "B0 ����� ���� ���� ����: ";
        cin >> bZero;
        if (bZero < bPlus || bZero > maxBGrade) {
            cout << "�߸��� �Է��Դϴ�. " << bPlus << "���� ũ�ų� ���� " << maxBGrade << "���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("B0", bZero);
        break;
    }

    // C��� �Է� (C+, C0)
    cout << "\n[C��� �Է�]\n";
    double cPlus;
    while (true) {
        cout << "C+ ����� ���� ���� ����: ";
        cin >> cPlus;
        if (cPlus < bZero && cPlus > 100) {
            cout << "�߸��� �Է��Դϴ�. " << bZero << "���� ũ�ų� ���� 100���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("C+", cPlus);
        break;
    }

    // C0 �Է�
    double cZero;
    while (true) {
        cout << "C0 ����� ���� ���� ����: ";
        cin >> cZero;
        if (cZero < cPlus && cZero > 100) {
            cout << "�߸��� �Է��Դϴ�. " << cPlus << "���� ũ�ų� ���� 100���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("C0", cZero);
        break;
    }

    // D��� �Է� (D+, D0)
    cout << "\n[D��� �Է�]\n";
    double dPlus;
    while (true) {
        cout << "D+ ����� ���� ���� ����: ";
        cin >> dPlus;
        if (dPlus < cZero || dPlus > 100) {
            cout << "�߸��� �Է��Դϴ�. " << cZero << "���� ũ�ų� ���� 100���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("D+", dPlus);
        break;
    }

    // D0 �Է�
    double dZero;
    while (true) {
        cout << "D0 ����� ���� ���� ����: ";
        cin >> dZero;
        if (dZero < dPlus || dZero > 100) {
            cout << "�߸��� �Է��Դϴ�. " << dPlus << "���� ũ�ų� ���� 100���� �۰ų� ���� ���� �Է��ϼ���.\n";
            continue;
        }
        gradeThresholds.emplace_back("D0", dZero);
        break;
    }

    // F ��� �������� �Է�
    double fThreshold;
    while (true) {
        cout << "\nF ��� ���� ���� (���� ����): ";
        cin >> fThreshold;
        if (fThreshold < 0 || fThreshold > 100) {
            cout << "�߸��� �Է��Դϴ�. 0���� 100 ������ ���� �Է��ϼ���.\n";
            continue;
        }
        break;
    }
    cout << "\n----------------------------------------------------------\n";

    // �Է¹��� ��� ���� ���
    cout << "\n[������ ��� ����]\n";
    for (const auto& grade : gradeThresholds) {
        cout << grade.first << ": ���� " << grade.second << "%\n";
    }
    cout << "F: " << fThreshold << "�� �̸�\n";
    cout << "\n----------------------------------------------------------\n";

    // ������ �ο��� �л��鸸 ����
    vector<pair<Student*, double>> studentScores;
    for (auto& student : students) {
        auto gradeIt = student->getScores().find(subject.getID());
        if (gradeIt != student->getScores().end() && gradeIt->second > 0.0) {
            studentScores.emplace_back(make_pair(student, gradeIt->second));
        }
    }

    if (studentScores.empty()) {
        cout << "������ �ο��� �л��� �����ϴ�.\n";
        return;
    }

    // ������ �ο��� �л��鸸 ����
    for (auto& student : students) {
        auto gradeIt = student->getScores().find(subject.getID());
        if (gradeIt != student->getScores().end() && gradeIt->second > 0.0) {
            studentScores.emplace_back(make_pair(student, gradeIt->second));
        }
    }

    if (studentScores.empty()) {
        cout << "������ �ο��� �л��� �����ϴ�.\n";
        return;
    }

    // ���� �������� ����
    sort(studentScores.begin(), studentScores.end(),
        [](const pair<Student*, double>& a, const pair<Student*, double>& b) {
            return a.second > b.second;
        });

    // ��ü �л� ��
    size_t totalStudents = studentScores.size();

    // ���� �ο�
    for (size_t i = 0; i < studentScores.size(); i++) {
        Student* student = studentScores[i].first;
        double score = studentScores[i].second;

        // ���� �л��� ���� ���� ��� (�����)
        double percentile = (static_cast<double>(i) / totalStudents) * 100.0;

        string letterGrade;
        double grade{};

        // F ��� ���� �� ���� üũ
        if (score < fThreshold) {
            letterGrade = "F";
            grade = 0.0;
        }
        // ��� �� ����
        else if (percentile < gradeThresholds[0].second) {  // A+ ����
            letterGrade = "A+";
            grade = 4.5;
        }
        else if (percentile < gradeThresholds[1].second) {  // A0 ����
            letterGrade = "A";
            grade = 4.0;
        }
        else if (percentile < gradeThresholds[2].second) {  // B+ ����
            letterGrade = "B+";
            grade = 3.5;
        }
        else if (percentile < gradeThresholds[3].second) {  // B0 ����
            letterGrade = "B";
            grade = 3.0;
        }
        else if (percentile < gradeThresholds[4].second) {  // C+ ����
            letterGrade = "C+";
            grade = 2.5;
        }
        else if (percentile < gradeThresholds[5].second) {  // C0 ����
            letterGrade = "C";
            grade = 2.0;
        }
        else if (percentile < gradeThresholds[6].second) {  // D+ ����
            letterGrade = "D+";
            grade = 1.5;
        }
        else if (percentile < gradeThresholds[7].second) {  // D0 ����
            letterGrade = "D";
            grade = 1.0;
        }
        else if(score < fThreshold){  // �������� F
            letterGrade = "F";
            grade = 0.0;
        }

        student->setLetterGrade(subject.getID(), letterGrade);
        student->setGrades(subject.getID(), grade);
    }

    // CSV ���� ������Ʈ�� ���� �л� ��� ���� (�ߺ� ���� ����)
    updateStudentRecordsCSV(students);

    }

  