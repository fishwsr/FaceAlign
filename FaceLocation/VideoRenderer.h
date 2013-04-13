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
	void render(std::string renderedVideoPath);
	Mat renderKeyFrame( Mat currentSrc );
	Mat propagateFromLastFrame( Mat currentSrc, Mat lastSrc, Mat lastDst );
private:
	VideoCapture *srcVideoCapture;
	int interval;
	Mat firstFrame;
	int frameWidth;
    int frameHeight;
	vector<cv::Point> currentFace;
	bool isKeyFrame(int index);
	ORBMatching orb;
};

