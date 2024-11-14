// Administrator.cpp
#include "Administrator.h"
#include "Student.h"      // Student 클래스 포함
#include "Professor.h"    // Professor 클래스 포함
#include <iostream>
#include <algorithm>

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

// Subject Management
void Administrator::addSubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    int id;
    string name, type, termStr, professorID;
    double credit;
    int year;

    cout << "\n현재 등록된 과목 목록: \n";
    for (const auto& subject : subjects) {
        cout << "과목 ID: " << subject.getID()
            << ", 이름: " << subject.getName()
            << ", 교수 ID: " << subject.getProfessorID() << endl;
    }

    
    cout << "\n-------------------------------------------------------------------------------------\n";
    // 과목 ID 입력
    while (true) {
        cout << "과목 ID를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        }
    }
    cin.ignore();

    // 중복 ID 확인
    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "오류: 이미 존재하는 과목 ID입니다.\n";
        return;
    }

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "과목 이름을 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        getline(cin, name);
        if (name == "back") return;
        if (!name.empty()) break;
        cout << "과목 이름은 비워둘 수 없습니다. 다시 입력하세요: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "과목 유형을 입력하세요 (Required/Elective/Basic) ('back'을 입력해 이전메뉴 돌아가기): ";
        getline(cin, type);
        if (type == "back") return;
        if (type == "Required" || type == "Elective" || type=="Basic") break;
        cout << "잘못된 유형입니다. 다시 입력하세요: ";
    }
    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "학점을 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            credit = stod(input);
            if (credit == 2 || credit == 3) break;
            cout << "유효하지 않은 학점입니다. 2학점 혹은 3학점 중 하나를 입력하세요: ";
        }
        catch (...) {
            cout << "유효하지 않은 입력입니다. 숫자를 입력하세요: ";
        }
    }
    cin.ignore();

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "년도를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            year = stoi(input);
            if (year > 0) break;
            cout << "유효하지 않은 연도입니다. 양의 정수를 입력하세요: ";
        }
        catch (...) {
            cout << "유효하지 않은 입력입니다. 정수를 입력하세요: ";
        }
    }
    cin.ignore();

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "학기를 입력하세요 (FIRST_TERM/SECOND_TERM) ('back'을 입력해 이전메뉴 돌아가기): ";
        getline(cin, termStr);
        if (termStr == "back") return;
        if (termStr == "FIRST_TERM" || termStr == "SECOND_TERM") break;
        cout << "잘못된 학기입니다. 다시 입력하세요: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "담당 교수 ID를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
    getline(cin, professorID);
    if (professorID == "back") return;

    Term term = (termStr == "FIRST_TERM") ? Term::FIRST_TERM : Term::SECOND_TERM;

    subjects.emplace_back(Subject(id, name, credit, type, year, term, professorID));
    cout << "\"" << name << "\" 과목이 생성되었습니다.\n";
}

void Administrator::deleteSubject(vector<Subject>& subjects) {
    int id;
    cout << "\n현재 등록된 과목 목록:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", 이름: " << subject.getName() << endl;
    }

    cout << "\n-------------------------------------------------------------------------------------\n";
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
            cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        }
    }
    cin.ignore();

    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "\"" << it->getName() << "\" 과목이 삭제되었습니다.\n";
        subjects.erase(it);
    }
    else {
        cout << "과목을 찾을 수 없습니다.\n";
    }
}

