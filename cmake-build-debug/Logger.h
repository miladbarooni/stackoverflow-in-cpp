//
// Created by milord on 11/27/18.
//

#ifndef STACKOVERFLOW_IN_CPP_LOGGER_H
#define STACKOVERFLOW_IN_CPP_LOGGER_H


class Logger {
private:
    Logger(){};
    static Logger *logger;
public:
    static Logger* getInstance() {

        return (!logger) ?
                logger = new Logger :
               logger;
    }

};


#endif //STACKOVERFLOW_IN_CPP_LOGGER_H
