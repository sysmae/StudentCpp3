// Administrator.cpp
#include "Administrator.h"
#include "Student.h"      // Student Ŭ���� ����
#include "Professor.h"    // Professor Ŭ���� ����
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

void Administrator::addSubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    while (true) { // ��ü ���� �߰� ������ �ݺ�
        int id;
        string name, type, termStr, professorID;
        double credit;
        int year;

        viewSubjects(subjects);
        cout << "-------------------------------------------------------------------------------------\n";

        // ���� ID �Է�
        while (true) {
            cout << "���� ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                id = stoi(input);
                // �ߺ� ID Ȯ��
                auto it = find_if(subjects.begin(), subjects.end(),
                    [id](const Subject& s) { return s.getID() == id; });
                if (it != subjects.end()) {
                    cout << "����: �̹� �����ϴ� ���� ID�Դϴ�. �ٸ� ID�� �Է��ϼ���.\n";
                }
                else {
                    break;
                }
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // ���� �̸� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "���� �̸��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): \n";
            getline(cin, name);
            if (name == "back") return;
            if (!name.empty() && isValidName(name)) break;
            cout << "���� �̸��� ����� �� ������, ���ڰ� ���Ե� �� �����ϴ�. �ٽ� �Է��ϼ���.\n";
        }

        // ���� ���� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "���� ������ �Է��ϼ��� (Required/Elective/Basic) ('back'�� �Է��� ���� �޴��� ���ư���): \n";
            getline(cin, type);
            if (type == "back") return;
            if (type == "Required" || type == "Elective" || type == "Basic") break;
            cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ���.\n";
        }

        // ���� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "������ �Է��ϼ��� (1 OR 2 OR 3)('back'�� �Է��� ���� �޴��� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                credit = stod(input);
                if (credit == 1 || credit == 2 || credit == 3) break;
                cout << "��ȿ���� ���� �����Դϴ�. 1, 2 Ȥ�� 3 ���� �� �ϳ��� �Է��ϼ���.\n";
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ���ڸ� �Է��ϼ���.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // �⵵ �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�⵵�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return;
            try {
                year = stoi(input);
                if (year > 0) break;
                cout << "��ȿ���� ���� �����Դϴ�. ���� ������ �Է��ϼ���.\n";
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ������ �Է��ϼ���.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // �б� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�б⸦ �Է��ϼ��� (FIRST_TERM/SECOND_TERM) ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, termStr);
            if (termStr == "back") return;
            if (termStr == "FIRST_TERM" || termStr == "SECOND_TERM") break;
            cout << "�߸��� �б��Դϴ�. �ٽ� �Է��ϼ���.\n";
        }

        // ��� ���� ID �Է� �� ��ȿ�� �˻�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";

            // ���� ��� ǥ��
            viewProfessors(users);

            cout << "��� ���� ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, professorID);
            if (professorID == "back") return;

            // ���� ID�� �����ϰ�, ������ Professor���� Ȯ��
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
                cout << "��ȿ���� ���� ���� ID�Դϴ�. �����ϴ� ������ ID�� �Է��ϼ���.\n";
            }
        }

        // �б� Ÿ�� ��ȯ
        Term term = (termStr == "FIRST_TERM") ? Term::FIRST_TERM : Term::SECOND_TERM;

        // ���� ���� �� �߰�
        subjects.emplace_back(Subject(id, name, credit, type, year, term, professorID));
        cout << "\"" << name << "\" ������ �����Ǿ����ϴ�.\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewSubjects(subjects);

        // �߰� ���� �� ���� ����
        break;
    }
}


void Administrator::deleteSubject(vector<Subject>& subjects) {
    int id;
    viewSubjects(subjects);

    cout << "-------------------------------------------------------------------------------------\n";
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
            cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: \n";
        }
    }
    cin.ignore();

    auto it = find_if(subjects.begin(), subjects.end(),
        [id](const Subject& s) { return s.getID() == id; });
    if (it != subjects.end()) {
        cout << "\"" << it->getName() << "\" ������ �����Ǿ����ϴ�.\n";
        subjects.erase(it);
        cout << "-------------------------------------------------------------------------------------\n";
        viewSubjects(subjects);
    }
    else {
        cout << "������ ã�� �� �����ϴ�.\n";
    }
}

