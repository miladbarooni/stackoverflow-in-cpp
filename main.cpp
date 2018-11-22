#include <iostream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include <fstream>
#include <vector>

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif


using namespace std;


enum MenuState {
    START,
    LOGGED_IN,
    END,
    CONTENT
};

int id_counter = 1;

int main() {
    cout << currentDateTime()<<endl;
    // vector for questions
    vector<Content> questions;
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
                cout << "c.contents\nd. delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'd': {
                        try {
                            loggedInUser->deleteAccount();
                            cout << "Account successfully deleted\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
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
            }
            case MenuState :: CONTENT:{
                cout << "c. create question\na. answer to questions\nm. mark as duplicate(Admin only)\nu. update questions\nd. delete questions\n";
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
                                Content content(answer,ContentType::ANSWER,0);
                                ContentRelation content_relation(content, ContentRelationType::ANSWER_TO);
                                question.addToRelations(content_relation);
                            }

                        }
                        break;
                    }

                    case('u'):
                    {

                    }

                }
            }
        }
    }
    system(CLEAR);
    cout << "GOODBYE" << endl;
    return 0;
}
