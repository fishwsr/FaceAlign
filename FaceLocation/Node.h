#ifndef NODE_H
#define NODE_H

#include <QApplication>
#include <QColor>
#include <QtGui/QGraphicsEllipseItem>
#include <QSet>

class Link;

class Node : public QGraphicsEllipseItem
{
    Q_DECLARE_TR_FUNCTIONS(Node)

public:
    Node(QPointF nodecenter, QGraphicsItem *parent);
    ~Node();
    void addLink(Link *link);
	void removeLink(Link *link);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private:
    QSet<Link *> myLinks;
};

#endif
