#include <iostream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include <fstream>
#include <vector>
#include <ctime>


#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

using namespace std;



vector<Content> questions;
// use for update a question
void change_questions(Content& new_Q, int index, User* logInUser)
{
    string question_string = logInUser->contents[index-1].getBody();
    for (auto &question:questions)
    {
        if (question_string == question.getBody())
        {
            question = new_Q;
        }
    }
// now time to reset id ( where clean code goes true :D)
    int id =1;
    for (auto &question:questions)
    {
        question.setId(id);
        id++;
    }

}
// use for deleting a question
void delete_from_questions(int index, User* logInUser)
{
    string question_string = logInUser->contents[index-1].getBody();
    for (auto i = questions.begin(); i<questions.end(); i++)
    {
        if (question_string == i->getBody())
        {
            questions.erase(i);
        }
    }
    int id =1;
    for (auto &question:questions)
    {
        question.setId(id);
        id++;
    }
}


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/loggedInUsertime format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


void write_on_log_file(string file_name, User* user)
{
    fstream log_file;
    log_file.open (file_name, ios::app);
    string log = user->email +  "\t" + user->username + "\t" + currentDateTime();
    log_file << log;
    log_file.close();
}
enum MenuState {
    START,
    LOGGED_IN,
    END,
    CONTENT
};

int id_counter = 1;

int main() {
    string str;
    fstream times_run;
    times_run.open("AppRunTimes.txt", ios::in);
    times_run >> str;
    times_run.close();
    int times = stoi(str) + 1;
    /////////////////////////
    string myLogFile = "log." + str + ".txt"; // This will make the log file name log.1.txt , log.2.txt ...
    // now times to add new times to AppRunTimes.txt
    str = to_string(times);
    times_run.open("AppRunTimes.txt", ios::out);
    times_run << str;
    times_run.close();
    // finish

    User::init("SECRET_KEY");
    User * loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
    string last_message;

    char choice;
    while(menuState != MenuState::END) {
        system(CLEAR);
        if (!last_message.empty())
            cout << last_message << endl;
        last_message = "";
        switch (menuState) {
            case MenuState::START: {
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case '1': { // login
                        try {
                            string username, password;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::login(username,password);
                            menuState = MenuState::LOGGED_IN;
                            write_on_log_file(myLogFile, loggedInUser);
                        } catch (WrongUsernameOrPasswordException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case '2': { // signup
                        try {
                            string username, password, email;
                            cout << "Enter Email: ";
                            cin >> email;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::signup(username, password, email);
                            menuState = MenuState::LOGGED_IN;
                            last_message = "User signed up!\n";
                        } catch (UsernameAlreadyExistsException &e) {
                            last_message = e.what();
                            break;

                        } catch (EmailAlreadyExistsException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "c. contents\nd. delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'd': {
                        try {
                            loggedInUser->deleteAccount();
                            cout << "Account successfully deleted\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                            break;
                        }
                        catch (DeleteAdminException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        last_message = "GOOD BYE\n";
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    case 'c':{
                        menuState = MenuState::CONTENT;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }

                }
                break;
            }
            case MenuState :: CONTENT:{
                cout << "c. create question\na. answer to questions\nm. mark as duplicate(Admin only)\nu. update questions\nd. delete questions\nb. back to user homepage\n";
                cin >> choice;
                switch(choice){
                    case ('c'):
                    {
                        cout << "Please Enter your question:\n";
                        string question;
                        cin >> question;
                        Content content(question, ContentType::QUESTION, id_counter++);
                        loggedInUser->addContent(content);
                        questions.push_back(content);
                        cout<< "question added to contents"<<endl;
                        menuState = MenuState ::LOGGED_IN;
                        break;

                    }

                    case ('a'):
                    {
                        int question_number;
                        string answer;
                        for (auto &question : questions)
                        {
                            cout << question.getId() << ". " << question.getBody()<< endl;
                        }
                        cout << "\nPlease Enter your question number:";
                        cin >> question_number;

                        for (auto &question : questions)
                        {
                            if (question.getId() == question_number)
                            {
                                question.addVisits();
                                cout << "q. " << question.getBody() << "\t(visits: " << question.getVisits() << ")" << endl;
                                question.printAnswers();
                                cout << endl << "Please enter your answer: ";
                                cin >> answer;
                                Content answer_content(answer,ContentType::ANSWER, 0);
                                ContentRelation content_relation(answer_content, ContentRelationType::ANSWER_TO);
                                //cout << content_relation.getDestination()->getBody()<< endl;
                                question.addToRelations(content_relation);
                            }

                        }
                        break;
                    }

                    case('u'): {
                        int chosen_question;
                        string new_question;
                        system("clear");

                        int flag = loggedInUser->printQuestions();
                        if (flag == 0)
                        {
                            menuState = MenuState ::LOGGED_IN;
                            break;
                        }
                        cout << "Your Questions are: " << endl;
                        cout << "Which question do you want to update (1,2,..): ";
                        cin >> chosen_question;
                        cout << "Please enter edited question: " ;
                        cin >> new_question;
                        Content new_Q(new_question, ContentType::QUESTION , 0);
                        change_questions(new_Q, chosen_question, loggedInUser);
                        loggedInUser->changeQuestion(chosen_question, new_Q);
                        break;

                    }
                    case ('d'):
                    {
                        int chosen_question;
                        string new_question;
                        system("clear");

                        int flag = loggedInUser->printQuestions();
                        if (flag == 0)
                        {
                            menuState = MenuState ::LOGGED_IN;
                            break;
                        }
                        cout << "Your Questions are: " << endl;
                        cout << "Which question do you want to delete (1,2,..): ";
                        cin >> chosen_question;
                        delete_from_questions(chosen_question, loggedInUser);
                        loggedInUser->deleteQuestion(chosen_question);
                        break;
                    }
                    case('b'):
                    {
                        menuState = MenuState :: LOGGED_IN;
                        break;
                    }

                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
        }
    }
    system(CLEAR);
    cout << "GOODBYE" << endl;
    return 0;
}
