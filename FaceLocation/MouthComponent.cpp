#include "MouthComponent.h"


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
