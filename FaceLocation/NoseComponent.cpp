#include "NoseComponent.h"


CNoseComponent::CNoseComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "nose";
	picNamePrefix = "nose";
	ptsNamePrefix = "nose";
	actualComponentPtsNum = 12;
}


CNoseComponent::~CNoseComponent(void)
{
}

QVector<Node*> CNoseComponent::getLocatedNodes()
{
	return faceModel->getNoseNodes();
}

std::vector<cv::Point> CNoseComponent::getTemplatePointsFromFile( std::ifstream* fin )
{
	int ptsNumInFile;
	*fin >> ptsNumInFile;

	std::vector<cv::Point> templatePoints;
	templatePoints.resize(actualComponentPtsNum);

	int width = templateImg.cols;

	for (int i = 0; i < ptsNumInFile; i++) {
		double ptsX, ptsY;
		*fin >> ptsX >>ptsY;
		if(i < 6) {
			templatePoints[i].x = ptsX;
			templatePoints[i].y = ptsY; 
		}else if(i == 6) {
			continue;
		} else if(i > 6) {
			templatePoints[i-1].x = ptsX;
			templatePoints[i-1].y = ptsY;
		}
	}
	return templatePoints;
}