// Modify Subject
void Administrator::modifySubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users) {
    while (true) { // ��ü ���� ���� ������ �ݺ�
        viewSubjects(subjects);
        cout << "-------------------------------------------------------------------------------------\n";
        int id;

        // ���� ID �Է� �� ��ȿ�� �˻� ����
        while (true) {
            cout << "������ ���� ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            string input;
            cin >> input;

            // 'back' �Է� �� �Լ� ����
            string lowerInput = input;
            // �ҹ��ڷ� ��ȯ
            for (auto& c : lowerInput) c = tolower(c);
            if (lowerInput == "back") return;

            try {
                id = stoi(input);
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���.\n";
                continue; // �ٽ� �Է� ��û
            }

            // �Է��� ID�� �����ϴ��� Ȯ��
            auto it = find_if(subjects.begin(), subjects.end(),
                [id](Subject& s) { return s.getID() == id; });

            if (it != subjects.end()) {
                // ������ ������ ��� ���� ���μ����� �̵�
                // ������ ���� ���� ���
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "���� ���� ����:\n";
                cout << "ID: " << it->getID() << "\n";
                cout << "�̸�: " << it->getName() << "\n";
                cout << "����: " << it->getType() << "\n";
                cout << "����: " << it->getCredit() << "\n";
                cout << "�⵵: " << it->getYear() << "\n";
                cout << "�б�: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << "\n";
                cout << "��� ���� ID: " << it->getProfessorID() << "\n";
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "///////'pass'�� �Է��ϸ� �������� ����///////" << endl;
                cout << "-------------------------------------------------------------------------------------\n";

                // ���� �̸� ����
                string newName;
                while (true) {
                    cout << "�� ���� �̸��� �Է��ϼ��� (����: " << it->getName() << "): ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���� cin >> input���� ���� ���� ���� ����
                    getline(cin, newName);

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = newName.find_first_not_of(" \t\n\r");
                    size_t end = newName.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newName = newName.substr(start, end - start + 1);
                    else
                        newName = "";

                    // �ҹ��� ��ȯ
                    string lowerNewName = newName;
                    for (auto& c : lowerNewName) c = tolower(c);

                    if (lowerNewName == "back") return;
                    if (lowerNewName == "pass") {
                        newName = it->getName(); // �������� ����
                        break;
                    }
                    if (!newName.empty() && isValidName(newName)) break;
                    cout << "���� �̸��� ����� �� ������, ���ڰ� ���Ե� �� �����ϴ�. �ٽ� �Է��ϼ���.\n";
                }

                // ���� ���� ����
                string newType;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "�� ���� ������ �Է��ϼ��� (����: " << it->getType() << ", Required/Elective/Basic �Է�): ";
                    getline(cin, newType);

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = newType.find_first_not_of(" \t\n\r");
                    size_t end = newType.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newType = newType.substr(start, end - start + 1);
                    else
                        newType = "";

                    // �ҹ��� ��ȯ
                    string lowerNewType = newType;
                    for (auto& c : lowerNewType) c = tolower(c);

                    if (lowerNewType == "back") return;
                    if (lowerNewType == "pass") {
                        newType = it->getType(); // �������� ����
                        break;
                    }
                    if (newType == "Required" || newType == "Elective" || newType == "Basic") break;
                    cout << "�߸��� �����Դϴ�. �ٽ� �Է��ϼ���.\n";
                }

                // ���� ����
                double newCredit;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "�� ������ �Է��ϼ��� (����: " << it->getCredit() << "����,1 OR 2 OR 3 �� �ϳ��� �Է�): ";
                    string creditInput;
                    cin >> creditInput;

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = creditInput.find_first_not_of(" \t\n\r");
                    size_t end = creditInput.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        creditInput = creditInput.substr(start, end - start + 1);
                    else
                        creditInput = "";

                    // �ҹ��� ��ȯ
                    string lowerCreditInput = creditInput;
                    for (auto& c : lowerCreditInput) c = tolower(c);

                    if (lowerCreditInput == "back") return;
                    if (lowerCreditInput == "pass") {
                        newCredit = it->getCredit(); // �������� ����
                        break;
                    }
                    try {
                        newCredit = stod(creditInput);
                        if (newCredit == 1 || newCredit == 2 || newCredit == 3) break;
                        cout << "��ȿ���� ���� �����Դϴ�. 1, 2 Ȥ�� 3���� �� �ϳ��� �Է��ϼ���.\n";
                    }
                    catch (...) {
                        cout << "��ȿ���� ���� �Է��Դϴ�. ���ڸ� �Է��ϼ���.\n";
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �Է� ���� ����

                // �⵵ ����
                int newYear;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "�� �⵵�� �Է��ϼ��� (����: " << it->getYear() << "): ";
                    string yearInput;
                    cin >> yearInput;

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = yearInput.find_first_not_of(" \t\n\r");
                    size_t end = yearInput.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        yearInput = yearInput.substr(start, end - start + 1);
                    else
                        yearInput = "";

                    // �ҹ��� ��ȯ
                    string lowerYearInput = yearInput;
                    for (auto& c : lowerYearInput) c = tolower(c);

                    if (lowerYearInput == "back") return;
                    if (lowerYearInput == "pass") {
                        newYear = it->getYear(); // �������� ����
                        break;
                    }
                    try {
                        newYear = stoi(yearInput);
                        if (newYear > 0) break;
                        cout << "��ȿ���� ���� �����Դϴ�. ���� ������ �Է��ϼ���.\n";
                    }
                    catch (...) {
                        cout << "��ȿ���� ���� �Է��Դϴ�. ������ �Է��ϼ���.\n";
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �Է� ���� ����

                // �б� ����
                string newTermStr;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";
                    cout << "�б⸦ �Է��ϼ��� (����: " << (it->getTerm() == Term::FIRST_TERM ? "FIRST_TERM" : "SECOND_TERM") << ", FIRST_TERM/SECOND_TERM �Է�): ";
                    getline(cin, newTermStr);

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = newTermStr.find_first_not_of(" \t\n\r");
                    size_t end = newTermStr.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newTermStr = newTermStr.substr(start, end - start + 1);
                    else
                        newTermStr = "";

                    // �ҹ��� ��ȯ
                    string lowerNewTermStr = newTermStr;
                    for (auto& c : lowerNewTermStr) c = tolower(c);

                    if (lowerNewTermStr == "back") return;
                    if (lowerNewTermStr == "pass") {
                        newTermStr = (it->getTerm() == Term::FIRST_TERM) ? "FIRST_TERM" : "SECOND_TERM"; // �������� ����
                        break;
                    }
                    if (newTermStr == "FIRST_TERM" || newTermStr == "SECOND_TERM") break;
                    cout << "�߸��� �б��Դϴ�. �ٽ� �Է��ϼ���.\n";
                }

                // ��� ���� ID �Է� �� ��ȿ�� �˻�
                string newProfessorID;
                while (true) {
                    cout << "-------------------------------------------------------------------------------------\n";

                    // ���� ��� ǥ��
                    viewProfessors(users);

                    cout << "��� ���� ID�� �Է��ϼ���: ";
                    getline(cin, newProfessorID);

                    // �Է°� ��ó��: �յ� ���� ����
                    size_t start = newProfessorID.find_first_not_of(" \t\n\r");
                    size_t end = newProfessorID.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                        newProfessorID = newProfessorID.substr(start, end - start + 1);
                    else
                        newProfessorID = "";

                    // �ҹ��� ��ȯ
                    string lowerProfessorID = newProfessorID;
                    for (auto& c : lowerProfessorID) c = tolower(c);

                    if (lowerProfessorID == "back") return;
                    if (lowerProfessorID == "pass") {
                        newProfessorID = it->getProfessorID(); // �������� ����
                        break;
                    }

                    // ���� ID�� �����ϰ�, ������ Professor���� Ȯ��
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
                        cout << "��ȿ���� ���� ���� ID�Դϴ�. �����ϴ� ������ ID�� �Է��ϼ���.\n";
                    }
                }

                // �б� Ÿ�� ��ȯ
                Term newTerm;
                if (newTermStr == "FIRST_TERM") {
                    newTerm = Term::FIRST_TERM;
                }
                else { // "SECOND_TERM" �� ���
                    newTerm = Term::SECOND_TERM;
                }

                // ���� ���� ����
                it->setName(newName);
                it->setType(newType);
                it->setCredit(newCredit);
                it->setYear(newYear);
                it->setTerm(newTerm);
                it->setProfessorID(newProfessorID);

                cout << "�ش� ������ �����Ǿ����ϴ�.\n";
                cout << "-------------------------------------------------------------------------------------\n";
                viewSubjects(subjects);

                return; // ���� �Ϸ� �� �Լ� ����
            }
            else {
                cout << "������ ã�� �� �����ϴ�.\n";
                // �ٽ� ���� ID�� �Է¹ޱ� ���� ������ ����մϴ�.
            }
        }
    }
}


