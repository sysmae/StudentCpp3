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
    // 출력할 표 헤더 설정
    cout << std::left;
    cout << std::setw(10) << "ID"
        << std::setw(20) << "과목이름"  // 과목 이름의 최대 길이에 맞춤
        << std::setw(10) << "학번"
        << std::setw(15) << "학생이름"
        << std::setw(12) << "점수"
        << std::setw(12) << "문자성적"
        << std::setw(10) << "평점"
        << endl;
    cout << string(10 + 20 + 10 + 15 + 12 + 10, '-') << endl;  // 구분선 길이 조정

    // 각 학생의 성적 출력
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


// Professor 클래스의 개선된 viewSubjects 함수
void Professor::viewSubjects(int year, int term) const {
    cout << "수업 중인 과목들 (" << year << "년 " << term << "학기):\n";

    // 표 헤더 출력 (열 너비 설정)
    cout << std::left;
    cout << std::setw(10) << "ID"
        << std::setw(20) << "이름"
        << std::setw(10) << "구분"
        << std::setw(8) << "학점"
        << std::setw(10) << "연도"
        << std::setw(8) << "학기"
        << '\n';
    cout << "---------------------------------------------------------------\n";

    // 입력한 연도와 학기에 해당하는 과목들만 출력
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

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects) {
        cout << year << "년 " << term << "학기에 해당하는 과목이 없습니다.\n";
    }
}



// Professor 클래스의 과목 조회 함수
void Professor::viewPreviousSubjects(int year, int term) const {
    // 현재 학기 기준으로 이전 학기를 계산
    int previousYear = year;
    int previousTerm = term - 1;

    if (previousTerm == 0) {  // 1학기에서 이전 학기는 2학기이므로
        previousTerm = 2;
        previousYear--;
    }

    // 과목 이름의 최대 길이를 계산하여 열 너비 결정
    size_t maxNameLength = 0;
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length());
    }


    // 입력한 연도와 학기에 해당하는 과목들만 출력
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

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects) {
        cout << year << "년 " << term << "학기에 해당하는 과목이 없습니다.\n";
    }

    // 이전 학기 과목 출력
    cout << "\n수업했던 과목들 (" << previousYear << "년 " << previousTerm << "학기):\n";

    // 표 헤더 출력 (열 너비 설정)
    cout << std::left;
    cout << std::setw(10) << "ID"
        << std::setw(maxNameLength + 2) << "이름"  // 과목 이름의 최대 길이에 맞춤
        << std::setw(10) << "구분"
        << std::setw(8) << "학점"
        << std::setw(10) << "연도"
        << std::setw(8) << "학기"
        << '\n';
    cout << string(10 + maxNameLength + 2 + 10 + 8 + 10 + 8, '-') << '\n';  // 구분선 길이 조정

    // 이전 학기에 해당하는 과목들만 출력
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

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects) {
        cout << previousYear << "년 " << previousTerm << "학기에 해당하는 과목이 없습니다.\n";
    }
}

void Professor::loadTeachingSubjects(const vector<Subject>& allSubjects) {
    for (const auto& subject : allSubjects) {

        // Check if the subject's professorID matches this professor's ID
        if (subject.getProfessorID() == getID()) {
            subjects.push_back(subject);
            //cout << "과목 추가됨: " << subject.getName() << endl; // 디버깅 출력
        }
    }
}

// 성적 부여 함수
void Professor::inputStudentScores(vector<Student*>& students, Subject& subject) {
    cout << "==========================================================\n";
    cout << "\n성적을 부여할 학생의 ID를 입력하세요 (종료하려면 0 입력):\n\n";
    cout << "==========================================================\n";

    while (true) {
        cout << "학생 ID: ";
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
            cout << student->getName() << "의 점수를 입력하세요 (0-100): ";
            while (true) {
                cin >> score;
                if (cin.fail() || score < 0.0 || score > 100.0) {
                    cin.clear(); // Clear input error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "잘못된 입력입니다. 0에서 100 사이의 점수를 입력하세요: ";
                }
                else {
                    break;
                }
            }
            student->setScores(subject.getID(), score);
            cout << "점수가 " << student->getName() << "에게 할당되었습니다.\n";
            updateStudentRecordsCSV(students); // You can pass the entire list of students here


        }
        else {
            cout << "학생 ID를 찾을 수 없습니다. 다시 입력해주세요.\n";
        }
        cout << "----------------------------------------------------------\n";
    }
}

