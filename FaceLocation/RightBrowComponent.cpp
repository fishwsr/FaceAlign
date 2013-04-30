#include "RightBrowComponent.h"


CRightBrowComponent::CRightBrowComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "rightEyeBrow";
	picNamePrefix = "rightEyeBrow";
	ptsNamePrefix = "eyeBrow";
	actualComponentPtsNum = 8;
}


CRightBrowComponent::~CRightBrowComponent(void)
{
}

std::vector<cv::Point> CRightBrowComponent::getLocatedPoints(){

	QVector<Node*> locatedNodes = faceModel->getRightBrowNodes();
	std::vector<cv::Point> locatedPointsToUse;
	locatedPointsToUse.resize(actualComponentPtsNum);
	for(int i = 0; i<actualComponentPtsNum; i++) {
		if(i == 0) {
			locatedPointsToUse[i].x = locatedNodes[4]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[4]->sceneBoundingRect().center().y();
		} else if ( i < 4){
			locatedPointsToUse[i].x = locatedNodes[4-i]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[4-i]->sceneBoundingRect().center().y();
		} else if ( i >= 4) {
			locatedPointsToUse[i].x = locatedNodes[13-i]->sceneBoundingRect().center().x();
			locatedPointsToUse[i].y = locatedNodes[13-i]->sceneBoundingRect().center().y();
		}
	}

	return locatedPointsToUse;

}

std::vector<cv::Point> CRightBrowComponent::getTemplatePointsFromFile( std::ifstream* fin)
{
	int ptsNumInFile;
	*fin >> ptsNumInFile;

	std::vector<cv::Point> templatePoints;
	templatePoints.resize(actualComponentPtsNum);

	int width = templateImg.cols;

	for (int i = 0; i < ptsNumInFile; i++) {
		double ptsX, ptsY;
		*fin >> ptsX >>ptsY;
		templatePoints[i].x = width - ptsX;
		templatePoints[i].y = ptsY;
	}
	return templatePoints;
}

