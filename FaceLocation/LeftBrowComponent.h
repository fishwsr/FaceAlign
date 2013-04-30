#pragma once
#include "facecomponent.h"
class CLeftBrowComponent :	public CFaceComponent
{
public:
	CLeftBrowComponent(int templateIndex, QFaceModel* faceModel);
	~CLeftBrowComponent(void);
protected:
	std::vector<cv::Point> getLocatedPoints();
};

