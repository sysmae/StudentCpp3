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
    if (grades.find(subjectID) != grades.end()) {
        grades[subjectID] = gradeVal;
    }
    else {
        cerr << "오류: 과목 ID " << subjectID << "가 존재하지 않습니다.\n";
    }

}
void Student::setScores(int subjectID, double scoreVal) {
	if (scores.find(subjectID) != scores.end()) {
		scores[subjectID] = scoreVal;
	}
	else {
		cerr << "오류: 과목 ID " << subjectID << "가 존재하지 않습니다.\n";
	}
}

// 문자 성적 설정 함수
void Student::setLetterGrade(int subjectID, const string& letterGradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = letterGradeVal;
        cout << "과목 ID " << subjectID << "의 문자 성적이 " << letterGradeVal << "(으)로 설정되었습니다.\n"; 
    }
    else {
        cerr << "오류: 과목 ID " << subjectID << "가 존재하지 않습니다.\n";
    }
}

void Student::updateLetterGrade(int subjectID, const string& gradeVal) {
    if (letterGrades.find(subjectID) != letterGrades.end()) {
        letterGrades[subjectID] = gradeVal;
    }
    else {
        cerr << "오류: 과목 ID " << subjectID << "가 존재하지 않습니다.\n";
    }
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
void Student::someFunction() {
    // 과목을 추가하는 코드 또는 초기화 코드가 여기 있을 수 있음
    printSubjectsCount();  // viewSubjects 호출 전 확인용 출력
    viewSubjects();        // 과목 목록을 출력하는 함수 호출
}

//professor.cpp 참고한 viewSubject
void Student::viewSubjects() const {
   
    cout << "수강 중인 과목 :\n";

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


    bool hasSubjects = false;
    for (const auto& subject : subjects) {

        cout << std::setw(10) << subject.getID()
            << std::setw(20) << subject.getName()
            << std::setw(10) << subject.getType()
            << std::setw(8) << subject.getCredit()

            << '\n';
        hasSubjects = true;
    }
   

    // 출력할 과목이 없는 경우 메시지 출력
    if (!hasSubjects) {
        cout << "해당하는 과목이 없습니다.\n";
    }
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

void Student::viewPreviousSubjects(int year, int term) const {
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
    cout << "\n수강한 과목들 (" << previousYear << "년 " << previousTerm << "학기):\n";

    // 표 헤더 출력 (열 너비 설정)
    cout << std::left;
    cout << "ID"
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
    std::cout << "================================\n";
    std::cout << "       졸업관련정보 조회하기:\n";
    std::cout << "================================\n";

    std::cout << std::left << std::setw(25) << "전공필수 (45): " << earnedRequiredMajorCredits << "\n";
    std::cout << std::left << std::setw(25) << "전공선택 (9): " << earnedSelectionMajorCredits << "\n";
    std::cout << std::left << std::setw(25) << "전공기초 (12): " << earnedMajorBasicsCredits << "\n";
    std::cout << std::left << std::setw(25) << "전공최소이수학점 (66): " << earnedTotalCredits << "\n";
}


