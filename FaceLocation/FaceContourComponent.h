#pragma once
#include "facecomponent.h"
class CFaceContourComponent :
	public CFaceComponent
{
public:
	CFaceContourComponent(int templateIndex, QFaceModel* faceModel);
	~CFaceContourComponent(void);
protected:
	virtual QVector<Node*> getLocatedNodes();
};

