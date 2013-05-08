#include "RenderWorker.h"
#include <QFileInfo>

CRenderWorker::CRenderWorker(CVideoRenderer* videoRenderer, QString* curFile, int bgThresholdValue, int qtzThresholdValue)
{
	this->videoRenderer = videoRenderer;
	this->curFile = curFile;
	this->bgThresholdValue = bgThresholdValue;
	this->qtzThresholdValue = qtzThresholdValue;
}


CRenderWorker::~CRenderWorker(void)
{
}

void CRenderWorker::renderVideo()
{
	QString renderedVideoPath = "temp/"+ QFileInfo(*curFile).baseName() + ".avi";
	videoRenderer->render(renderedVideoPath.toStdString(), bgThresholdValue, qtzThresholdValue);
}
