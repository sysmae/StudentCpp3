// Administrator.cpp
#include "Administrator.h"
#include "Student.h"      // Student 클래스 포함
#include "Professor.h"    // Professor 클래스 포함
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "utils.h"
#include <regex>

using namespace std;

// Constructor
Administrator::Administrator(string n, string i, string p, string phone, string mail)
    : User(n, i, p, phone, mail, "Administrator") {}

// Get User Type
string Administrator::getUserType() const { return "Administrator"; }

// Add User
void Administrator::addUser(vector<unique_ptr<User>>& users, unique_ptr<User> newUser) {
    // 중복 ID 확인
    auto it = find_if(users.begin(), users.end(),
        [&newUser](const unique_ptr<User>& user) { return user->getID() == newUser->getID(); });

    if (it != users.end()) {
        cout << "오류: 이미 존재하는 ID입니다. 사용자를 추가할 수 없습니다.\n";
        return;
    }

    cout << "새로운 사용자가 추가되었습니다: " << newUser->getName() << endl;
    users.push_back(move(newUser));
}

// Remove User
void Administrator::removeUser(vector<unique_ptr<User>>& users, const string& userID) {
    auto it = remove_if(users.begin(), users.end(),
        [&userID](const unique_ptr<User>& user) { return user->getID() == userID; });

    if (it != users.end()) {
        cout << "사용자가 성공적으로 제거되었습니다: " << (*it)->getName() << endl;
        users.erase(it, users.end());
    }
    else {
        cout << "사용자를 찾을 수 없습니다.\n";
    }
}

// View All Users
void Administrator::viewAllUsers(const vector<unique_ptr<User>>& users) const {
    cout << "=== 모든 사용자 목록 ===\n";
    for (const auto& user : users) {
        cout << "ID: " << user->getID()
            << ", 이름: " << user->getName()
            << ", 유형: " << user->getUserType() << endl;
    }
}

void Administrator::addSubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    while (true) { // 전체 과목 추가 과정을 반복
        int id;
        string name, type, termStr, professorID;
        double credit;
        int year;

        viewSubjects(subjects);
        cout << "-------------------------------------------------------------------------------------\n";

        // 과목 ID 입력
        while (true) {
            cout << "과목 ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                id = stoi(input);
                // 중복 ID 확인
                auto it = find_if(subjects.begin(), subjects.end(),
                    [id](const Subject& s) { return s.getID() == id; });
                if (it != subjects.end()) {
                    cout << "오류: 이미 존재하는 과목 ID입니다. 다른 ID를 입력하세요.\n";
                }
                else {
                    break;
                }
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 과목 이름 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "과목 이름을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): \n";
            getline(cin, name);
            if (name == "back") return;
            if (!name.empty() && isValidName(name)) break;
            cout << "과목 이름은 비워둘 수 없으며, 숫자가 포함될 수 없습니다. 다시 입력하세요.\n";
        }

        // 과목 유형 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "과목 유형을 입력하세요 (Required/Elective/Basic) ('back'을 입력해 이전 메뉴로 돌아가기): \n";
            getline(cin, type);
            if (type == "back") return;
            if (type == "Required" || type == "Elective" || type == "Basic") break;
            cout << "잘못된 유형입니다. 다시 입력하세요.\n";
        }

        // 학점 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "학점을 입력하세요 (1 OR 2 OR 3)('back'을 입력해 이전 메뉴로 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                credit = stod(input);
                if (credit == 1 || credit == 2 || credit == 3) break;
                cout << "유효하지 않은 학점입니다. 1, 2 혹은 3 학점 중 하나를 입력하세요.\n";
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 숫자를 입력하세요.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 년도 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "년도를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                year = stoi(input);
                if (year > 0) break;
                cout << "유효하지 않은 연도입니다. 양의 정수를 입력하세요.\n";
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 정수를 입력하세요.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 학기 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "학기를 입력하세요 (FIRST_TERM/SECOND_TERM) ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, termStr);
            if (termStr == "back") return;
            if (termStr == "FIRST_TERM" || termStr == "SECOND_TERM") break;
            cout << "잘못된 학기입니다. 다시 입력하세요.\n";
        }

        // 담당 교수 ID 입력 및 유효성 검사
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";

            // 교수 목록 표시
            viewProfessors(users);

            cout << "담당 교수 ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, professorID);
            if (professorID == "back") return;

            // 교수 ID가 존재하고, 유형이 Professor인지 확인
            bool validProfessor = false;
            for (const auto& user : users) {
                if (user->getUserType() == "Professor" && user->getID() == professorID) {
                    validProfessor = true;
                    break;
                }
            }

            if (validProfessor) {
                break;
            }
            else {
                cout << "유효하지 않은 교수 ID입니다. 존재하는 교수의 ID를 입력하세요.\n";
            }
        }

        // 학기 타입 변환
        Term term = (termStr == "FIRST_TERM") ? Term::FIRST_TERM : Term::SECOND_TERM;

        // 과목 생성 및 추가
        subjects.emplace_back(Subject(id, name, credit, type, year, term, professorID));
        cout << "\"" << name << "\" 과목이 생성되었습니다.\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewSubjects(subjects);

        // 추가 성공 후 루프 종료
        break;
    }
}


