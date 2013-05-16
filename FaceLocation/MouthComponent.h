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
	virtual std::vector<cv::Point> filterPoints(std::vector<cv::Point> points);
	virtual void renderComponentInColor(std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height);
};

