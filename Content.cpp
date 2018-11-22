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
    for (auto &content_relation : relations)
    {
        cout << "A. " << content_relation.getDestination()->getBody()<<endl ;

    }
}


ContentRelation::ContentRelation(Content content, ContentRelationType type)
{
    this-> destination = &content;
    this-> content_relation_type = type;
}

Content* ContentRelation::getDestination()
{
    return this->destination;
}