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

    setZValue(-1);
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
	QPointF p1 = myFromNode->pos();
	QPointF p2 = myToNode->pos();
    setLine(QLineF(myFromNode->pos(), myToNode->pos()));
}
