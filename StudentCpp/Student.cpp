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

// 성적 설정 함수
void Student::setGrades(int subjectID, double gradeVal) {
    // Instead of checking if it exists, directly set or insert the value
    grades[subjectID] = gradeVal;
    //cout << "학생 " << studentID << " 과목 ID " << subjectID << "의 평점이 " << gradeVal << "(으)로 설정되었습니다.\n";
}

// 점수 설정 함수
void Student::setScores(int subjectID, double scoreVal) {
    // Instead of checking if it exists, directly set or insert the value
    scores[subjectID] = scoreVal;
	//cout << "학생 " << studentID << "과목 ID " << subjectID << "의 점수가 " << scoreVal << "(으)로 설정되었습니다.\n";
}

// 문자 성적 설정 함수
void Student::setLetterGrade(int subjectID, const string& letterGradeVal) {
    // Instead of checking if it exists, directly set or insert the value
    letterGrades[subjectID] = letterGradeVal;
    //cout << "학생 " << studentID << "과목 ID " << subjectID << "의 문자 성적이 " << letterGradeVal << "(으)로 설정되었습니다.\n";
}

void Student::updateLetterGrade(int subjectID, const string& gradeVal) {
        letterGrades[subjectID] = gradeVal;
		//cout << "학생 " << studentID << "과목 ID " << subjectID << "의 문자 성적이 " << gradeVal << "(으)로 업데이트되었습니다.\n";
}

void Student::enrollSubject(const Subject& subject) {
    // 학생이 이미 과목에 등록되어 있는지 확인
    for (const auto& sub : subjects) {
        if (sub.getID() == subject.getID()) {
            cout << "오류: 이미 " << subject.getName() << " 과목에 등록되어 있습니다.\n";
            return;
        }
    }

    subjects.push_back(subject);
    scores[subject.getID()] = 0.0;           // 초기 성적을 0.0으로 설정
    grades[subject.getID()] = 0.0;           // 초기 평점을 0.0으로 설정
    letterGrades[subject.getID()] = "N/A";  // 기본 문자 성적
    //cout << "성공적으로 " << subject.getName() << " 과목에 등록되었습니다.\n"; // 디버깅 출력

}

/*Subject* findSubject(vector<Subject>& subjects, int subjectId)*/
//과목이 제대로 있는지 확인
void Student::printSubjectsCount() const {
    cout << "현재 등록된 과목 수: " << subjects.size() << endl;
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


// 공통 표 헤더 출력 함수
void printTableHeader(size_t nameWidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(nameWidth) << "이름"
        << setw(10) << "구분"
        << setw(8) << "학점"
        << setw(10) << "연도"
        << setw(8) << "학기"
        << '\n';
    cout << string(10 + nameWidth + 10 + 8 + 10 + 8, '-') << '\n';  // 구분선 길이 조정
}

void printTableHeaderProfessor(size_t Emailwidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(20) << "이름"
        << setw(15) << "전화번호"
        << setw(Emailwidth) << "이메일"
        << '\n';
    cout << string(10 + 20 + 15 + Emailwidth, '-') << '\n';  // 구분선 길이 조정
}

void printTableHeaderStudent(size_t Emailwidth) {
    cout << left;
    cout << setw(10) << "ID"
        << setw(20) << "이름"
        << setw(15) << "전화번호"
        << setw(Emailwidth) << "이메일"
        << setw(10) << "학번"
        << '\n';
    cout << string(10 + 20 + 15 + Emailwidth + 10, '-') << '\n';  // 구분선 길이 조정
}

// 특정 연도와 학기에 해당하는 과목 출력 함수
bool Student::viewSubjectsByTerm(int year, int term, bool showHeader = true) const {
    // 과목 이름의 최대 길이를 계산하여 열 너비 결정
    size_t maxNameLength = 30; // 기본 이름 열 너비를 30으로 설정
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // 헤더 출력
    if (showHeader) {
        printTableHeader(maxNameLength);
    }

    // 입력한 연도와 학기에 해당하는 과목들만 출력
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

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects && showHeader) {
        cout << year << "년 " << term << "학기에 해당하는 과목이 없습니다.\n";
    }

    return hasSubjects;
}

// 현재 학기 과목 출력 함수
void Student::viewCurSubjects(int year, int term) const {
    cout << "현재 수강 중인 과목 :\n";
    printTableHeader(30);  // 기본 이름 열 너비를 30으로 설정
    viewSubjectsByTerm(year, term, false);
}

// 모든 이전 학기 과목 출력 함수
void Student::viewAllPreviousSubjects(int year, int term) const {
    cout << "\n이전 학기 수강한 모든 과목들:\n";

    // 과목 이름의 최대 길이를 계산하여 열 너비 결정
    size_t maxNameLength = 30; // 기본 이름 열 너비를 30으로 설정
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    // 표 헤더 출력
    printTableHeader(maxNameLength);

    // 모든 이전 학기 과목 출력 (연도 및 학기 역순으로 반복)
    bool hasSubjects = false;
    for (int y = year; y >= 0; --y) {
        for (int t = (y == year ? term - 1 : 2); t > 0; --t) {
            if (viewSubjectsByTerm(y, t, false)) {
                hasSubjects = true;
            }
        }
    }

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects) {
        cout << "이전 학기에 해당하는 과목이 없습니다.\n";
    }
}