void Administrator::viewSubjects(const vector<Subject>& subjects) const {
    cout << "���� ��ϵ� ���� ���: \n";
    // ���� �̸��� �ִ� ���̸� ����Ͽ� �� �ʺ� ����
    size_t maxNameLength = 30; // �⺻ �̸� �� �ʺ� 15���� ����
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
    cout << "���� ���: \n";
    size_t maxNameLength = 15; // �⺻ �̸� �� �ʺ� 15���� ����
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
    cout << "���� ID�� �Է��ϼ���: ";
    cin >> profID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Professor" && user->getID() == profID) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "=== ���� ���� ===\n";
            cout << "ID: " << user->getID() << "\n";
            cout << "�̸�: " << user->getName() << "\n";
            cout << "��ȭ��ȣ: " << user->getPhoneNumber() << "\n";
            cout << "�̸���: " << user->getEmail() << "\n";
            cout << "-------------------------------------------------------------------------------------\n\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
    }
}
void Administrator::addProfessor(vector<unique_ptr<User>>& users) {
    while (true) { // ���� �߰� ������ �ݺ�
        string id, name, phone, email;

        viewProfessors(users);
        cout << "-------------------------------------------------------------------------------------\n";

        // ID �Է�
        while (true) {
            cout << "ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            cin >> id;
            if (id == "back") return; // ���� �޴��� ���ư���

            // ID �ߺ� üũ
            bool idExists = false;
            for (const auto& user : users) {
                if (user->getID() == id) {
                    cout << "����: �̹� �����ϴ� ID�Դϴ�. �ٸ� ID�� �Է��ϼ���.\n";
                    idExists = true;
                    break;
                }
            }
            if (!idExists) break; // �ߺ����� �ʴ� ID��� ���� ����
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // �̸� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�̸��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, name);
            if (name == "back") return; // ���� �޴��� ���ư���
            if (!name.empty() && isValidName(name)) break;
            cout << "�̸��� ����� �� ������, ���ڰ� ���Ե� �� �����ϴ�. �ٽ� �Է��ϼ���: \n";
        }

        // ��ȭ��ȣ �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "��ȭ��ȣ�� �Է��ϼ��� [010-xxxx-xxxx ����] ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, phone);
            if (phone == "back") return; // ���� �޴��� ���ư���

            // ��ȭ��ȣ ���� üũ
            regex phonePattern("^010-\\d{4}-\\d{4}$");
            if (!regex_match(phone, phonePattern)) {
                cout << "��ȿ���� ���� �����Դϴ�. [010-xxxx-xxxx] �������� �ٽ� �Է��ϼ���.\n";
                continue;
            }

            // ��ȭ��ȣ �ߺ� üũ
            bool phoneExists = false;
            for (const auto& user : users) {
                if (user->getPhoneNumber() == phone) {
                    cout << "����: �̹� �����ϴ� ��ȭ��ȣ�Դϴ�. �ٸ� ��ȭ��ȣ�� �Է��ϼ���.\n";
                    phoneExists = true;
                    break;
                }
            }
            if (!phoneExists) break; // �ߺ����� �ʴ� ��ȭ��ȣ��� ���� ����
        }

        // �̸��� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�̸����� �Է��ϼ��� [��: example@cau.ac.kr] ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, email);
            if (email == "back") return; // ���� �޴��� ���ư���

            // �̸��� ���� üũ
            regex emailPattern("^[a-zA-Z0-9._%+-]+@cau\\.ac\\.kr$");
            if (regex_match(email, emailPattern)) {
                break; // �ùٸ� �̸��� �����̶�� ���� ����
            }
            else {
                cout << "��ȿ���� ���� �̸��� �����Դϴ�. [��: example@cau.ac.kr] �ٽ� �Է��ϼ���.\n";
            }
        }

        /////////////////////////////////////////////////////////////////////////////////
            // ���� ��й�ȣ�� "0000"���� �����ϵ�, 
            // ���� ���Ͽ���0000�� �ƴ�, ���ڰ� ������ ��� 
            // �ɼ� -> ������ -> �ڵ������� ��ȯ üũ ���� �� ��
            //
            //
            //
            ///////////////////////////////////////////////////////////////////////////////////

        string password = "0000";

        // ���� ��ü ���� �� �߰�
        unique_ptr<User> newProf = make_unique<Professor>(name, id, password, phone, email);
        addUser(users, move(newProf));

        cout << "\"" << name << "\" ������ �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);

        // ���� �߰� �� ���� ���� (���ϴ� ��� ��� �߰��� �� �ֵ��� ���� ����)
        break;
    }
}


