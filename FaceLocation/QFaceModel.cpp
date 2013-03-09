#include "QFaceModel.h"
#include "Node.h"
#include "Link.h"
#include <QtGui>


QFaceModel::QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent)
{
	nodes.resize(pointnum);
	for(int i=0;i<pointnum;i=i++)
	{	
		nodes[i] = new Node(QRectF(ptsPos[2*i], ptsPos[2*i+1], 3, 3),parent);
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

QVector<Node*> QFaceModel::getRightBrowNodes()
{
	return nodes.mid(26, 10);
}

QVector<Node*> QFaceModel::getLeftBrowNodes()
{
	return nodes.mid(16, 10);
}

QVector<Node*> QFaceModel::getLeftEyeNodes()
{
	return nodes.mid(0, 8);
}

QVector<Node*> QFaceModel::getRightEyeNodes()
{
	return nodes.mid(8, 8);
}

QVector<Node*> QFaceModel::getNoseNodes()
{
	return nodes.mid(36, 12);
}

QVector<Node*> QFaceModel::getMouthNodes()
{
	return nodes.mid(48, 20);
}

QVector<Node*> QFaceModel::getFaceProfileNodes()
{
	return nodes.mid(68, 19);
}

void QFaceModel::paint()
{

}
