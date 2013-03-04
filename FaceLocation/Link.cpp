#include <QtGui>

#include "link.h"
#include "node.h"

Link::Link(Node *fromNode, Node *toNode, QGraphicsItem *parent)
{
	setParentItem(parent);
    myFromNode = fromNode;
    myToNode = toNode;

    myFromNode->addLink(this);
    myToNode->addLink(this);

    //setZValue(0);
	setPen(QPen(Qt::blue));
    trackNodes();
}

Link::~Link()
{
    myFromNode->removeLink(this);
    myToNode->removeLink(this);
}

void Link::trackNodes()
{
	QPointF p1 = myFromNode->sceneBoundingRect().center();
	QPointF p2 = myToNode->sceneBoundingRect().center();
	setLine(QLineF(p1, p2));
}
