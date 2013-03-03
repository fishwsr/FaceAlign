#include "QFaceModel.h"
#include <QtGui>

QFaceModel::QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent)
{
	nodes.resize(pointnum);
	for(int i=0;i<pointnum;i=i++)
	{
		nodes[i] = new QGraphicsEllipseItem(QRect(ptsPos[2*i], ptsPos[2*i+1], 2.5, 2.5),parent);;
		nodes[i]->setFlag(QGraphicsItem::ItemIsMovable,true);
		nodes[i]->setPen(QPen(Qt::red));
		nodes[i]->setBrush(Qt::red);
		nodes[i]->setFlag(QGraphicsItem::ItemIsMovable,true);
	}
}


QFaceModel::~QFaceModel(void)
{
	nodes.clear();
}