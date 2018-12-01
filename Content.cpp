//
// Created by milord on 11/22/18.
//

#include "Content.h"

Content::Content(string content, ContentType content_type, int id=0)
{
    this->body = content;
    this->contentType = content_type;
    this->visits = 0;
    this->id = id;
    vector<ContentRelation> temp;
    this->relations = temp;
    vector<Content> temp1;
    this->answers = temp1;

}
void Content::addVisits()
{
    this->visits++;
}

int Content::getVisits()
{
    return this->visits;
}

int Content::getId()
{
    return this->id;
}
string Content::getBody()
{
    return this->body;
}
void Content::addToRelations(ContentRelation content_relation)
{
    relations.push_back(content_relation);
}

void Content::printAnswers()
{
    if (answers.size() == 0)
    {
        cout << "\nNOTE:  There is no Answer for this question yet(Be first one)" << endl ;
    }
    for (auto &answer : answers)
        cout << "A." << answer.getBody()<<endl;
}


ContentRelation::ContentRelation(Content& content, ContentRelationType type)
{
    this-> destination = &content;
    this-> content_relation_type = type;
}

Content* ContentRelation::getDestination()
{
    return this->destination;
}

void Content::setBody(string new_body) {
    this->body = new_body;
}

void Content::setId(int new_id)
{
    this->id = new_id;
}