#pragma once

#include "faceAlign.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "FaceSketch.h"
#include "ORBMatching.h"

using namespace cv;

class CVideoRenderer
{
public:
	CVideoRenderer(std::string videoFilePath);
	~CVideoRenderer(void);
	Mat getFirstFrame();
	void render(std::string renderedVideoPath, int bgThresholdValue, int fcThresholdValue);
private:
	VideoCapture *srcVideoCapture;
	int interval;
	Mat firstFrame;
	int frameWidth;
        int frameHeight;
        ORBMatching orb;
	vector<cv::Point> currentFace;
	bool isKeyFrame(int index);
	Mat renderKeyFrame( Mat currentSrc, int bgThresholdValue= 60, int fcThresholdValue = 80 );
	Mat propagateFromLastFrame( Mat currentSrc, Mat lastSrc, Mat lastDst );
};

