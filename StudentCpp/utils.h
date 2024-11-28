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
void updateStudentRecordsCSV(const vector<StudentRecord>& updatedRecords);
void saveUsers(const std::string& filename, const std::vector<std::unique_ptr<User>>& users);
void saveSubjects(const std::string& filename, const std::vector<Subject>& subjects);
void updateUsersCSV(const std::vector<User*>& users);

// Helper function
Subject* findSubject(std::vector<Subject>& subjects, int subjectId);

// User login function
User* login(std::vector<std::unique_ptr<User>>& users);

// Display menu based on user type
void displayMenu(const std::string& userType);

// Declaration of displayHistogram
void displayHistogramForSubject(const std::vector<StudentRecord>& records, int subjectID);

// Function to mask password input
std::string getMaskedInput();

// Function to filter records by subject
std::vector<StudentRecord> filterRecordsBySubject(const std::vector<StudentRecord>& records, int subjectID);

// Function to validate name
bool isValidName(const std::string& name);

// Function to print table headers
void printTableHeader(size_t nameWidth);
void printTableHeaderProfessor(size_t nameWidth);
void printTableHeaderStudent(size_t nameWidth);

#endif // UTILS_H