void Professor::assignGrades(vector<Student*>& students, Subject& subject) {
    // 성적 평가 방식 선택
    cout << "==========================================================\n";
    cout << "\n성적 평가 방식을 선택하세요:\n";
    cout << "\n==========================================================\n";
    cout << "1. A: 상위 35%, B: 상위 70%\n";
    cout << "2. A: 상위 50%, B: 상위 90%\n\n";

    int methodChoice;
    double maxAGrade, maxBGrade; // A등급과 B등급의 최대 비율 저장

    while (true) {
        cout << "선택: ";
        cin >> methodChoice;
        if (methodChoice == 1) {
            setGradingMethod(METHOD_1);
            maxAGrade = 35.0;
            maxBGrade = 70.0;
            cout << "성적 평가 방식이 방법 1로 설정되었습니다.\n";
            cout << "(A등급 상한: 35%, B등급 상한: 70%)\n\n";
            break;
        }
        else if (methodChoice == 2) {
            setGradingMethod(METHOD_2);
            maxAGrade = 50.0;
            maxBGrade = 90.0;
            cout << "성적 평가 방식이 방법 2로 설정되었습니다.\n";
            cout << "(A등급 상한: 50%, B등급 상한: 90%)\n\n";
            break;
        }
        else {
            cout << "잘못된 선택입니다. 다시 입력해주세요.\n\n";
        }
    }
    cout << "----------------------------------------------------------\n";

    // 세부 등급 분배 입력
    vector<pair<string, double>> gradeThresholds;
    cout << "\n세부 등급별 상위 비율을 입력하세요 (0-100):\n";

    // A등급 입력 (A+, A0)
    cout << "\n[A등급 입력 - 총 " << maxAGrade << "% 이하]\n";
    double currentATotal = 0.0;

    // A+ 입력
    double aPlus;
    while (true) {
        cout << "A+ 등급의 상위 기준 비율: ";
        cin >> aPlus;
        if (aPlus <= 0 || aPlus > maxAGrade) {
            cout << "잘못된 입력입니다. 0보다 크고 " << maxAGrade << "보다 작은 값을 입력하세요.\n";
            continue;
        }
        currentATotal = aPlus;
        gradeThresholds.emplace_back("A+", aPlus);
        break;
    }

    // A0 입력
    double aZero;
    while (true) {
        cout << "A0 등급의 상위 기준 비율: ";
        cin >> aZero;
        if (aZero < currentATotal || aZero > maxAGrade) {
            cout << "잘못된 입력입니다. " << currentATotal << "보다 크거나 같고 " << maxAGrade << "보다 작은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("A0", aZero);
        break;
    }

    // B등급 입력 (B+, B0)
    cout << "\n[B등급 입력 - 총 " << maxBGrade << "% 이하]\n";
    double bPlus;
    while (true) {
        cout << "B+ 등급의 상위 기준 비율: ";
        cin >> bPlus;
        if (bPlus < aZero || bPlus > maxBGrade) {
            cout << "잘못된 입력입니다. " << aZero << "보다 크거나 같고 " << maxBGrade << "보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("B+", bPlus);
        break;
    }

    // B0 입력
    double bZero;
    while (true) {
        cout << "B0 등급의 상위 기준 비율: ";
        cin >> bZero;
        if (bZero < bPlus || bZero > maxBGrade) {
            cout << "잘못된 입력입니다. " << bPlus << "보다 크거나 같고 " << maxBGrade << "보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("B0", bZero);
        break;
    }

    // C등급 입력 (C+, C0)
    cout << "\n[C등급 입력]\n";
    double cPlus;
    while (true) {
        cout << "C+ 등급의 상위 기준 비율: ";
        cin >> cPlus;
        if (cPlus < bZero && cPlus > 100) {
            cout << "잘못된 입력입니다. " << bZero << "보다 크거나 같고 100보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("C+", cPlus);
        break;
    }

    // C0 입력
    double cZero;
    while (true) {
        cout << "C0 등급의 상위 기준 비율: ";
        cin >> cZero;
        if (cZero < cPlus && cZero > 100) {
            cout << "잘못된 입력입니다. " << cPlus << "보다 크거나 같고 100보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("C0", cZero);
        break;
    }

    // D등급 입력 (D+, D0)
    cout << "\n[D등급 입력]\n";
    double dPlus;
    while (true) {
        cout << "D+ 등급의 상위 기준 비율: ";
        cin >> dPlus;
        if (dPlus < cZero || dPlus > 100) {
            cout << "잘못된 입력입니다. " << cZero << "보다 크거나 같고 100보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("D+", dPlus);
        break;
    }

    // D0 입력
    double dZero;
    while (true) {
        cout << "D0 등급의 상위 기준 비율: ";
        cin >> dZero;
        if (dZero < dPlus || dZero > 100) {
            cout << "잘못된 입력입니다. " << dPlus << "보다 크거나 같고 100보다 작거나 같은 값을 입력하세요.\n";
            continue;
        }
        gradeThresholds.emplace_back("D0", dZero);
        break;
    }

    // F 등급 기준점수 입력
    double fThreshold;
    while (true) {
        cout << "\nF 등급 기준 점수 (절대 점수): ";
        cin >> fThreshold;
        if (fThreshold < 0 || fThreshold > 100) {
            cout << "잘못된 입력입니다. 0에서 100 사이의 값을 입력하세요.\n";
            continue;
        }
        break;
    }
    cout << "\n----------------------------------------------------------\n";

    // 입력받은 등급 분포 출력
    cout << "\n[설정된 등급 분포]\n";
    for (const auto& grade : gradeThresholds) {
        cout << grade.first << ": 상위 " << grade.second << "%\n";
    }
    cout << "F: " << fThreshold << "점 미만\n";
    cout << "\n----------------------------------------------------------\n";

    // 점수가 부여된 학생들만 추출
    vector<pair<Student*, double>> studentScores;
    for (auto& student : students) {
        auto gradeIt = student->getScores().find(subject.getID());
        if (gradeIt != student->getScores().end() && gradeIt->second > 0.0) {
            studentScores.emplace_back(make_pair(student, gradeIt->second));
        }
    }

    if (studentScores.empty()) {
        cout << "성적이 부여된 학생이 없습니다.\n";
        return;
    }

    // 점수가 부여된 학생들만 추출
    for (auto& student : students) {
        auto gradeIt = student->getScores().find(subject.getID());
        if (gradeIt != student->getScores().end() && gradeIt->second > 0.0) {
            studentScores.emplace_back(make_pair(student, gradeIt->second));
        }
    }

    if (studentScores.empty()) {
        cout << "성적이 부여된 학생이 없습니다.\n";
        return;
    }

    // 점수 내림차순 정렬
    sort(studentScores.begin(), studentScores.end(),
        [](const pair<Student*, double>& a, const pair<Student*, double>& b) {
            return a.second > b.second;
        });

    // 전체 학생 수
    size_t totalStudents = studentScores.size();

    // 성적 부여
    for (size_t i = 0; i < studentScores.size(); i++) {
        Student* student = studentScores[i].first;
        double score = studentScores[i].second;

        // 현재 학생의 상위 비율 계산 (백분율)
        double percentile = (static_cast<double>(i) / totalStudents) * 100.0;

        string letterGrade;
        double grade{};

        // F 등급 절대 평가 먼저 체크
        if (score < fThreshold) {
            letterGrade = "F";
            grade = 0.0;
        }
        // 상대 평가 적용
        else if (percentile < gradeThresholds[0].second) {  // A+ 범위
            letterGrade = "A+";
            grade = 4.5;
        }
        else if (percentile < gradeThresholds[1].second) {  // A0 범위
            letterGrade = "A";
            grade = 4.0;
        }
        else if (percentile < gradeThresholds[2].second) {  // B+ 범위
            letterGrade = "B+";
            grade = 3.5;
        }
        else if (percentile < gradeThresholds[3].second) {  // B0 범위
            letterGrade = "B";
            grade = 3.0;
        }
        else if (percentile < gradeThresholds[4].second) {  // C+ 범위
            letterGrade = "C+";
            grade = 2.5;
        }
        else if (percentile < gradeThresholds[5].second) {  // C0 범위
            letterGrade = "C";
            grade = 2.0;
        }
        else if (percentile < gradeThresholds[6].second) {  // D+ 범위
            letterGrade = "D+";
            grade = 1.5;
        }
        else if (percentile < gradeThresholds[7].second) {  // D0 범위
            letterGrade = "D";
            grade = 1.0;
        }
        else if(score < fThreshold){  // 나머지는 F
            letterGrade = "F";
            grade = 0.0;
        }

        student->setLetterGrade(subject.getID(), letterGrade);
        student->setGrades(subject.getID(), grade);
    }

    // CSV 파일 업데이트를 위한 학생 목록 생성 (중복 선언 제거)
    updateStudentRecordsCSV(students);

    }

  