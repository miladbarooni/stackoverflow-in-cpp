#pragma once

#include <iostream>
#include "AbstractUser.h"

class User : public AbstractUser {
public:
    User(string username, string password, string email, UserType type);
    friend void write_on_log_file(string log_file,User* loggedInUser);
    static void init(const string &salt);
    void addContent(Content content);
    int printQuestions();
    void changeQuestion(int index, Content new_question);
    void deleteQuestion(int index);
    string getEmail();
public:
    void set_password(string password);
    bool check_password(string password);

public:
    bool authenticate(string username, string password);
    void deleteAccount();

public:
    static User& login(string username, string password);
    static User& signup(string username, string password, string email);
    vector<Content> contents;// show all contents of a user
private:
    static string salt;
    static vector<User> users;

};
