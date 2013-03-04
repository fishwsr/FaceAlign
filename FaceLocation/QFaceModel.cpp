#include "QFaceModel.h"
#include "Node.h"
#include "Link.h"
#include <QtGui>

QFaceModel::QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent)
{
	nodes.resize(pointnum);
	for(int i=0;i<pointnum;i=i++)
	{	
		nodes[i] = new Node(QPointF(ptsPos[2*i], ptsPos[2*i+1]),parent);
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
	for(int i = 0;i<links.size();i++)
	{
		int c = 0;
		switch(i)
		{
		case 7:
		case 15:
			c = i - 7;
			break;
		case 25:
		case 35:
			c = i - 9;
			break;
		case 47:
			c = i - 11;
			break;
		case 67:
			c = i - 19;
			break;
		default:
			c = i + 1;
		}
		links[i] = new Link(nodes[i], nodes[c], parent);
	}
}