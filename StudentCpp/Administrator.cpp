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

    cout << "\n���� ��ϵ� ���� ���: \n";
    for (const auto& subject : subjects) {
        cout << "���� ID: " << subject.getID()
            << ", �̸�: " << subject.getName()
            << ", ���� ID: " << subject.getProfessorID() << endl;
    }

    
    cout << "\n-------------------------------------------------------------------------------------\n";
    // ���� ID �Է�
    while (true) {
        cout << "���� ID�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        }
    }
    cin.ignore();

    // �ߺ� ID Ȯ��
    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "����: �̹� �����ϴ� ���� ID�Դϴ�.\n";
        return;
    }

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "���� �̸��� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        getline(cin, name);
        if (name == "back") return;
        if (!name.empty()) break;
        cout << "���� �̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "���� ������ �Է��ϼ��� (Required/Elective/Basic) ('back'�� �Է��� �����޴� ���ư���): ";
        getline(cin, type);
        if (type == "back") return;
        if (type == "Required" || type == "Elective" || type=="Basic") break;
        cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ���: ";
    }
    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "������ �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            credit = stod(input);
            if (credit == 2 || credit == 3) break;
            cout << "��ȿ���� ���� �����Դϴ�. 2���� Ȥ�� 3���� �� �ϳ��� �Է��ϼ���: ";
        }
        catch (...) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ���ڸ� �Է��ϼ���: ";
        }
    }
    cin.ignore();

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "�⵵�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            year = stoi(input);
            if (year > 0) break;
            cout << "��ȿ���� ���� �����Դϴ�. ���� ������ �Է��ϼ���: ";
        }
        catch (...) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ������ �Է��ϼ���: ";
        }
    }
    cin.ignore();

    cout << "-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "�б⸦ �Է��ϼ��� (FIRST_TERM/SECOND_TERM) ('back'�� �Է��� �����޴� ���ư���): ";
        getline(cin, termStr);
        if (termStr == "back") return;
        if (termStr == "FIRST_TERM" || termStr == "SECOND_TERM") break;
        cout << "�߸��� �б��Դϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "��� ���� ID�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
    getline(cin, professorID);
    if (professorID == "back") return;

    Term term = (termStr == "FIRST_TERM") ? Term::FIRST_TERM : Term::SECOND_TERM;

    subjects.emplace_back(Subject(id, name, credit, type, year, term, professorID));
    cout << "\"" << name << "\" ������ �����Ǿ����ϴ�.\n";
}

void Administrator::deleteSubject(vector<Subject>& subjects) {
    int id;
    cout << "\n���� ��ϵ� ���� ���:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName() << endl;
    }

    cout << "\n-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "������ ���� ID�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        }
    }
    cin.ignore();

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
    cout << "\n���� ��ϵ� ���� ���:\n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName() << endl;
    }

    cout << "\n-------------------------------------------------------------------------------------\n";
    while (true) {
        cout << "������ ���� ID�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
        string input;
        cin >> input;
        if (input == "back") return;
        try {
            id = stoi(input);
            break;
        }
        catch (...) {
            cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
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

        cout << "���� ���� ����:\n";
        cout << "ID: " << it->getID() << "\n";
        cout << "�̸�: " << it->getName() << "\n";
        cout << "����: " << it->getType() << "\n";
        cout << "����: " << it->getCredit() << "\n";
        cout << "�⵵: " << it->getYear() << "\n";
        cout << "�б�: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "\n";
        cout << "��� ���� ID: " << it->getProfessorID() << "\n";

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "�� ���� �̸��� �Է��ϼ��� (����: " << it->getName() << ")('back'�� �Է��� �����޴� ���ư���): ";
            getline(cin, newName);
            if (newName == "back") return;
            if (!newName.empty()) break;
            cout << "���� �̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "�� ���� ������ �Է��ϼ��� (Required/Elective/Basic)('back'�� �Է��� �����޴� ���ư���): ";
            getline(cin, newType);
            if (newType == "back") return;
            if (newType == "Required" || newType == "Elective" || newType == "Basic") break;
            cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ��� (Required/Elective): ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "�� ������ �Է��ϼ��� (����: " << it->getCredit() << ")('back'�� �Է��� �����޴� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                newCredit = stod(input);
                if (newCredit == 2 || newCredit == 3) break;
                cout << "��ȿ���� ���� �����Դϴ�. 2���� Ȥ�� 3���� �� �ϳ��� �Է��ϼ���: ";
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. 2���� Ȥ�� 3���� �� �ϳ��� �Է��ϼ���: ";
            }
        }
        cin.ignore();

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "�� �⵵�� �Է��ϼ��� (����: " << it->getYear() << ")('back'�� �Է��� �����޴� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                newYear = stoi(input);
                if (newYear > 0) break;
                cout << "��ȿ���� ���� �����Դϴ�. ���� ������ �Է��ϼ���: ";
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ������ �Է��ϼ���: ";
            }
        }
        cin.ignore();

        cout << "-------------------------------------------------------------------------------------\n";
        while (true) {
            cout << "�б⸦ �Է��ϼ��� (FIRST_TERM/SECOND_TERM) ('back'�� �Է��� �����޴� ���ư���): ";
            getline(cin, newTermStr);
            if (newTermStr == "back") return;
            if (newTermStr == "FIRST_TERM" || newTermStr == "SECOND_TERM") break;
            cout << "�߸��� �б��Դϴ�. �ٽ� �Է��ϼ��� (FIRST_TERM, SECOND_TERM �� �ϳ��� �Է��ϼ���): ";
        }

        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);
        cout << "��� ���� ID�� �Է��ϼ��� ('back'�� �Է��� �����޴� ���ư���): ";
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
    cout << "\n���� ��ϵ� ���� ���: \n";
    for (const auto& subject : subjects) {
        cout << "ID: " << subject.getID()
            << ", �̸�: " << subject.getName()
            << ", ����: " << subject.getType()
            << ", ����: " << subject.getCredit()
            << ", �⵵: " << subject.getYear()
            << ", �б�: " << (subject.getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM")
            << ", ���� ID: " << subject.getProfessorID() << "\n\n";
    }
}

