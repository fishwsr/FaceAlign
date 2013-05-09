#pragma once
#include <QObject>
#include "VideoRenderer.h"

class CRenderWorker : public QObject {

	Q_OBJECT

public:
	CRenderWorker(CVideoRenderer* videoRenderer, int bgThresholdValue, int qtzThresholdValue);
	~CRenderWorker();
public slots:
	void renderVideo();
private:
	CVideoRenderer* videoRenderer;
	QString* curFile;
	int bgThresholdValue;
	int qtzThresholdValue;

};

