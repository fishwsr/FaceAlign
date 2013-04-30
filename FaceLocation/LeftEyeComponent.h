#pragma once
#include "facecomponent.h"
class CLeftEyeComponent :
	public CFaceComponent
{
public:
	CLeftEyeComponent(int templateIndex, QFaceModel* faceModel);
	~CLeftEyeComponent(void);
protected:
	virtual QVector<Node*> getLocatedNodes();
};

