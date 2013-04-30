#pragma once
#include "facecomponent.h"
class CRightBrowComponent :
	public CFaceComponent
{
public:
	CRightBrowComponent(int templateIndex, QFaceModel* faceModel);
	~CRightBrowComponent(void);
protected:
	std::vector<cv::Point> getTemplatePointsFromFile(std::ifstream* fin);
	std::vector<cv::Point> CRightBrowComponent::getLocatedPoints();
	virtual QVector<Node*> getLocatedNodes();
};

