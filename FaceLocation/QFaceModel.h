#pragma once
#include <QVector>
#include "Node.h"
#include "Link.h"

class QFaceModel
{
public:
	QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent);
	~QFaceModel(void);
	void paint();
private:
	void Connect(float *ptsPos,QGraphicsItem *parent);
	QVector<Node *> nodes;
	QVector<Link *> links;
};