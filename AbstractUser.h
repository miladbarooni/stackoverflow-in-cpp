#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "Content.h"
#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)
using namespace std;

enum UserType {
    ADMIN,
    MEMBER
};

class User;

class AbstractUser {
public:
    hash<string> pass_hash;
    friend void write_on_log_file(string file_name, User* user);
    virtual bool authenticate(string username, string password) = 0;
    virtual void deleteAccount() = 0;
    string username;
protected:
    string password;
    string email;
    UserType type;
    vector<Content> contents;
};
