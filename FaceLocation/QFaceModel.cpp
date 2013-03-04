#include "QFaceModel.h"
#include <QtGui>

QFaceModel::QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent)
{
	nodes.resize(pointnum);
	for(int i=0;i<pointnum;i=i++)
	{
		nodes[i] = new QGraphicsEllipseItem(QRect(ptsPos[2*i], ptsPos[2*i+1], 3, 3),parent);;
		nodes[i]->setFlag(QGraphicsItem::ItemIsMovable,true);
		nodes[i]->setPen(QPen(Qt::red));
		nodes[i]->setBrush(Qt::red);
	}
	Connect(ptsPos,parent);
}


QFaceModel::~QFaceModel(void)
{
	nodes.clear();
	links.clear();
}

void QFaceModel::Connect(float *ptsPos,QGraphicsItem *parent)
{
	links.resize(nodes.size()-8);
	for(int i = 0,j = 0;j<links.size();j++)
	{
		i=2*j;
		int c = 0;
		QPointF p1(ptsPos[i],ptsPos[i+1]);
		switch(i)
		{
		case 14:
		case 30:
			c = i - 14;
			break;
		case 50:
		case 70:
			c = i - 18;
			break;
		case 94:
			c = i - 22;
			break;
		case 134:
			c = i - 38;
			break;
		default:
			c = i + 2;
		}
		QPointF p2(ptsPos[c],ptsPos[c+1]);
		links[j] = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y(), parent);
		links[j]->setPen(QPen(Qt::blue));
	}
}