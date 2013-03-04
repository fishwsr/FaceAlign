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
	//QPointF p1 = myFromNode->pos());
	//QPointF p2 = myToNode->mapFromParent(0,0);

    setLine(QLineF(myFromNode->scenePos(), myToNode->scenePos()));
}
