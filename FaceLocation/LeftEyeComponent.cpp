#include "LeftEyeComponent.h"


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