// Professor Management

void Administrator::viewProfessors(const vector<unique_ptr<User>>& users) const {
    cout << "\n���� ���: \n";
    for (const auto& user : users) {
        if (user->getUserType() == "Professor") {
            cout << "ID: " << user->getID()
                << ", �̸�: " << user->getName()
                << ", ��ȭ��ȣ: " << user->getPhoneNumber()
                << ", �̸���: " << user->getEmail() << "\n\n";
        }
    }
}

void Administrator::viewProfessorInfo(const vector<unique_ptr<User>>& users) const {
    viewProfessors(users);
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
            cout << "�̸���: " << user->getEmail() << "\n\n";
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
    string id, name, phone, email;

    cout << "ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        cin >> id;
        if (id == "back") return; // ���� �޴��� ���ư���
        if (cin) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "�̸��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    cin.ignore(); // ���� �Է� ���� ����
    while (true) {
        getline(cin, name);
        if (name == "back") return; // ���� �޴��� ���ư���
        if (!name.empty()) break;
        cout << "�̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "��ȭ��ȣ�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        getline(cin, phone);
        if (phone == "back") return; // ���� �޴��� ���ư���
        if (!phone.empty()) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ��ȭ��ȣ�� �ٽ� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "�̸����� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    getline(cin, email);
    if (email == "back") return; // ���� �޴��� ���ư���

    // ���� ��й�ȣ�� "0000"���� ����
    string password = "0000";

    unique_ptr<User> newProf = make_unique<Professor>(name, id, password, phone, email);
    addUser(users, move(newProf));

    cout << "\"" << name << "\" ������ �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n";
}

void Administrator::deleteProfessor(vector<unique_ptr<User>>& users) {
    string profID;
    viewProfessors(users);
    cout << "������ ���� ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        cin >> profID;
        if (profID == "back") return; // ���� �޴��� ���ư���
        if (cin) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

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
    cout << "\n�л� ���: \n";
    for (const auto& user : users) {
        if (user->getUserType() == "Student") {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "ID: " << student->getID()
                    << ", �̸�: " << student->getName()
                    << ", ��ȭ��ȣ: " << student->getPhoneNumber()
                    << ", �̸���: " << student->getEmail()
                    << ", �й�: " << student->getStudentID() << "\n\n";
            }
        }
    }
}

void Administrator::viewStudentInfo(const vector<unique_ptr<User>>& users) const {
    string studentID;
    viewStudents(users);
    cout << "�л� ID�� �Է��ϼ���: \n";
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
                cout << "�й�: " << student->getStudentID() << "\n\n";
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
    string name, id, phone, email;
    int studentID;

    cout << "ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): \n";
    while (true) {
        cin >> id;
        if (id == "back") return; // ���� �޴��� ���ư���
        if (cin) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "�̸��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    cin.ignore(); // ���� �Է� ���� ����
    while (true) {
        getline(cin, name);
        if (name == "back") return; // ���� �޴��� ���ư���
        if (!name.empty()) break;
        cout << "�̸��� ����� �� �����ϴ�. �ٽ� �Է��ϼ���: ";
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "��ȭ��ȣ�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        getline(cin, phone);
        if (phone == "back") return; // ���� �޴��� ���ư���
        if (!phone.empty()) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ��ȭ��ȣ�� �ٽ� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "�̸����� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    getline(cin, email);
    if (email == "back") return; // ���� �޴��� ���ư���

    cout << "-------------------------------------------------------------------------------------\n";
    cout << "�й��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        cin >> studentID;
        if (cin) break;
        if (studentID == 0) return; // ���� �޴��� ���ư���
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� �й��� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ���� ��й�ȣ�� "0000"���� ����
    string password = "0000";

    unique_ptr<User> newStudent = make_unique<Student>(name, id, password, phone, email, studentID);
    addUser(users, move(newStudent));

    cout << "\"" << name << "\" �л��� �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n\n";
}

void Administrator::deleteStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    string studentID;
    cout << "ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        cin >> studentID;
        if (studentID == "back") return; // ���� �޴��� ���ư���
        if (cin) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // �л� ���� ǥ��
    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            cout << "=== ������ �л� ���� ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "�̸�: " << user->getName() << "\n";
            cout << "��ȭ��ȣ: " << user->getPhoneNumber() << "\n";
            cout << "�̸���: " << user->getEmail() << "\n";
            cout << "�й�: " << dynamic_cast<Student*>(user.get())->getStudentID() << "\n\n";
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
