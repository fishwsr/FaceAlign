#include "LeftBrowComponent.h"
#include <opencv2\core\core.hpp>
#include <qstring.h>


CLeftBrowComponent::CLeftBrowComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "leftEyeBrow";
	picNamePrefix = "leftEyeBrow";
	ptsNamePrefix = "eyeBrow";
	actualComponentPtsNum = 8;
}


CLeftBrowComponent::~CLeftBrowComponent(void)
{
}

std::vector<cv::Point> CLeftBrowComponent::getLocatedPoints(){

	QVector<Node*> locatedNodes = faceModel->getLeftBrowNodes();
	std::vector<cv::Point> locatedPointsToUse;
	locatedPointsToUse.resize(actualComponentPtsNum);
	for(int i = 0; i<actualComponentPtsNum; i++) {
		if(i == 0) {
			locatedPointsToUse[i].x = locatedNodes[9]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[9]->sceneBoundingRect().center().y();
		} else if ( i < 5){
			locatedPointsToUse[i].x = locatedNodes[i]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[i]->sceneBoundingRect().center().y();
		} else if ( i >= 5) {
			locatedPointsToUse[i].x = locatedNodes[i+1]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[i+1]->sceneBoundingRect().center().y();
		}
	}

	return locatedPointsToUse;

}

QVector<Node*> CLeftBrowComponent::getLocatedNodes()
{
	return faceModel->getLeftBrowNodes();
}


