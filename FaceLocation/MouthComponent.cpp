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

cv::Mat CMouthComponent::renderComponent( int width, int height )
{
	warpedTemplate = cvCreateMat(height, width, CV_8UC3);
	return CFaceComponent::renderComponent(width, height);
}

void CMouthComponent::doRender( std::vector<cv::Point> templatePoints, cv::Mat templateMat )
{
	
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
			double isMouthPoint = pointPolygonTest( mouthPoints, cv::Point2f(row,col), false );
			if(isMouthPoint > 0){
				_I(col,row)[0] = 68;
				_I(col,row)[1] = 101;
				_I(col,row)[2] = 187;
			}
		}
	}
	warpedTemplate = _I;
}
