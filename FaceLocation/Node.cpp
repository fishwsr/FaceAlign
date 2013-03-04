#include <QtGui>

#include "link.h"
#include "node.h"

Node::Node(QRectF rect, QGraphicsItem *parent)
{
	setParentItem(parent);
	setRect(rect);
	setPen(QPen(Qt::red));
	setBrush(Qt::red);
    setFlags(ItemIsMovable | ItemIsSelectable);
	//setPos(rect.center());
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