// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>
#include <string>
#include "User.h"
#include "Subject.h"
#include "StudentRecord.h"

// Forward declaration to avoid circular dependency
class Student;

// Function declarations
std::vector<std::unique_ptr<User>> loadUsers(const std::string& filename);
std::vector<Subject> loadSubjects(const std::string& filename);
std::vector<StudentRecord> loadStudentRecords(const std::string& filename);
void updateStudentRecordsCSV(const std::vector<Student*>& students);
void saveUsers(const std::string& filename, const std::vector<std::unique_ptr<User>>& users);
void saveSubjects(const std::string& filename, const std::vector<Subject>& subjects);


// Helper function
Subject* findSubject(std::vector<Subject>& subjects, int subjectId);

// User login function
User* login(std::vector<std::unique_ptr<User>>& users);

// Display menu based on user type
void displayMenu(const std::string& userType);

// Declaration of displayHistogram
void displayHistogramForSubject(const vector<StudentRecord>& records, int subjectID);
// Function to mask password input
std::string getMaskedInput();

// Function to update Users.csv file
void updateUsersCSV(const vector<User*>& users);

vector<StudentRecord> filterRecordsBySubject(const vector<StudentRecord>& records, int subjectID);
#endif // UTILS_H
