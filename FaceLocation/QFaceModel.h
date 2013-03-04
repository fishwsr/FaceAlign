#pragma once
#include <QVector>
#include <QtGui/QGraphicsEllipseItem>
#include <QtGui/QGraphicsLineItem>

class QFaceModel
{
public:
	QFaceModel(float *ptsPos, int pointnum, QGraphicsItem *parent);
	~QFaceModel(void);
	void paint();
private:
	void Connect(float *ptsPos,QGraphicsItem *parent);
	QVector<QGraphicsEllipseItem *> nodes;
	QVector<QGraphicsLineItem *> links;
};