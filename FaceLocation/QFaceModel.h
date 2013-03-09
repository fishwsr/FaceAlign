#pragma once
#include <QVector>
#include "Node.h"
#include "Link.h"

class QFaceModel
{
public:
	QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent);
	~QFaceModel(void);
	QVector<Node*> getLeftBrowNodes();
	QVector<Node*> getRightBrowNodes();
	QVector<Node*> getLeftEyeNodes();
	QVector<Node*> getRightEyeNodes();
	QVector<Node*> getNoseNodes();
	QVector<Node*> getMouthNodes();
	QVector<Node*> getFaceProfileNodes();
	void paint();
private:
	void Connect(float *ptsPos,QGraphicsItem *parent);
	QVector<Node *> nodes;
	QVector<Link *> links;
	
};