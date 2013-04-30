#pragma once
#include "facecomponent.h"
class CNoseComponent :
	public CFaceComponent
{
public:
	CNoseComponent(int templateIndex, QFaceModel* faceModel);
	~CNoseComponent(void);
protected:
	virtual QVector<Node*> getLocatedNodes();
	std::vector<cv::Point> getTemplatePointsFromFile(std::ifstream* fin);
};