void Administrator::deleteSubject(vector<Subject>& subjects) {
    int id;
    viewSubjects(subjects);

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "삭제할 과목 ID를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: \n";
        }
    }
    cin.ignore();

    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "\"" << it->getName() << "\" 과목이 삭제되었습니다.\n";
        subjects.erase(it);
        cout << "-------------------------------------------------------------------------------------\n";
        viewSubjects(subjects);
    }
    else {
        cout << "과목을 찾을 수 없습니다.\n";
    }
}

// Modify Subject
void Administrator::modifySubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    while (true) { // 전체 과목 수정 과정을 반복
        viewSubjects(subjects);
        cout << "-------------------------------------------------------------------------------------\n";
        int id;

        // 과목 ID 입력 및 유효성 검사 루프
        while (true) {
            cout << "수정할 과목 ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            string input;
            cin >> input;

            // 'back' 입력 시 함수 종료
            string lowerInput = input;
            // 소문자로 변환
            for (auto& c : lowerInput) c = tolower(c);
            if (lowerInput == "back") return;

            try {
                id = stoi(input);
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요.\n";
                continue; // 다시 입력 요청
            }

            // 입력한 ID가 존재하는지 확인
            auto it = find_if(subjects.begin(), subjects.end(),
                [id](Subject& s) { return s.getID() == id; });

            if (it != subjects.end()) {
                // 과목이 존재할 경우 수정 프로세스로 이동
                // 수정할 과목 정보 출력
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "현재 과목 정보:\n";
                cout << "ID: " << it->getID() << "\n";
                cout << "이름: " << it->getName() << "\n";
                cout << "유형: " << it->getType() << "\n";
                cout << "학점: " << it->getCredit() << "\n";
                cout << "년도: " << it->getYear() << "\n";
                cout << "학기: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "\n";
                cout << "담당 교수 ID: " << it->getProfessorID() << "\n";
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "///////'pass'를 입력하면 변경하지 않음///////" << endl;
                cout << "-------------------------------------------------------------------------------------\n";

                // 과목 이름 수정
                string newName;
                while (true) {
                    cout << "새 과목 이름을 입력하세요 (현재: " << it->getName() << "): ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 이전 cin >> input에서 남은 개행 문자 제거
                    getline(cin, newName);

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = newName.find_first_not_of(" \t\n\r");
                    size_t end = newName.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newName = newName.substr(start, end - start + 1);
                    else
                        newName = "";

                    // 소문자 변환
                    string lowerNewName = newName;
                    for (auto& c : lowerNewName) c = tolower(c);

                    if (lowerNewName == "back") return;
                    if (lowerNewName == "pass") {
                        newName = it->getName(); // 변경하지 않음
                        break;
                    }
                    if (!newName.empty() && isValidName(newName)) break;
                    cout << "과목 이름은 비워둘 수 없으며, 숫자가 포함될 수 없습니다. 다시 입력하세요.\n";
                }

                // 과목 유형 수정
                string newType;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "새 과목 유형을 입력하세요 (현재: " << it->getType() << ", Required/Elective/Basic 입력): ";
                    getline(cin, newType);

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = newType.find_first_not_of(" \t\n\r");
                    size_t end = newType.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newType = newType.substr(start, end - start + 1);
                    else
                        newType = "";

                    // 소문자 변환
                    string lowerNewType = newType;
                    for (auto& c : lowerNewType) c = tolower(c);

                    if (lowerNewType == "back") return;
                    if (lowerNewType == "pass") {
                        newType = it->getType(); // 변경하지 않음
                        break;
                    }
                    if (newType == "Required" || newType == "Elective" || newType == "Basic") break;
                    cout << "잘못된 유형입니다. 다시 입력하세요.\n";
                }

                // 학점 수정
                double newCredit;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "새 학점을 입력하세요 (현재: " << it->getCredit() << "학점,1 OR 2 OR 3 중 하나를 입력): ";
                    string creditInput;
                    cin >> creditInput;

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = creditInput.find_first_not_of(" \t\n\r");
                    size_t end = creditInput.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        creditInput = creditInput.substr(start, end - start + 1);
                    else
                        creditInput = "";

                    // 소문자 변환
                    string lowerCreditInput = creditInput;
                    for (auto& c : lowerCreditInput) c = tolower(c);

                    if (lowerCreditInput == "back") return;
                    if (lowerCreditInput == "pass") {
                        newCredit = it->getCredit(); // 변경하지 않음
                        break;
                    }
                    try {
                        newCredit = stod(creditInput);
                        if (newCredit == 1 || newCredit == 2 || newCredit == 3) break;
                        cout << "유효하지 않은 학점입니다. 1, 2 혹은 3학점 중 하나를 입력하세요.\n";
                    }
                    catch (...) {
                        cout << "유효하지 않은 입력입니다. 숫자를 입력하세요.\n";
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 입력 버퍼 정리

                // 년도 수정
                int newYear;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "새 년도를 입력하세요 (현재: " << it->getYear() << "): ";
                    string yearInput;
                    cin >> yearInput;

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = yearInput.find_first_not_of(" \t\n\r");
                    size_t end = yearInput.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        yearInput = yearInput.substr(start, end - start + 1);
                    else
                        yearInput = "";

                    // 소문자 변환
                    string lowerYearInput = yearInput;
                    for (auto& c : lowerYearInput) c = tolower(c);

                    if (lowerYearInput == "back") return;
                    if (lowerYearInput == "pass") {
                        newYear = it->getYear(); // 변경하지 않음
                        break;
                    }
                    try {
                        newYear = stoi(yearInput);
                        if (newYear > 0) break;
                        cout << "유효하지 않은 연도입니다. 양의 정수를 입력하세요.\n";
                    }
                    catch (...) {
                        cout << "유효하지 않은 입력입니다. 정수를 입력하세요.\n";
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 입력 버퍼 정리

                // 학기 수정
                string newTermStr;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "학기를 입력하세요 (현재: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << ", FIRST_TERM/SECOND_TERM 입력): ";
                    getline(cin, newTermStr);

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = newTermStr.find_first_not_of(" \t\n\r");
                    size_t end = newTermStr.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newTermStr = newTermStr.substr(start, end - start + 1);
                    else
                        newTermStr = "";

                    // 소문자 변환
                    string lowerNewTermStr = newTermStr;
                    for (auto& c : lowerNewTermStr) c = tolower(c);

                    if (lowerNewTermStr == "back") return;
                    if (lowerNewTermStr == "pass") {
                        newTermStr = (it->getTerm() == Term::FIRST_TERM) ? "FIRST_TERM" : "SECOND_TERM"; // 변경하지 않음
                        break;
                    }
                    if (newTermStr == "FIRST_TERM" || newTermStr == "SECOND_TERM") break;
                    cout << "잘못된 학기입니다. 다시 입력하세요.\n";
                }

                // 담당 교수 ID 입력 및 유효성 검사
                string newProfessorID;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";

                    // 교수 목록 표시
                    viewProfessors(users);

                    cout << "담당 교수 ID를 입력하세요: ";
                    getline(cin, newProfessorID);

                    // 입력값 전처리: 앞뒤 공백 제거
                    size_t start = newProfessorID.find_first_not_of(" \t\n\r");
                    size_t end = newProfessorID.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newProfessorID = newProfessorID.substr(start, end - start + 1);
                    else
                        newProfessorID = "";

                    // 소문자 변환
                    string lowerProfessorID = newProfessorID;
                    for (auto& c : lowerProfessorID) c = tolower(c);

                    if (lowerProfessorID == "back") return;
                    if (lowerProfessorID == "pass") {
                        newProfessorID = it->getProfessorID(); // 변경하지 않음
                        break;
                    }

                    // 교수 ID가 존재하고, 유형이 Professor인지 확인
                    bool validProfessor = false;
                    for (const auto& user : users) {
                        if (user->getUserType() == "Professor" && user->getID() == newProfessorID) {
                            validProfessor = true;
                            break;
                        }
                    }

                    if (validProfessor) {
                        break;
                    }
                    else {
                        cout << "유효하지 않은 교수 ID입니다. 존재하는 교수의 ID를 입력하세요.\n";
                    }
                }

                // 학기 타입 변환
                Term newTerm;
                if (newTermStr == "FIRST_TERM") {
                    newTerm = Term::FIRST_TERM;
                }
                else { // "SECOND_TERM" 일 경우
                    newTerm = Term::SECOND_TERM;
                }

                // 과목 정보 수정
                it->setName(newName);
                it->setType(newType);
                it->setCredit(newCredit);
                it->setYear(newYear);
                it->setTerm(newTerm);
                it->setProfessorID(newProfessorID);

                cout << "해당 과목이 수정되었습니다.\n";
                cout << "-------------------------------------------------------------------------------------\n";
                viewSubjects(subjects);

                return; // 수정 완료 후 함수 종료
            }
            else {
                cout << "과목을 찾을 수 없습니다.\n";
                // 다시 과목 ID를 입력받기 위해 루프를 계속합니다.
            }
        }
    }
}