void Student::viewGrades() const {
    cout << "\n수치 성적:\n";
    for (const auto& gradeEntry : grades) {
        cout << "- 과목 ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}

void Student::viewLetterGrades() const {
    cout << "\n문자 성적:\n";
    for (const auto& gradeEntry : letterGrades) {
        cout << "- 과목 ID " << gradeEntry.first << ": " << gradeEntry.second << endl;
    }
}


// 특정 과목의 문자 성적 조회
string Student::getLetterGrade(int subjectID) const {
    auto it = letterGrades.find(subjectID);
    if (it != letterGrades.end()) {
        return it->second;
    }
    else {
        cerr << "오류: 과목 ID " << subjectID << "의 문자 성적이 존재하지 않습니다.\n";
        return "N/A"; // 기본 값 반환
    }
}

void Student::viewGradeBySubjectID(int subjectID) const {
    // 과목 ID가 존재하는지 확인
    bool subjectFound = false;
    bool studentHasScore = false;

    for (const auto& subject : subjects) {
        if (subject.getID() == subjectID) {
            subjectFound = true;

            // 과목 정보 출력
            string subjectName = subject.getName();
            double score = 0.0;
            string letterGrade = "N/A";
            double grade = 0.0;

            // 학생의 점수와 성적을 확인
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

            // 학생의 점수가 존재하는 경우만 출력
            if (studentHasScore) {
                cout << "\n" << subjectName << " (과목 ID: " << subjectID << ")의 성적:\n";
                cout << left
                    << setw(12) << "점수: " << score << "\n"
                    << setw(12) << "문자 성적: " << letterGrade << "\n"
                    << setw(12) << "평점: " << grade << "\n";
            }
            else {
                cout << "오류: 해당 학생의 성적 정보가 존재하지 않습니다.\n";
            }

            break;
        }
    }

    // 과목이 존재하지 않으면 메시지 출력
    if (!subjectFound) {
        cout << "오류: 과목 ID " << subjectID << "에 해당하는 과목을 찾을 수 없습니다.\n";
    }
}


// 전체 성적 평균 계산
double Student::calculateAverageScore() const {
    double totalScore = 0.0;
    int count = 0;
    for (const auto& entry : scores) {
        totalScore += entry.second;
        count++;
    }
    if (count == 0) return 0.0; // 과목이 없으면 0 반환
    return totalScore / count; // 평균 반환
}



// 졸업요건 조회
void Student::checkGraduationRequirements(const std::vector<Subject>& allSubjects, const std::vector<StudentRecord>& allStudentRecords, int currentYear, int currentTerm) {
    double earnedRequiredMajorCredits = 0.0;
    double earnedSelectionMajorCredits = 0.0;
    double earnedMajorBasicsCredits = 0.0;
    double earnedTotalCredits = 0.0;

    // 본인이 들어야 하는 학점
    const double requiredMajorCreditsRequired = 17.0; // 전공필수 최소 17학점
    const double basicsCreditsRequired = 14.0; // 전공기초 최소 14학점
    const double totalCreditsRequired = 66.0; // 전공필수 + 전공선택 최소 66학점

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

            if (!subject) continue; // 과목이 찾아지지 않으면 넘어가기

            // Check if the subject is from the current semester, skip those subjects
            if (subject->getYear() == currentYear && subject->getTerm() == currentTerm) {
                continue;
            }

            // F를 받은 학점은 더하지 않는다
            if (record.getLetterGrade() == "F") {
                continue;
            }

            // 수강과목의 타입에 따라
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

    // 총 이수 학점 계산
    double totalEarnedCredits = earnedRequiredMajorCredits + earnedSelectionMajorCredits;

    // 졸업 요건 출력
    std::cout << "=====================================================================================\n";
    std::cout << "졸업요건 조회:\n";
    std::cout << "-------------------------------------------------------------------------------------\n";

    // 본인이 들어야 하는 학점과 실제 들은 학점 출력
    std::cout << std::left << std::setw(25) << "전공기초 (필수 14학점): " << earnedMajorBasicsCredits << " (실제 들은 학점)\n";
    std::cout << std::left << std::setw(25) << "전공필수 (필수 17학점): " << earnedRequiredMajorCredits << " (실제 들은 학점)\n";
    std::cout << std::left << std::setw(25) << "전공선택 (필수 66학점 합산): " << earnedSelectionMajorCredits << " (실제 들은 학점)\n";
    std::cout << std::left << std::setw(25) << "전공필수 + 전공 선택: " << totalEarnedCredits << " (실제 들은 학점)\n";

    // 졸업 요건 충족 여부 확인
    bool graduationMet = true;
    if (earnedMajorBasicsCredits < basicsCreditsRequired) {
        graduationMet = false;
        std::cout << "전공기초 학점 부족: " << basicsCreditsRequired - earnedMajorBasicsCredits << "학점 부족\n";
    }
    if (earnedRequiredMajorCredits < requiredMajorCreditsRequired) {
        graduationMet = false;
        std::cout << "전공필수 학점 부족: " << requiredMajorCreditsRequired - earnedRequiredMajorCredits << "학점 부족\n";
    }
    if (totalEarnedCredits < totalCreditsRequired) {
        graduationMet = false;
        std::cout << "전공필수 + 전공선택 합산 학점 부족: " << totalCreditsRequired - totalEarnedCredits << "학점 부족\n";
    }

    // 졸업 요건 충족 여부 출력
    if (graduationMet) {
        std::cout << "졸업 요건을 충족했습니다.\n";
    }
    else {
        std::cout << "졸업 요건을 충족하지 않았습니다.\n";
    }
}
