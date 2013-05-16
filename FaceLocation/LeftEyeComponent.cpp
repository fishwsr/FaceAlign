#include "LeftEyeComponent.h"
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>


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

void CLeftEyeComponent::renderComponentInColor( std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height )
{
	CFaceComponent::renderComponentInColor(templatePoints, templateMat, width, height);

	/*
	模板已经是白色了
	cv::Mat_<cv::Vec3b> _I = warpedTemplate;
	std::vector<cv::Point> eyePoints = getLocatedPoints();
	#pragma omp parallel for
	for( int row = 0; row < warpedTemplate.rows; ++row){
		for( int col = 0; col < warpedTemplate.cols; ++col )
		{
			double isEyePoint = pointPolygonTest(eyePoints, cv::Point2f(col,row), false );
			if(isEyePoint > 0){
				if(_I(row,col)[0] > 250 && _I(row,col)[1] > 250 && _I(row,col)[2] > 250){
					_I(row,col)[0] = 253;
					_I(row,col)[1] = 254;
					_I(row,col)[2] = 252;
				}
			}
		}
	}
	warpedTemplate = _I;*/
}
