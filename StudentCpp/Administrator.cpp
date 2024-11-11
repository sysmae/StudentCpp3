// Administrator.cpp
#include "Administrator.h"
#include "Student.h"      // Student Ŭ���� ����
#include "Professor.h"    // Professor Ŭ���� ����
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
    // �ߺ� ID Ȯ��
    auto it = find_if(users.begin(), users.end(),
        [&newUser](const unique_ptr<User>& user) { return user->getID() == newUser->getID(); });

    if (it != users.end()) {
        cout << "����: �̹� �����ϴ� ID�Դϴ�. ����ڸ� �߰��� �� �����ϴ�.\n";
        return;
    }

    cout << "���ο� ����ڰ� �߰��Ǿ����ϴ�: " << newUser->getName() << endl;
    users.push_back(move(newUser));
}

// Remove User
void Administrator::removeUser(vector<unique_ptr<User>>& users, const string& userID) {
    auto it = remove_if(users.begin(), users.end(),
        [&userID](const unique_ptr<User>& user) { return user->getID() == userID; });

    if (it != users.end()) {
        cout << "����ڰ� ���������� ���ŵǾ����ϴ�: " << (*it)->getName() << endl;
        users.erase(it, users.end());
    }
    else {
        cout << "����ڸ� ã�� �� �����ϴ�.\n";
    }
}

// View All Users
void Administrator::viewAllUsers(const vector<unique_ptr<User>>& users) const {
    cout << "=== ��� ����� ��� ===\n";
    for (const auto& user : users) {
        cout << "ID: " << user->getID()
            << ", �̸�: " << user->getName()
            << ", ����: " << user->getUserType() << endl;
    }
}

