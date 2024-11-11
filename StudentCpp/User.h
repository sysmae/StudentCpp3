// User.h
#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

// Forward declarations
class User {
protected:
    string id;
    string password;
    string name;
    string phoneNumber;
    string email;
    string userType;

public:
    User(string n, string i, string p, string phone, string mail, string type);
    virtual ~User(); // Virtual destructor for proper cleanup

    string getID() const;
    string getName() const;
    string getPassword() const;
    string getEmail() const;
    string getPhoneNumber() const;

    void setName(string n);
    void setPassword(string p);
    void setEmail(string mail);
    void setPhoneNumber(string phone);

    virtual string getUserType() const = 0; // Pure virtual function
};

#endif // USER_H
