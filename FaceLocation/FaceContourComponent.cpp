#include "FaceContourComponent.h"
#include <opencv2\highgui\highgui.hpp>

CFaceContourComponent::CFaceContourComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "faceContour";
	picNamePrefix = "faceContour";
	ptsNamePrefix = "faceContour";
	actualComponentPtsNum = 19;
}


CFaceContourComponent::~CFaceContourComponent(void)
{
}

QVector<Node*> CFaceContourComponent::getLocatedNodes()
{
	return faceModel->getFaceProfileNodes();
}

std::vector<cv::Point> CFaceContourComponent::filterPoints( std::vector<cv::Point> points )
{
	std::vector<cv::Point> filteredPoints;

	for(int i = 0; i < points.size(); i++){
		if(i%3 == 0) {
			filteredPoints.push_back(points.at(i));
		}
	}
	return filteredPoints;
}

void CFaceContourComponent::renderComponentInColor( std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height )
{
	warpedTemplate = cvCreateMat(height, width, CV_8UC3);
	warpedTemplate.setTo(255);
	std::vector<cv::Point> facePoints = getLocatedPoints();
	for (int i = 0; i< facePoints.size()-1; i++)
	{
		cv::line(warpedTemplate, facePoints[i], facePoints[i+1],cv::Scalar(10, 10, 10),1);
	}
}
