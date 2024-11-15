// Administrator.h
#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include "Subject.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class Administrator : public User {
public:
    Administrator(string n, string i, string p, string phone, string mail);

    string getUserType() const override;

    void addUser(vector<unique_ptr<User>>& users, unique_ptr<User> newUser);
    void removeUser(vector<unique_ptr<User>>& users, const string& userID);
    void viewAllUsers(const vector<unique_ptr<User>>& users) const;

    // Subject management functions
    void addSubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users);
    void deleteSubject(vector<Subject>& subjects);
    void modifySubject(vector<Subject>& subjects, const vector<unique_ptr<User>>& users);
    void viewSubjects(const vector<Subject>& subjects) const;

    // Professor management functions
    void viewProfessors(const vector<unique_ptr<User>>& users);
    void viewProfessorInfo(const vector<unique_ptr<User>>& users) const;
    void addProfessor(vector<unique_ptr<User>>& users);
    void deleteProfessor(vector<unique_ptr<User>>& users);

    // Student management functions
    void viewStudents(const vector<unique_ptr<User>>& users);
    void viewStudentInfo(const vector<unique_ptr<User>>& users) const;
    void addStudent(vector<unique_ptr<User>>& users);
    void deleteStudent(vector<unique_ptr<User>>& users);
};

#endif // ADMINISTRATOR_H