void Administrator::viewSubjects(const vector<Subject>& subjects) const {
    cout << "현재 등록된 과목 목록: \n";
    // 과목 이름의 최대 길이를 계산하여 열 너비 결정
    size_t maxNameLength = 30; // 기본 이름 열 너비를 15으로 설정
    for (const auto& subject : subjects) {
        maxNameLength = max(maxNameLength, subject.getName().length() + 2);
    }

    printTableHeader(maxNameLength);

    bool hasSubjects = false;
    for (const auto& subject : subjects) {
        cout << setw(10) << subject.getID()
            << setw(maxNameLength) << subject.getName()
            << setw(10) << subject.getType()
            << setw(8) << subject.getCredit()
            << setw(10) << subject.getYear()
            << setw(8) << subject.getTerm()
            << '\n';
    }
}
   
// Professor Management
void Administrator::viewProfessors(const vector<unique_ptr<User>>& users) {
    cout << "교수 목록: \n";
    size_t maxNameLength = 15; // 기본 이름 열 너비를 15으로 설정
    for (const auto& user : users) {
        maxNameLength = max(maxNameLength, user->getName().length() + 2);
    }

    printTableHeaderProfessor(maxNameLength);

    
    for (const auto& user : users) {
        if (user->getUserType() == "Professor") {
            cout << setw(10) << user->getID()
                << setw(maxNameLength) << user->getName()
                << setw(15) << user->getPhoneNumber()
                << setw(20) << user->getEmail()
                << '\n';
        }
    }
}
void Administrator::viewProfessorInfo(const vector<unique_ptr<User>>& users) {
    string profID;
    viewProfessors(users);
    cout << "-------------------------------------------------------------------------------------\n";
    cout << "교수 ID을 입력하세요: ";
    cin >> profID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "=== 교수 정보 ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "이름: " << user->getName() << "\n";
            cout << "전화번호: " << user->getPhoneNumber() << "\n";
            cout << "이메일: " << user->getEmail() << "\n";
            cout << "-------------------------------------------------------------------------------------\n\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "해당 ID의 교수를 찾을 수 없습니다.\n";
    }
}
void Administrator::addProfessor(vector<unique_ptr<User>>& users) {
    while (true) { // 교수 추가 과정을 반복
        string id, name, phone, email;

        viewProfessors(users);
        cout << "-------------------------------------------------------------------------------------\n";

        // ID 입력
        while (true) {
            cout << "ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            cin >> id;
            if (id == "back") return; // 이전 메뉴로 돌아가기

            // ID 중복 체크
            bool idExists = false;
            for (const auto& user : users) {
                if (user->getID() == id) {
                    cout << "오류: 이미 존재하는 ID입니다. 다른 ID를 입력하세요.\n";
                    idExists = true;
                    break;
                }
            }
            if (!idExists) break; // 중복되지 않는 ID라면 루프 종료
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 이름 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "이름을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, name);
            if (name == "back") return; // 이전 메뉴로 돌아가기
            if (!name.empty() && isValidName(name)) break;
            cout << "이름은 비워둘 수 없으며, 숫자가 포함될 수 없습니다. 다시 입력하세요: \n";
        }

        // 전화번호 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "전화번호를 입력하세요 [010-xxxx-xxxx 형식] ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, phone);
            if (phone == "back") return; // 이전 메뉴로 돌아가기

            // 전화번호 형식 체크
            regex phonePattern("^010-\\d{4}-\\d{4}$");
            if (!regex_match(phone, phonePattern)) {
                cout << "유효하지 않은 형식입니다. [010-xxxx-xxxx] 형식으로 다시 입력하세요.\n";
                continue;
            }

            // 전화번호 중복 체크
            bool phoneExists = false;
            for (const auto& user : users) {
                if (user->getPhoneNumber() == phone) {
                    cout << "오류: 이미 존재하는 전화번호입니다. 다른 전화번호를 입력하세요.\n";
                    phoneExists = true;
                    break;
                }
            }
            if (!phoneExists) break; // 중복되지 않는 전화번호라면 루프 종료
        }

        // 이메일 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "이메일을 입력하세요 [예: example@cau.ac.kr] ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, email);
            if (email == "back") return; // 이전 메뉴로 돌아가기

            // 이메일 형식 체크
            regex emailPattern("^[a-zA-Z0-9._%+-]+@cau\\.ac\\.kr$");
            if (regex_match(email, emailPattern)) {
                break; // 올바른 이메일 형식이라면 루프 종료
            }
            else {
                cout << "유효하지 않은 이메일 형식입니다. [예: example@cau.ac.kr] 다시 입력하세요.\n";
            }
        }

        /////////////////////////////////////////////////////////////////////////////////
            // 최초 비밀번호는 "0000"으로 설정하되, 
            // 엑셀 파일에서0000이 아닌, 숫자가 누락될 경우 
            // 옵션 -> 데이터 -> 자동데이터 변환 체크 해제 할 것
            //
            //
            //
            ///////////////////////////////////////////////////////////////////////////////////

        string password = "0000";

        // 교수 객체 생성 및 추가
        unique_ptr<User> newProf = make_unique<Professor>(name, id, password, phone, email);
        addUser(users, move(newProf));

        cout << "\"" << name << "\" 교수가 추가되었습니다. (최초 비밀번호는 0000입니다.)\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);

        // 교수 추가 후 루프 종료 (원하는 경우 계속 추가할 수 있도록 조정 가능)
        break;
    }
}


