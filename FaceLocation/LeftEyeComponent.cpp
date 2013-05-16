#include "LeftEyeComponent.h"
#include <opencv2\core\core.hpp>


CLeftEyeComponent::CLeftEyeComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "leftEye";
	picNamePrefix = "leftEye";
	ptsNamePrefix = "leftEye";
	actualComponentPtsNum = 8;
}


CLeftEyeComponent::~CLeftEyeComponent(void)
{
}

QVector<Node*> CLeftEyeComponent::getLocatedNodes()
{
	return faceModel->getLeftEyeNodes();
}

void CLeftEyeComponent::preProcessTemplateImage()
{
	std::vector<std::vector<cv::Point>> templateKeyPoints = getTemplateKeyPoints();
	//templateImg.setTo(255);
	for (int i = 0; i < templateKeyPoints.size(); i++)
	{
		for(int j = 0; j< templateKeyPoints[i].size()-1; j++)
		{
			cv::line(templateImg, templateKeyPoints[i][j], templateKeyPoints[i][j+1],cv::Scalar(10, 10, 10),4);
		}
	};
}
