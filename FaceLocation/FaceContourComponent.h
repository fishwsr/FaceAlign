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
	virtual std::vector<cv::Point> filterPoints(std::vector<cv::Point> points);
	virtual cv::Mat renderComponent(int width, int height);
	virtual void doRender(std::vector<cv::Point> templatePoints, cv::Mat templateMat);
};