// Subject Management
void Administrator::addSubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    int id;
    string name, type, termStr, professorID;
    double credit;
    int year;

    cout << "== ���� ��ϵ� ���� ��� ==\n";

    for (const auto& subject : subjects) {
        cout << "���� ID: " << subject.getID()
            << ", �̸�: " << subject.getName()
            << ", ���� ID: " << subject.getProfessorID() << endl;
    }

    cout << "=== ���� �߰� ===\n";
    cout << "���� ID�� �Է��ϼ���: ";
    while (!(cin >> id)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();  // ���� ����

    // �ߺ� ID Ȯ��
    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "����: �̹� �����ϴ� ���� ID�Դϴ�.\n";
        return;
    }

    cout << "================================================================================\n";
    cout << "���� �̸��� �Է��ϼ���: ";
    cin.ignore(); // ���� �Է� ���� ����
    while (true) {
        getline(cin, name);
        if (!name.empty()) break;
        cout << "���� �̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "================================================================================\n";
    cout << "���� ������ �Է��ϼ��� (Required/Elective): ";
    while (true) {
        getline(cin, type);
        if (type == "Required" || type == "Elective") break;
        cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ��� (Required/Elective): ";
    }

    cout << "================================================================================\n";
    cout << "������ �Է��ϼ���: ";
    while (!(cin >> credit) || (credit != 2 && credit != 3)) {
        cout << "��ȿ���� ���� �����Դϴ�. 2���� Ȥ�� 3���� �� �ϳ��� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    cout << "================================================================================\n";
    cout << "�⵵�� �Է��ϼ���: ";
    while (!(cin >> year) || year < 0) {
        cout << "��ȿ���� ���� �����Դϴ�. �ٽ� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    cout << "================================================================================\n";
    cout << "�б⸦ �Է��ϼ��� (FIRST_TERM/SECOND_TERM): ";
    while (true) {
        getline(cin, termStr);
        if (termStr == "FIRST_TERM" || termStr == "SECOND_TERM") break;
        cout << "�ٽ� �Է��ϼ��� (FIRST_TERM, SECOND_TERM �� �ϳ��� �Է��ϼ���): ";
    }

    cout << "================================================================================\n";
    viewProfessors(users);
    cout << "��� ���� ID�� �Է��ϼ��� (������ ����): ";
    getline(cin, professorID);

    Term term;
    if (termStr == "FIRST_TERM") {
        term = Term::FIRST_TERM;
    }
    else if (termStr == "SECOND_TERM") {
        term = Term::SECOND_TERM;
    }
    else {
        cout << "�߸��� �б� �Է��Դϴ�.\n";
        return;
    }

    cout << "================================================================================\n";
    subjects.emplace_back(Subject(id, name, credit, type, year, term, professorID));
    cout << "\"" << name << "\" ������ �����Ǿ����ϴ�.\n";
}

void Administrator::deleteSubject(vector<Subject>& subjects) {
    int id;
    cout << "=== ���� ���� ===\n";
    cout << "���� ��ϵ� ���� ���:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName() << endl;
    }

    cout << "================================================================================\n";
    cout << "������ ���� ID�� �Է��ϼ���: ";
    while (!(cin >> id)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();  // ���� ����

    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "\"" << it->getName() << "\" ������ �����Ǿ����ϴ�.\n";
        subjects.erase(it);
    }
    else {
        cout << "������ ã�� �� �����ϴ�.\n";
    }
}

void Administrator::modifySubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    int id;
    cout << "=== ���� ���� ===\n";
    cout << "���� ��ϵ� ���� ���:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName() << endl;
    }

    cout << "================================================================================\n";
    cout << "������ ���� ID�� �Է��ϼ���: ";
    while (!(cin >> id)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();  // ���� ����
    cout << "================================================================================\n";
    auto it = find_if(subjects.begin(), subjects.end(),
        [id](Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        string newName, newType, newTermStr, newProfessorID;
        double newCredit;
        int newYear;

        cout << "���� ���� ����:\n";
        cout << "ID: " << it->getID() << "\n";
        cout << "�̸�: " << it->getName() << "\n";
        cout << "����: " << it->getType() << "\n";
        cout << "����: " << it->getCredit() << "\n";
        cout << "�⵵: " << it->getYear() << "\n";
        cout << "�б�: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "\n";
        cout << "��� ���� ID: " << it->getProfessorID() << "\n\n";

        cout << "================================================================================\n";
        cout << "�� ���� �̸��� �Է��ϼ��� (����: " << it->getName() << "): ";
        cin.ignore(); // ���� �Է� ���� ����
        while (true) {
            getline(cin, newName);
            if (!newName.empty()) break;
            cout << "���� �̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
        }

        cout << "================================================================================\n";
        cout << "�� ���� ������ �Է��ϼ��� (Required/Elective) (����: " << it->getType() << "): ";
        while (true) {
            getline(cin, newType);
            if (newType == "Required" || newType == "Elective") break;
            cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ��� (Required/Elective): ";
        }

        cout << "================================================================================\n";
        cout << "�� ������ �Է��ϼ��� (����: " << it->getCredit() << "): ";
        while (!(cin >> newCredit) || (newCredit != 2 && newCredit != 3)) {
            cout << "��ȿ���� ���� �����Դϴ�. 2���� Ȥ�� 3���� �� �ϳ��� �Է��ϼ���: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "================================================================================\n";
        cout << "�� �⵵�� �Է��ϼ��� (����: " << it->getYear() << "): ";
        while (!(cin >> newYear) || newYear < 0) {
            cout << "��ȿ���� ���� �����Դϴ�. �ٽ� �Է��ϼ���: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "================================================================================\n";
        cout << "�� �б⸦ �Է��ϼ��� (FIRST_TERM/SECOND_TERM) (����: "
            << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "): ";
        while (true) {
            cin >> newTermStr;
            if (newTermStr == "FIRST_TERM" || newTermStr == "SECOND_TERM") break;
            cout << "�ٽ� �Է��ϼ��� (FIRST_TERM, SECOND_TERM �� �ϳ��� �Է��ϼ���): ";
        }

        cout << "================================================================================\n";
        viewProfessors(users);
        cout << "�� ��� ���� ID�� �Է��ϼ��� (����: " << it->getProfessorID() << "): ";
        while (!(cin >> newProfessorID)) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Term newTerm;
        if (newTermStr == "FIRST_TERM") {
            newTerm = Term::FIRST_TERM;
        }
        else if (newTermStr == "SECOND_TERM") {
            newTerm = Term::SECOND_TERM;
        }
        else {
            cout << "�߸��� �б� �Է��Դϴ�.\n";
            return;
        }

        it->setName(newName);
        it->setType(newType);
        it->setCredit(newCredit);
        it->setYear(newYear);
        it->setTerm(newTerm);
        it->setProfessorID(newProfessorID);

        cout << "�ش� ������ �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "������ ã�� �� �����ϴ�.\n";
    }
}


void Administrator::viewSubjects(const vector<Subject>& subjects) const {
    cout << "=== ���� ��� ===\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName()
            << ", ����: " << subject.getType()
            << ", ����: " << subject.getCredit()
            << ", �⵵: " << subject.getYear()
            << ", �б�: " << (subject.getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM")
            << ", ���� ID: " << subject.getProfessorID() << "\n";
    }
}

// Professor Management

void Administrator::viewProfessors(const vector<unique_ptr<User>>& users) const {
    cout << "=== ���� ��� ===\n";
    for (const auto& user : users) {
        if (user->getUserType() == "Professor") {
            cout << "ID: " << user->getID()
                << ", �̸�: " << user->getName()
                << ", ��ȭ��ȣ: " << user->getPhoneNumber()
                << ", �̸���: " << user->getEmail() << "\n";
        }
    }
}

void Administrator::viewProfessorInfo(const vector<unique_ptr<User>>& users) const {
    cout << "=== ���� ���� ��ȸ ===\n";
    string profID;
    cout << "���� ID�� �Է��ϼ���: ";
    cin >> profID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "=== ���� ���� ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "�̸�: " << user->getName() << "\n";
            cout << "��ȭ��ȣ: " << user->getPhoneNumber() << "\n";
            cout << "�̸���: " << user->getEmail() << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
    }
}

void Administrator::addProfessor(vector<unique_ptr<User>>& users) {
    viewProfessors(users);
    cout << "=== ���� �߰� ===\n";
    string id, name, phone, email;
    cout << "ID�� �Է��ϼ���: ";
    while (!(cin >> id)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "================================================================================\n";
    cout << "�̸��� �Է��ϼ���: ";
    cin.ignore(); // ���� �Է� ���� ����
    while (true) {
        getline(cin, name);
        if (!name.empty()) break;
        cout << "�̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "================================================================================\n";
    cout << "��ȭ��ȣ�� �Է��ϼ���: ";
    while (!(getline(cin, phone))) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���ڸ� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "================================================================================\n";
    cout << "�̸����� �Է��ϼ���: ";
    getline(cin, email);

    // ���� ��й�ȣ�� "0000"���� ����
    string password = "0000";

    unique_ptr<User> newProf = make_unique<Professor>(name, id, password, phone, email);
    addUser(users, move(newProf));

    cout << "\"" << name << "\" ������ �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n";
}

void Administrator::deleteProfessor(vector<unique_ptr<User>>& users) {
    viewProfessors(users);
    cout << "=== ���� ���� ===\n";
    string profID;
    cout << "������ ���� ID�� �Է��ϼ���: ";
    cin >> profID;

    // ���� ���� ǥ��
    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "=== ������ ���� ���� ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "�̸�: " << user->getName() << "\n";
            cout << "��ȭ��ȣ: " << user->getPhoneNumber() << "\n";
            cout << "�̸���: " << user->getEmail() << "\n";
            found = true;
            break;
        }
    }

    if (found) {
        removeUser(users, profID);
        cout << "\"" << profID << "\" ������ �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
    }
}

// Student Management

void Administrator::viewStudents(const vector<unique_ptr<User>>& users) const {
    cout << "=== �л� ��� ===\n";
    for (const auto& user : users) {
        if (user->getUserType() == "Student") {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "ID: " << student->getID()
                    << ", �̸�: " << student->getName()
                    << ", ��ȭ��ȣ: " << student->getPhoneNumber()
                    << ", �̸���: " << student->getEmail()
                    << ", �й�: " << student->getStudentID() << "\n";
            }
        }
    }
}

void Administrator::viewStudentInfo(const vector<unique_ptr<User>>& users) const {
    cout << "=== �л� ���� ��ȸ ===\n";
    string studentID;
    cout << "�л� ID�� �Է��ϼ���: ";
    cin >> studentID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "=== �л� ���� ===\n";
                cout << "ID: " << student->getID() << "\n";
                cout << "�̸�: " << student->getName() << "\n";
                cout << "��ȭ��ȣ: " << student->getPhoneNumber() << "\n";
                cout << "�̸���: " << student->getEmail() << "\n";
                cout << "�й�: " << student->getStudentID() << "\n";
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "�ش� ID�� �л��� ã�� �� �����ϴ�.\n";
    }
}

