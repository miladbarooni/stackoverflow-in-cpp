#include <iostream>
#include <string>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include <fstream>
#include <vector>
#include <ctime>
//#include <adoctint.h>

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

using namespace std;

class LogData {
public:

    static LogData* getLogData() {
        if (a == NULL)a = new LogData();

        return a;
    }
    int v;
    void init() {
        ifstream i0("logcounter.txt");
        if (!i0.is_open()) {
            ofstream i22("logcounter.txt");
            i22 << 1;
            i22.close();

        }
        else {
            i0.close();
        }

    }
    void fillx() {
        ifstream i1("logcounter.txt");
        i1.seekg(0);
        i1 >> v;
        i1.close();
    }
    void setx() {
        string temp = to_string(v);
        name = name + temp + txt;
    }
    void addx() {
        ofstream i2("logcounter.txt");
        i2.clear();
        i2 << v + 1;
        i2.close();
    }
    void writelog(User* a) {
        ofstream i3(name, ios_base::app);
        time_t now = time(0);
        char* dt = ctime(&now);
        i3 << a->getEmail()<<"   "<<a->username<< "   "<< dt;
        i3.close();
    }
private:
    string name;
    string txt;
    LogData( string s = "log", string s22=".txt") :txt(s22), name(s) {}
    static LogData* a;
};

LogData* LogData::a = NULL;


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


//void write_on_log_file(string file_name, User* user)
//{
//    fstream log_file;
//    log_file.open (file_name, ios::app);
//    string log = user->email +  "\t" + user->username + "\t" + currentDateTime();
//    log_file << log;
//    log_file.close();
//}
//enum MenuState {
//    START,
//    LOGGED_IN,
//    END,
//    CONTENT
//};

enum MenuState {
    START,
    LOGGED_IN,
    END,
    CONTENT
};

int id_counter = 1;

int main() {


    LogData *d = LogData::getLogData();//in 5 khat avale main copy she
    d->init();
    d->fillx();
    d->setx();
    d->addx();

    //d->writelog(User* a);inam harja khast log benevise call she
    //d->writelog(User* a);
//
//    string str;
//    fstream times_run;
//    times_run.open("AppRunTimes.txt", ios::in);
//    times_run >> str;
//    times_run.close();
//    int times = stoi(str) + 1;
//    /////////////////////////
//    string myLogFile = "log." + str + ".txt"; // This will make the log file name log.1.txt , log.2.txt ...
//    // now times to add new times to AppRunTimes.txt
//    str = to_string(times);
//    times_run.open("AppRunTimes.txt", ios::out);
//    times_run << str;
//    times_run.close();
    // finish
    vector<Content> questions;
    User::init("SECRET_KEY");
    User *loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
    string last_message;

    char choice;
    while (menuState != MenuState::END) {

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
                            loggedInUser = &User::login(username, password);
                            menuState = MenuState::LOGGED_IN;
                            //write_on_log_file(myLogFile, loggedInUser);
                            d->writelog(loggedInUser); // added //
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
                            d->writelog(loggedInUser); // added //
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
                    case 'c': {
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
            case MenuState::CONTENT: {
                if (loggedInUser->username == "admin")
                    cout << "c. create question\na. answer to questions\nm. mark as duplicate(Admin only)\nu. update questions\nd. delete questions\nb. back to user homepage\n";
                else
                    cout << "c. create question\na. answer to questions\nu. update questions\nd. delete questions\nb. back to user homepage\n";

                cin >> choice;
                switch (choice) {
                    case ('c'): {
                        cout << "Please Enter your question:\n";
                        string input;
                        cin >> input;
                        //cin.getline(input, sizeof(input));
                        Content content(input, ContentType::QUESTION, id_counter++);
                        loggedInUser->addContent(content);
                        questions.push_back(content);
                        cout << "question added to contents" << endl;
                        menuState = MenuState::LOGGED_IN;
                        break;

                    }

                    case ('a'): {
                        int question_number;

                        for (auto &question : questions) {
                            cout << question.getId() << ". " << question.getBody();
                            if (question.isDuplicate) cout << " (duplicate of question with ID: " << question.duplicateId << ")";
                            cout << endl;

                        }
                        cout << "\nPlease Enter your question number:";
                        cin >> question_number;

                        for (auto &question : questions) {
                            if (question.getId() == question_number) {
                                question.addVisits();
                                cout << "q. " << question.getBody() << "\t(visits: " << question.getVisits() << ")";
                                cout << endl;
                                question.printAnswers();
                                cin.ignore();
                                cout << endl << "Please enter your answer: ";
                                char input[100];
                                cin.getline(input, sizeof(input));
                                Content content(input, ContentType::ANSWER, 0);
                                question.answers.push_back(content);
                                ContentRelation content_relation(content, ContentRelationType::ANSWER_TO);
                                question.addToRelations(content_relation);
                            }

                        }
                        break;
                    }

                    case ('u'): {
                        int chosen_question;
                        system(CLEAR);

                        int flag = loggedInUser->printQuestions();

                        if (flag == 0)
                        {
                            last_message = "There are no questions!\n";
                            menuState = MenuState ::LOGGED_IN;
                            break;
                        }
                        cout << "Your Questions are: " << endl;
                        cout << "Which question do you want to update (1,2,..): ";
                        cin >> chosen_question;
                        if (chosen_question > flag)
                        {
                            last_message = "Invalid number!\n";
                            break;
                        }
                        cin.ignore();
                        cout << "Please enter edited question: " ;
                        char new_question[100];
                        cin.getline(new_question, sizeof(new_question));


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

                    case ('b'):
                        menuState = MenuState::LOGGED_IN;
                        break;
                    case ('m'):
                    {
                        int choice1, choice2;
                        int size = (int)questions.size();
                        if (loggedInUser->username == "admin") {
                            for (auto &question : questions)
                                cout << question.getId() << ". " << question.getBody() << endl;

                                cout << "Choose which one to be duplicate of: " ;
                                cin >> choice1;
                                while (choice1 > size)
                                {
                                    cout << "Invalid. Try again:" ;
                                    cin >> choice1;


                                }
                                cout << "Choose which one is duplicate of: " ;
                                cin >> choice2;

                                while (choice2 > size || choice2 == choice1)
                                {

                                    cout << "Invalid. Try again:" ;
                                    cin >> choice2;


                                }
                                Content* content1;
                                Content* content2;
                                for (auto &question1 : questions)
                                    if (question1.getId() == choice1) {
                                        content1 = &question1;
                                        question1.isDuplicate = true;
                                        question1.duplicateId = choice2;

                                    }


                                for (auto &question2 : questions)
                                    if (question2.getId() == choice2) {
                                        content2 = &question2;

                                    }


                                ContentRelation contentRelation(*content1,ContentRelationType::DOUPLICATE_OF);
                                content2->addToRelations(contentRelation);


                            }
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
        system(CLEAR);

    }

    cout << "GOODBYE" << endl;
    return 0;
}
