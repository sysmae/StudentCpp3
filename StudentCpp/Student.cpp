// Student.cpp
#include "Student.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <stdexcept>

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
        //cout << "과목 ID " << subjectID << "의 문자 성적이 " << letterGradeVal << "(으)로 설정되었습니다.\n"; //디버깅용 출려기
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

void Student::viewSubjects() const {
    cout << "\n등록된 과목들:\n";
    for (const auto& subject : subjects) {
        cout << "- " << subject.getName() << " (" << subject.getCredit()
            << " 학점, 학기: " << subject.getTerm() << " " << subject.getYear() << ")\n";
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
