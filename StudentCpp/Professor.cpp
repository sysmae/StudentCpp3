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
    cout << left;
    cout << setw(15) << "ID"
        << setw(30) << "�����̸�"  // ���� �̸��� �ִ� ���̿� ����
        << setw(10) << "�й�"
        << setw(15) << "�л��̸�"
        << setw(12) << "����"
        << setw(12) << "���ڼ���"
        << setw(15) << "����"
        << endl;
    cout << string(20 + 20 + 10 + 15 + 12 + 23, '-') << endl;  // ���м� ���� ����

    // �� �л��� ���� ���
    for (const auto& student : enrolledStudents) {
        cout << setw(15) << student->getID()
            << setw(30) << subject->getName()
            << setw(10) << student->getStudentID()
            << setw(15) << student->getName()
            << setw(12) << student->getScores().at(subjectId)
            << setw(10) << student->getLetterGrade(subjectId)
            << setw(10) << student->getGrades().at(subjectId)
            << endl;
    }
}


// ǥ ��� ��� �Լ�
void printSubjectHeader(size_t nameWidth) {
    cout << left;
    cout << setw(15) << "ID"
        << setw(nameWidth) << "�̸�"
        << setw(10) << "����"
        << setw(8) << "����"
        << setw(10) << "����"
        << setw(5) << "�б�" << endl;
    
    cout << string(15 + nameWidth + 10 + 8 + 10 + 5, '-') << '\n';  // ���м� ���� ����
}

// Ư�� ������ �б⿡ �ش��ϴ� ���� ��� �Լ�
bool Professor::printSubjectsByTerm(int year, int term, bool showHeader = true) const {
    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 40;
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // ��� ���
    if (showHeader) {
        printSubjectHeader(maxNameLength);
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

    if (!hasSubjects && showHeader) {
        cout << "�ش� ������ �б⿡ ���� ������ �����ϴ�.\n";
    }

    return hasSubjects;
}

// Professor Ŭ������ ������ viewSubjects �Լ�
void Professor::viewSubjects(int year, int term) const {
    cout << "���� ���� ����� (" << year << "�� " << term << "�б�):\n";
    printSubjectsByTerm(year, term);
}

void Professor::viewAllPreviousSubjects(int year, int term) const {
    cout << "���� �б� �����ߴ� ��� �����:\n";

    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 30;
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // ������� ������ �б⺰�� �׷�ȭ�ϱ� ���� �ڷᱸ��
    map<int, map<int, vector<Subject>>> yearTermSubjects;

    // ������� ������ �б⺰�� �׷�ȭ
    for (const auto& subject : subjects) {
        // ���� �б� ���� ����
        if (subject.getYear() < year || (subject.getYear() == year && subject.getTerm() < term)) {
            yearTermSubjects[subject.getYear()][subject.getTerm()].push_back(subject);
        }
    }

    // ������ �ִ��� Ȯ��
    if (yearTermSubjects.empty()) {
        cout << "���� �б⿡ �ش��ϴ� ������ �����ϴ�.\n";
        return;
    }

    // ǥ ��� ��� (�ѹ���)
    printSubjectHeader(maxNameLength);

    int prevYear = -1;
    int prevTerm = -1;

    // ���� �ֱ� �������� ����ϱ� ���� �������� ��ȸ
    for (auto y_it = yearTermSubjects.rbegin(); y_it != yearTermSubjects.rend(); ++y_it) {
        int y = y_it->first;
        auto& termSubjects = y_it->second;

        // �� ���� ������ �б⸦ �������� ��ȸ (�ֱ� �б����)
        for (auto t_it = termSubjects.rbegin(); t_it != termSubjects.rend(); ++t_it) {
            int t = t_it->first;
            auto& subjectsInTerm = t_it->second;

            // ������ �ٲ�� �ٹٲ� ���
            if (prevYear != -1 && y != prevYear) {
                cout << "\n";
            }


            // ���� ������ �б� ������Ʈ
            prevYear = y;
            prevTerm = t;

            // ���� ���
            for (const auto& subject : subjectsInTerm) {
                cout << setw(15) << subject.getID()
                    << setw(maxNameLength) << subject.getName()
                    << setw(13) << subject.getType()
                    << setw(6) << subject.getCredit()
                    << setw(10) << subject.getYear()
                    << setw(10) << subject.getTerm()
                    << "\n";
            }
        }
    }cout << "\n";
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

// �л� ���� �Է� �Լ�
void Professor::inputStudentScores(vector<Student*>& students, Subject& subject) {
    while (true) {
        cout << "\n������ �Է��� �л��� ID�� �Է��ϼ��� (�����Ϸ��� 0 �Է�): ";
        int studentIDInput;
        cin >> studentIDInput;
        if (studentIDInput == 0) {
            break;
        }

        // �л� ID�� �л� ã��
        auto it = find_if(students.begin(), students.end(),
            [studentIDInput](Student* s) { return s->getStudentID() == studentIDInput; });
        if (it != students.end()) {
            Student* student = *it;
            double score;
            cout << student->getName() << "�� ������ �Է��ϼ��� (0-100): ";
            while (true) {
                cin >> score;
                if (cin.fail() || score < 0.0 || score > 100.0) {
                    cin.clear(); // �Է� ���� �÷��� �ʱ�ȭ
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �߸��� �Է� ����
                    cout << "�߸��� �Է��Դϴ�. 0���� 100 ������ ������ �Է��ϼ���: ";
                } else {
                    break;
                }
            }

            // ���� ����
            student->setScores(subject.getID(), score);
            cout << "������ " << student->getName() << "���� ���������� �ԷµǾ����ϴ�.\n";

            // StudentRecord ������Ʈ
            StudentRecord record(student->getStudentID(), subject.getID(), score, 0.0, "N/A", subject.getProfessorID());
            updateStudentRecordsCSV({ record });
        } else {
            cout << "�л� ID�� ã�� �� �����ϴ�. �ٽ� �Է��ϼ���.\n";
        }
    }
}

void Professor::assignGrades(vector<Student*>& students, Subject& subject) {
    // ���� �� ��� ����
    cout << "\n���� �� ����� �����ϼ���:\n";
    cout << "-------------------------------------------------------------------------------------\n";
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
    cout << "-------------------------------------------------------------------------------------\n";

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
    cout << "\n-----------------------------------------------------------------------------------\n";

    // �Է¹��� ��� ���� ���
    cout << "\n[������ ��� ����]\n";
    for (const auto& grade : gradeThresholds) {
        cout << grade.first << ": ���� " << grade.second << "%\n";
    }
    cout << "F: " << fThreshold << "�� �̸�\n";
    cout << "\n-----------------------------------------------------------------------------------\n";

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

        // StudentRecord ������Ʈ
        StudentRecord record(student->getStudentID(), subject.getID(), score, grade, letterGrade, subject.getProfessorID());
        updateStudentRecordsCSV({ record });
    }

    cout << "������ ���������� �ο��Ǿ����ϴ�.\n";
}