#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

class Node;

class Link : public QGraphicsLineItem
{
public:
    Link(Node *fromNode, Node *toNode, QGraphicsItem *parent);
    ~Link();

    void trackNodes();

private:
    Node *myFromNode;
    Node *myToNode;
};

#endif