void Administrator::deleteProfessor(vector<unique_ptr<User>>& users) {
    string profID;
    viewProfessors(users);
    cout << "-------------------------------------------------------------------------------------\n";
    cout << "������ ���� ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
    while (true) {
        cin >> profID;
        if (profID == "back") return; // ���� �޴��� ���ư���
        if (cin) break;
        cout << "��ȿ���� ���� �Է��Դϴ�. ���� ID�� �Է��ϼ���: \n";
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
        cout << "-------------------------------------------------------------------------------------\n";
        viewProfessors(users);
    }
    else {
        cout << "�ش� ID�� ������ ã�� �� �����ϴ�.\n";
    }
}

// Student Management

void Administrator::viewStudents(const vector<unique_ptr<User>>& users) {
    cout << "�л� ���: \n";
    size_t maxEmailLength = 30; // �⺻ �̸� �� �ʺ� 30���� ����
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
    cout << "�л� ID�� �Է��ϼ���: ";
    cin >> studentID;

    bool found = false;
    for (const auto& user : users) {
        if (user->getUserType() == "Student" && user->getID() == studentID) {
            const Student* student = dynamic_cast<const Student*>(user.get());
            if (student) {
                cout << "-------------------------------------------------------------------------------------\n";
                cout << "=== �л� ���� ===\n";
                cout << "ID: " << student->getID() << "\n";
                cout << "�̸�: " << student->getName() << "\n";
                cout << "��ȭ��ȣ: " << student->getPhoneNumber() << "\n";
                cout << "�̸���: " << student->getEmail() << "\n";
                cout << "�й�: " << student->getStudentID() << "\n";
                cout << "-------------------------------------------------------------------------------------\n\n";
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
    while (true) { // �л� �߰� ������ �ݺ�
        string name, id, phone, email;
        int studentID;

        viewStudents(users);
        cout << "-------------------------------------------------------------------------------------\n";

        // ID �Է�
        while (true) {
            cout << "ID�� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            cin >> id;
            if (id == "back") return; // ���� �޴��� ���ư���

            // ID �ߺ� üũ
            bool idExists = false;
            for (const auto& user : users) {
                if (user->getID() == id) {
                    cout << "����: �̹� �����ϴ� ID�Դϴ�. �ٸ� ID�� �Է��ϼ���.\n";
                    idExists = true;
                    break;
                }
            }
            if (!idExists) break; // �ߺ����� �ʴ� ID��� ���� ����
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // �̸� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�̸��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, name);
            if (name == "back") return; // ���� �޴��� ���ư���
            if (!name.empty() && isValidName(name)) break;
            cout << "�̸��� ����� �� ������, ���ڰ� ���Ե� �� �����ϴ�. �ٽ� �Է��ϼ���: \n";
        }

        // ��ȭ��ȣ �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "��ȭ��ȣ�� �Է��ϼ��� [010-xxxx-xxxx ����] ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, phone);
            if (phone == "back") return; // ���� �޴��� ���ư���

            // ��ȭ��ȣ ���� üũ
            regex phonePattern("^010-\\d{4}-\\d{4}$");
            if (!regex_match(phone, phonePattern)) {
                cout << "��ȿ���� ���� �����Դϴ�. [010-xxxx-xxxx] �������� �ٽ� �Է��ϼ���.\n";
                continue;
            }

            // ��ȭ��ȣ �ߺ� üũ
            bool phoneExists = false;
            for (const auto& user : users) {
                if (user->getPhoneNumber() == phone) {
                    cout << "����: �̹� �����ϴ� ��ȭ��ȣ�Դϴ�. �ٸ� ��ȭ��ȣ�� �Է��ϼ���.\n";
                    phoneExists = true;
                    break;
                }
            }
            if (!phoneExists) break; // �ߺ����� �ʴ� ��ȭ��ȣ��� ���� ����
        }

        // �̸��� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�̸����� �Է��ϼ��� [��: example@cau.ac.kr] ('back'�� �Է��� ���� �޴��� ���ư���): ";
            getline(cin, email);
            if (email == "back") return; // ���� �޴��� ���ư���

            // �̸��� ���� üũ
            regex emailPattern("^[a-zA-Z0-9._%+-]+@cau\\.ac\\.kr$");
            if (regex_match(email, emailPattern)) {
                break; // �ùٸ� �̸��� �����̶�� ���� ����
            }
            else {
                cout << "��ȿ���� ���� �̸��� �����Դϴ�. [��: example@cau.ac.kr] �ٽ� �Է��ϼ���.\n";
            }
        }

        // �й� �Է�
        while (true) {
            cout << "-------------------------------------------------------------------------------------\n";
            cout << "�й��� �Է��ϼ��� ('back'�� �Է��� ���� �޴��� ���ư���): ";
            string input;
            cin >> input;
            if (input == "back") return; // ���� �޴��� ���ư���

            try {
                studentID = stoi(input);
                break;
            }
            catch (...) {
                cout << "��ȿ���� ���� �Է��Դϴ�. ���� �й��� �Է��ϼ���.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        /////////////////////////////////////////////////////////////////////////////////
       // ���� ��й�ȣ�� "0000"���� �����ϵ�, 
       // ���� ���Ͽ���0000�� �ƴ�, ���ڰ� ������ ��� 
       // �ɼ� -> ������ -> �ڵ������� ��ȯ üũ ���� �� ��
       //
       //
       //
       ///////////////////////////////////////////////////////////////////////////////////

        string password = "0000";

        // �л� ��ü ���� �� �߰�
        unique_ptr<User> newStudent = make_unique<Student>(name, id, password, phone, email, studentID);
        addUser(users, move(newStudent));

        cout << "\"" << name << "\" �л��� �߰��Ǿ����ϴ�. (���� ��й�ȣ�� 0000�Դϴ�.)\n";
        cout << "-------------------------------------------------------------------------------------\n";
        viewStudents(users);

        // �л� �߰� �� ���� ����
        break;
    }
}





void Administrator::deleteStudent(vector<unique_ptr<User>>& users) {
    viewStudents(users);
    string studentID;

    cout << "-------------------------------------------------------------------------------------\n";
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
        cout << "-------------------------------------------------------------------------------------\n";
        viewStudents(users);
    }
    else {
        cout << "�ش� ID�� �л��� ã�� �� �����ϴ�.\n";
    }
}