void Administrator::deleteProfessor(vector<unique_ptr<User>>& users) {
    string profID;
    viewProfessors(users);
    cout << "-------------------------------------------------------------------------------------\n";
    cout << "삭제할 교수 ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        cin >> profID;
        if (profID == "back") return; // 이전 메뉴로 돌아가기
        if (cin) break;
        cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: \n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 교수 정보 표시
    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "=== 삭제할 교수 정보 ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "이름: " << user->getName() << "\n";
            cout << "전화번호: " << user->getPhoneNumber() << "\n";
            cout << "이메일: " << user->getEmail() << "\n";
            found = true;
            break;
        }
    }

    if (found) {
        removeUser(users, profID);
        cout << "\"" << profID << "\" 교수가 삭제되었습니다.\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);
    }
    else {
        cout << "해당 ID의 교수를 찾을 수 없습니다.\n";
    }
}

// Student Management

void Administrator::viewStudents(const vector<unique_ptr<User>>& users) {
    cout << "학생 목록: \n";
    size_t maxEmailLength = 30; // 기본 이름 열 너비를 30으로 설정
    for (const auto& user : users) {
        maxEmailLength = max(maxEmailLength, user->getName().length() + 2);
    }

    printTableHeaderStudent(maxEmailLength);

    for (const auto& user : users) {
        if (user->getUserType() == "Student") {
            cout << setw(10) << user->getID()
                << setw(20) << user->getName()
                << setw(15) << user->getPhoneNumber()
                << setw(maxEmailLength) << user->getEmail()
                << setw(10) << dynamic_cast<Student*>(user.get())->getStudentID()
                << '\n';
        }
    }
}

