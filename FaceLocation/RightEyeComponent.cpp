#include "RightEyeComponent.h"


CRightEyeComponent::CRightEyeComponent(int templateIndex, QFaceModel* faceModel):CFaceComponent(templateIndex, faceModel)
{
	folderName = "rightEye";
	picNamePrefix = "rightEye";
	ptsNamePrefix = "rightEye";
	actualComponentPtsNum = 8;
}


CRightEyeComponent::~CRightEyeComponent(void)
{
}

QVector<Node*> CRightEyeComponent::getLocatedNodes()
{
	return faceModel->getRightEyeNodes();
}
