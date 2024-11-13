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


//professor.cpp 참고한 viewSubject
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

// 특정 연도와 학기에 해당하는 과목 출력 함수
void Student::viewSubjectsByTerm(int year, int term, bool showHeader = true) const {
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
    if (!hasSubjects) {
        cout << year << "년 " << term << "학기에 해당하는 과목이 없습니다.\n";
    }
}

// 현재 학기 과목 출력 함수
void Student::viewCurSubjects(int year, int term) const {
    cout << "현재 수강 중인 과목 :\n";
    printTableHeader(30);  // 기본 이름 열 너비를 30으로 설정
    viewSubjectsByTerm(year, term, false);
}

// 이전 학기 과목 출력 함수
void Student::viewPreviousSubjects(int year, int term) const {
    // 현재 학기 기준으로 이전 학기를 계산
    int previousYear = year;
    int previousTerm = term - 1;

    if (previousTerm == 0) {  // 1학기에서 이전 학기는 2학기이므로
        previousTerm = 2;
        previousYear--;
    }

    cout << "\n이전 학기 수강한 과목들 (" << previousYear << "년 " << previousTerm << "학기):\n";
    printTableHeader(30);  // 기본 이름 열 너비를 30으로 설정
    viewSubjectsByTerm(previousYear, previousTerm, false);
}

//아래는 원래 있던 코드
/*void Student::viewSubjects() const {
    cout << "\n등록된 과목들:\n";

    for (const auto& subject : subjects) {
        cout << "- " << subject.getName() << " (" << subject.getCredit()
            << " 학점, 학기: " << subject.getTerm() << " " << subject.getYear() << ")\n";
    }
}
*/


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

//졸업요건 조회
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

        if (!subject) continue; //과목이 찾아지지 않으면 넘어가기

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
        else if (subject->getType() == "Major Basics") {
            earnedMajorBasicsCredits += subject->getCredit();
        }
    }
     */

    // Calculate total earned credits
    earnedTotalCredits = earnedRequiredMajorCredits + earnedSelectionMajorCredits + earnedMajorBasicsCredits;

    // Display the graduation requirement summary
    cout << "================================\n";
    cout << "       졸업관련정보 조회하기:\n";
    cout << "================================\n";

    cout << left << setw(25) << "전공필수 (45): " << earnedRequiredMajorCredits << "\n";
    cout << left << setw(25) << "전공선택 (9): " << earnedSelectionMajorCredits << "\n";
    cout << left << setw(25) << "전공기초 (12): " << earnedMajorBasicsCredits << "\n";
    cout << left << setw(25) << "전공최소이수학점 (66): " << earnedTotalCredits << "\n";
}