void Administrator::modifySubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    int id;
    cout << "\n현재 등록된 과목 목록:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", 이름: " << subject.getName() << endl;
    }

    cout << "\n-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "수정할 과목 ID를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        }
    }
    cin.ignore();

    cout << "-------------------------------------------------------------------------------------\n";
    auto it = find_if(subjects.begin(), subjects.end(),
        [id](Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        string newName, newType, newTermStr, newProfessorID;
        double newCredit;
        int newYear;

        cout << "현재 과목 정보:\n";
        cout << "ID: " << it->getID() << "\n";
        cout << "이름: " << it->getName() << "\n";
        cout << "유형: " << it->getType() << "\n";
        cout << "학점: " << it->getCredit() << "\n";
        cout << "년도: " << it->getYear() << "\n";
        cout << "학기: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "\n";
        cout << "담당 교수 ID: " << it->getProfessorID() << "\n";

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "새 과목 이름을 입력하세요 (현재: " << it->getName() << ")('back'을 입력해 이전메뉴 돌아가기): ";
            getline(cin, newName);
            if (newName == "back") return;
            if (!newName.empty()) break;
            cout << "과목 이름은 비워둘 수 없습니다. 다시 입력하세요: ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "새 과목 유형을 입력하세요 (Required/Elective/Basic)('back'을 입력해 이전메뉴 돌아가기): ";
            getline(cin, newType);
            if (newType == "back") return;
            if (newType == "Required" || newType == "Elective" || newType == "Basic") break;
            cout << "잘못된 유형입니다. 다시 입력하세요 (Required/Elective): ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "새 학점을 입력하세요 (현재: " << it->getCredit() << ")('back'을 입력해 이전메뉴 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                newCredit = stod(input);
                if (newCredit == 2 || newCredit == 3) break;
                cout << "유효하지 않은 학점입니다. 2학점 혹은 3학점 중 하나를 입력하세요: ";
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 2학점 혹은 3학점 중 하나를 입력하세요: ";
            }
        }
        cin.ignore();

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "새 년도를 입력하세요 (현재: " << it->getYear() << ")('back'을 입력해 이전메뉴 돌아가기): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                newYear = stoi(input);
                if (newYear > 0) break;
                cout << "유효하지 않은 연도입니다. 양의 정수를 입력하세요: ";
            }
            catch (...) {
                cout << "유효하지 않은 입력입니다. 정수를 입력하세요: ";
            }
        }
        cin.ignore();

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "학기를 입력하세요 (FIRST_TERM/SECOND_TERM) ('back'을 입력해 이전메뉴 돌아가기): ";
            getline(cin, newTermStr);
            if (newTermStr == "back") return;
            if (newTermStr == "FIRST_TERM" || newTermStr == "SECOND_TERM") break;
            cout << "잘못된 학기입니다. 다시 입력하세요 (FIRST_TERM, SECOND_TERM 중 하나를 입력하세요): ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);
        cout << "담당 교수 ID를 입력하세요 ('back'을 입력해 이전메뉴 돌아가기): ";
        getline(cin, newProfessorID);
        if (newProfessorID == "back") return;

        Term newTerm;
        if (newTermStr == "FIRST_TERM") {
            newTerm = Term::FIRST_TERM;
        }
        else if (newTermStr == "SECOND_TERM") {
            newTerm = Term::SECOND_TERM;
        }
        else {
            cout << "잘못된 학기 입력입니다.\n";
            return;
        }

        it->setName(newName);
        it->setType(newType);
        it->setCredit(newCredit);
        it->setYear(newYear);
        it->setTerm(newTerm);
        it->setProfessorID(newProfessorID);

        cout << "해당 과목이 수정되었습니다.\n";
    }
    else {
        cout << "과목을 찾을 수 없습니다.\n";
    }
}

void Administrator::viewSubjects(const vector<Subject>& subjects) const {
    cout << "\n현재 등록된 과목 목록: \n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", 이름: " << subject.getName()
            << ", 유형: " << subject.getType()
            << ", 학점: " << subject.getCredit()
            << ", 년도: " << subject.getYear()
            << ", 학기: " << (subject.getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM")
            << ", 교수 ID: " << subject.getProfessorID() << "\n\n";
    }
}

// Professor Management

void Administrator::viewProfessors(const vector<unique_ptr<User>>& users) const {
    cout << "\n교수 목록: \n";
    for (const auto& user : users) {
        if (user->getUserType() == "Professor") {
            cout << "ID: " << user->getID()
                << ", 이름: " << user->getName()
                << ", 전화번호: " << user->getPhoneNumber()
                << ", 이메일: " << user->getEmail() << "\n\n";
        }
    }
}

void Administrator::viewProfessorInfo(const vector<unique_ptr<User>>& users) const {
    viewProfessors(users);
    string profID;
    cout << "교수 ID을 입력하세요: ";
    cin >> profID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "=== 교수 정보 ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "이름: " << user->getName() << "\n";
            cout << "전화번호: " << user->getPhoneNumber() << "\n";
            cout << "이메일: " << user->getEmail() << "\n\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "해당 ID의 교수를 찾을 수 없습니다.\n";
    }
}

