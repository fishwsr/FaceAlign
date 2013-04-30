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
