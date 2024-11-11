// User.cpp
#include "User.h"

User::User(string n, string i, string p, string phone, string mail, string type)
    : name(n), id(i), password(p), phoneNumber(phone), email(mail), userType(type) {}

User::~User() {} // Virtual destructor

string User::getID() const { return id; }
string User::getName() const { return name; }
string User::getPassword() const { return password; }
string User::getEmail() const { return email; }
string User::getPhoneNumber() const { return phoneNumber; }

void User::setName(string n) { name = n; }
void User::setPassword(string p) { password = p; }
void User::setEmail(string mail) { email = mail; }
void User::setPhoneNumber(string phone) { phoneNumber = phone; }