void Administrator::addProfessor(vector<unique_ptr<User>>& users) {
    viewProfessors(users);
    string id, name, phone, email;

    cout << "ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        cin >> id;
        if (id == "back") return; // 이전 메뉴로 돌아가기
        if (cin) break;
        cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "이름을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    cin.ignore(); // 이전 입력 버퍼 비우기
    while (true) {
        getline(cin, name);
        if (name == "back") return; // 이전 메뉴로 돌아가기
        if (!name.empty()) break;
        cout << "이름은 비워둘 수 없습니다. 다시 입력하세요: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "전화번호를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        getline(cin, phone);
        if (phone == "back") return; // 이전 메뉴로 돌아가기
        if (!phone.empty()) break;
        cout << "유효하지 않은 입력입니다. 전화번호를 다시 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "이메일을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    getline(cin, email);
    if (email == "back") return; // 이전 메뉴로 돌아가기

    // 최초 비밀번호는 "0000"으로 설정
    string password = "0000";

    unique_ptr<User> newProf = make_unique<Professor>(name, id, password, phone, email);
    addUser(users, move(newProf));

    cout << "\"" << name << "\" 교수가 추가되었습니다. (최초 비밀번호는 0000입니다.)\n";
}

void Administrator::deleteProfessor(vector<unique_ptr<User>>& users) {
    string profID;
    viewProfessors(users);
    cout << "삭제할 교수 ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        cin >> profID;
        if (profID == "back") return; // 이전 메뉴로 돌아가기
        if (cin) break;
        cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
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
    }
    else {
        cout << "해당 ID의 교수를 찾을 수 없습니다.\n";
    }
}

// Student Management

void Administrator::viewStudents(const vector<unique_ptr<User>>& users) const {
    cout << "\n학생 목록: \n";
    for (const auto& user : users) {
        if (user->getUserType() == "Student") {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "ID: " << student->getID()
                    << ", 이름: " << student->getName()
                    << ", 전화번호: " << student->getPhoneNumber()
                    << ", 이메일: " << student->getEmail()
                    << ", 학번: " << student->getStudentID() << "\n\n";
            }
        }
    }
}

void Administrator::viewStudentInfo(const vector<unique_ptr<User>>& users) const {
    string studentID;
    viewStudents(users);
    cout << "학생 ID를 입력하세요: \n";
    cin >> studentID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "=== 학생 정보 ===\n";
                cout << "ID: " << student->getID() << "\n";
                cout << "이름: " << student->getName() << "\n";
                cout << "전화번호: " << student->getPhoneNumber() << "\n";
                cout << "이메일: " << student->getEmail() << "\n";
                cout << "학번: " << student->getStudentID() << "\n\n";
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
    viewStudents(users);
    string name, id, phone, email;
    int studentID;

    cout << "ID를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): \n";
    while (true) {
        cin >> id;
        if (id == "back") return; // 이전 메뉴로 돌아가기
        if (cin) break;
        cout << "유효하지 않은 입력입니다. 정수 ID를 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "이름을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    cin.ignore(); // 이전 입력 버퍼 비우기
    while (true) {
        getline(cin, name);
        if (name == "back") return; // 이전 메뉴로 돌아가기
        if (!name.empty()) break;
        cout << "이름은 비워둘 수 없습니다. 다시 입력하세요: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "전화번호를 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        getline(cin, phone);
        if (phone == "back") return; // 이전 메뉴로 돌아가기
        if (!phone.empty()) break;
        cout << "유효하지 않은 입력입니다. 전화번호를 다시 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "이메일을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    getline(cin, email);
    if (email == "back") return; // 이전 메뉴로 돌아가기

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "학번을 입력하세요 ('back'을 입력해 이전 메뉴로 돌아가기): ";
    while (true) {
        cin >> studentID;
        if (cin) break;
        if (studentID == 0) return; // 이전 메뉴로 돌아가기
        cout << "유효하지 않은 입력입니다. 정수 학번을 입력하세요: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 최초 비밀번호는 "0000"으로 설정
    string password = "0000";

    unique_ptr<User> newStudent = make_unique<Student>(name, id, password, phone, email, studentID);
    addUser(users, move(newStudent));

    cout << "\"" << name << "\" 학생이 추가되었습니다. (최초 비밀번호는 0000입니다.)\n\n";
}

void Administrator::deleteStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    string studentID;
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
    }
    else {
        cout << "해당 ID의 학생을 찾을 수 없습니다.\n";
    }
}