void Administrator::addStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    cout << "=== �л� �߰� ===\n";
    string name, id, phone, email;
    int studentID;

    cout << "ID�� �Է��ϼ���: ";
    while (!(cin >> id)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "================================================================================\n";
    cout << "�̸��� �Է��ϼ���: ";
    cin.ignore(); // ���� �Է� ���� ����
    while (true) {
        getline(cin, name);
        if (!name.empty()) break;
        cout << "�̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "================================================================================\n";
    cout << "��ȭ��ȣ�� �Է��ϼ���: ";
    while (!(getline(cin, phone))) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���ڸ� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "================================================================================\n";
    cout << "�̸����� �Է��ϼ���: ";
    getline(cin, email);

    cout << "================================================================================\n";
    cout << "�й��� �Է��ϼ���: ";
    while (!(cin >> studentID)) {
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ���� ��й�ȣ�� "0000"���� ����
    string password = "0000";

    unique_ptr<User> newStudent = make_unique<Student>(name, id, password, phone, email, studentID);
    addUser(users, move(newStudent));

    cout << "\"" << name << "\" �л��� �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n";
}

void Administrator::deleteStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    cout << "=== �л� ���� ===\n";
    string studentID;
    cout << "������ �л� ID�� �Է��ϼ���: ";
    cin >> studentID;

    // �л� ���� ǥ��
    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            cout << "=== ������ �л� ���� ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "�̸�: " << user->getName() << "\n";
            cout << "��ȭ��ȣ: " << user->getPhoneNumber() << "\n";
            cout << "�̸���: " << user->getEmail() << "\n";
            cout << "�й�: " << dynamic_cast<Student*>(user.get())->getStudentID() << "\n";
            found = true;
            break;
        }
    }

    if (found) {
        removeUser(users, studentID);
        cout << "\"" << studentID << "\" �л��� �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "�ش� ID�� �л��� ã�� �� �����ϴ�.\n";
    }
}
