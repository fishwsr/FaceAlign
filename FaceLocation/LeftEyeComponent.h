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
	virtual void preProcessTemplateImage();
	virtual void renderComponentInColor(std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height);
};

