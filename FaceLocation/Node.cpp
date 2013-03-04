#include <QtGui>

#include "link.h"
#include "node.h"

Node::Node(QPointF nodecenter, QGraphicsItem *parent)
{
	setParentItem(parent);
	setRect(QRectF(0, 0, 3, 3));
	setPen(QPen(Qt::red));
	setBrush(Qt::red);
    setFlags(ItemIsMovable | ItemIsSelectable |ItemSendsGeometryChanges);
	setPos(nodecenter.x(),nodecenter.y());
}

Node::~Node()
{
    foreach (Link *link, myLinks)
        delete link;
}

void Node::addLink(Link *link)
{
    myLinks.insert(link);
}

void Node::removeLink(Link *link)
{
	myLinks.remove(link);
}

QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Link *link, myLinks)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}