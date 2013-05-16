#include "MouthComponent.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

CMouthComponent::CMouthComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "mouth";
	picNamePrefix = "mouth";
	ptsNamePrefix = "mouth";
	actualComponentPtsNum = 12;
}


CMouthComponent::~CMouthComponent(void)
{
}

QVector<Node*> CMouthComponent::getLocatedNodes()
{
	return faceModel->getMouthNodes();
}

std::vector<cv::Point> CMouthComponent::filterPoints( std::vector<cv::Point> points )
{
	return points;
}


void CMouthComponent::renderComponentInColor( std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height )
{
	warpedTemplate = cvCreateMat(height, width, CV_8UC3);
	warpedTemplate.setTo(255);
	std::vector<cv::Point> mouthPoints = getLocatedPoints();
	for (int i = 0; i< mouthPoints.size(); i++)
	{
		int j;
		if ( i == 11 || i == 19)
		{
			j = 0;
		}
		else
		{
			j = i + 1;
		}
		cv::Point kp1 = mouthPoints[i];
		cv::Point kp2 = mouthPoints[j];
		cv::line(warpedTemplate, kp1, kp2,cv::Scalar(0, 0, 255), 1);
	}

	cv::Mat_<cv::Vec3b> _I = warpedTemplate;

	#pragma omp parallel for
	for( int row = 0; row < warpedTemplate.rows; ++row){
		for( int col = 0; col < warpedTemplate.cols; ++col )
		{
			double isMouthPoint = pointPolygonTest( mouthPoints, cv::Point2f(col,row), false );
			if(isMouthPoint > 0){
				_I(row,col)[0] = 68;
				_I(row,col)[1] = 101;
				_I(row,col)[2] = 187;
			}
		}
	}
	warpedTemplate = _I;
}
