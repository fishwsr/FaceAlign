#pragma once
#include "facecomponent.h"
class CMouthComponent :
	public CFaceComponent
{
public:
	CMouthComponent(int templateIndex, QFaceModel* faceModel);
	~CMouthComponent(void);
protected:
	virtual QVector<Node*> getLocatedNodes();
};

