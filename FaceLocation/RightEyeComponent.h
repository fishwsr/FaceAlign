#pragma once
#include "facecomponent.h"
class CRightEyeComponent :	public CFaceComponent
{
public:
	CRightEyeComponent(int templateIndex, QFaceModel* faceModel);
	~CRightEyeComponent(void);
protected:
	virtual QVector<Node*> getLocatedNodes();
};