void Administrator::viewStudentInfo(const vector<unique_ptr<User>>& users) {
    string studentID;
    viewStudents(users);
    cout << "-------------------------------------------------------------------------------------\n";
    cout << "학생 ID를 입력하세요: ";
    cin >> studentID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "=== 학생 정보 ===\n";
                cout << "ID: " << student->getID() << "\n";
                cout << "이름: " << student->getName() << "\n";
                cout << "전화번호: " << student->getPhoneNumber() << "\n";
                cout << "이메일: " << student->getEmail() << "\n";
                cout << "학번: " << student->getStudentID() << "\n";
                cout << "-------------------------------------------------------------------------------------\n\n";
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "해당 ID의 학생을 찾을 수 없습니다.\n";
    }
}

void Administrator::addStudent(vector<unique_ptr<User>>& users) {
    while (true) { // 학생 추가 과정을 반복
        string name, id, phone, email;
        int studentID;

        viewStudents(users);
        cout << "-------------------------------------------------------------------------------------\n";

        // ID 입력
        while (true) {
            cout << "ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            cin >> id;
            if (id == "back") return; // 이전 메뉴로 돌아가기

            // ID 중복 체크
            bool idExists = false;
            for (const auto& user : users) {
                if (user->getID() == id) {
                    cout << "오류: 이미 존재하는 ID입니다. 다른 ID를 입력하세요.\n";
                    idExists = true;
                    break;
                }
            }
            if (!idExists) break; // 중복되지 않는 ID라면 루프 종료
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 이름 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "이름을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, name);
            if (name == "back") return; // 이전 메뉴로 돌아가기
            if (!name.empty() && isValidName(name)) break;
            cout << "이름은 비워둘 수 없으며, 숫자가 포함될 수 없습니다. 다시 입력하세요: \n";
        }

        // 전화번호 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "전화번호를 입력하세요 [010-xxxx-xxxx 형식] ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, phone);
            if (phone == "back") return; // 이전 메뉴로 돌아가기

            // 전화번호 형식 체크
            regex phonePattern("^010-\\d{4}-\\d{4}$");
            if (!regex_match(phone, phonePattern)) {
                cout << "유효하지 않은 형식입니다. [010-xxxx-xxxx] 형식으로 다시 입력하세요.\n";
                continue;
            }

            // 전화번호 중복 체크
            bool phoneExists = false;
            for (const auto& user : users) {
                if (user->getPhoneNumber() == phone) {
                    cout << "오류: 이미 존재하는 전화번호입니다. 다른 전화번호를 입력하세요.\n";
                    phoneExists = true;
                    break;
                }
            }
            if (!phoneExists) break; // 중복되지 않는 전화번호라면 루프 종료
        }

        // 이메일 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "이메일을 입력하세요 [예: example@cau.ac.kr] ('back'을 입력해 이전 메뉴로 돌아가기): ";
            getline(cin, email);
            if (email == "back") return; // 이전 메뉴로 돌아가기

            // 이메일 형식 체크
            regex emailPattern("^[a-zA-Z0-9._%+-]+@cau\\.ac\\.kr$");
            if (regex_match(email, emailPattern)) {
                break; // 올바른 이메일 형식이라면 루프 종료
            }
            else {
                cout << "유효하지 않은 이메일 형식입니다. [예: example@cau.ac.kr] 다시 입력하세요.\n";
            }
        }

        // 학번 입력
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "학번을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return; // 이전 메뉴로 돌아가기

            try {
                studentID = stoi(input);
                break;
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 정수 학번을 입력하세요.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        /////////////////////////////////////////////////////////////////////////////////
       // 최초 비밀번호는 "0000"으로 설정하되, 
       // 엑셀 파일에서0000이 아닌, 숫자가 누락될 경우 
       // 옵션 -> 데이터 -> 자동데이터 변환 체크 해제 할 것
       //
       //
       //
       ///////////////////////////////////////////////////////////////////////////////////

        string password = "0000";

        // 학생 객체 생성 및 추가
        unique_ptr<User> newStudent = make_unique<Student>(name, id, password, phone, email, studentID);
        addUser(users, move(newStudent));

        cout << "\"" << name << "\" 학생이 추가되었습니다. (최초 비밀번호는 0000입니다.)\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewStudents(users);

        // 학생 추가 후 루프 종료
        break;
    }
}





void Administrator::deleteStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    string studentID;

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        cin >> studentID;
        if (studentID == "back") return; // 이전 메뉴로 돌아가기
        if (cin) break;
        cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 학생 정보 표시
    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            cout << "=== 삭제할 학생 정보 ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "이름: " << user->getName() << "\n";
            cout << "전화번호: " << user->getPhoneNumber() << "\n";
            cout << "이메일: " << user->getEmail() << "\n";
            cout << "학번: " << dynamic_cast<Student*>(user.get())->getStudentID() << "\n\n";
            found = true;
            break;
        }
    }

    if (found) {
        removeUser(users, studentID);
        cout << "\"" << studentID << "\" 학생이 삭제되었습니다.\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewStudents(users);
    }
    else {
        cout << "해당 ID의 학생을 찾을 수 없습니다.\n";
    }
}
