#pragma once

#include "faceAlign.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "FaceSketch.h"
#include "ORBMatching.h"

using namespace cv;

class CVideoRenderer: public QObject
{
	Q_OBJECT
public:
	CVideoRenderer(QString* videoFilePath, CFaceSketch* faceSketch);
	~CVideoRenderer(void);
	Mat getFirstFrame();
	void render(int bgThresholdValue, int qtzThresholdValue);
	QString getRenderedVideoPath();
public slots:
	void stopRender();
signals:
	void progressChanged(float percent);
private:
	VideoCapture *srcVideoCapture;
	QString renderedVideoPath;
	int interval;
	Mat firstFrame;
	int frameWidth;
    int frameHeight;
	int frameCount;
	bool isStop;
    ORBMatching orb;
	CFaceSketch* faceSketch;
	vector<cv::Point> currentFace;
	bool isKeyFrame(int index);
	Mat renderKeyFrame( Mat currentSrc, int bgThresholdValue= 60, int qtzThresholdValue = 15 );
	Mat propagateFromLastFrame( Mat currentSrc, Mat lastSrc, Mat lastDst );
};

