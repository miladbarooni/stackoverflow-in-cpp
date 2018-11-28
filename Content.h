//
// Created by milord on 11/22/18.
//
#include <string>
#include <vector>
#include <iostream>
using namespace std;
#ifndef STACKOVERFLOW_IN_CPP_CONTENT_H
#define STACKOVERFLOW_IN_CPP_CONTENT_H


enum ContentRelationType{
    DOUPLICATE_OF,
    ANSWER_TO
};
class Content;

enum ContentType{
    QUESTION,
    ANSWER
};

class ContentRelation
{

    Content *destination;
    ContentRelationType content_relation_type;
public:
    ContentRelation(Content& content, ContentRelationType type);
    Content* getDestination();
};

class Content{
public:
    Content(string content, ContentType content_type, int id);
    int getId();
    string getBody();
    void addToRelations(ContentRelation content_relation);
    void printAnswers();
    void addVisits();
    int getVisits();
    vector<ContentRelation> relations;
    void setBody(string new_content);
    void setId(int new_id);
private:
    string body;
    ContentType contentType;
    int visits; 
    int id;



};

#endif //STACKOVERFLOW_IN_CPP_CONTENT_H
