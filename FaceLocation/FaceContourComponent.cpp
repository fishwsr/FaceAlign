#include "FaceContourComponent.h"


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
