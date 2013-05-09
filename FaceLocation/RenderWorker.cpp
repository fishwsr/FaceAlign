#include "RenderWorker.h"
#include <QFileInfo>
#include <QThread>

CRenderWorker::CRenderWorker(CVideoRenderer* videoRenderer, int bgThresholdValue, int qtzThresholdValue)
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
	videoRenderer->render(bgThresholdValue, qtzThresholdValue);
	QThread::currentThread()->exit();
}
