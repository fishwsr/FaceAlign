#pragma once

#include "faceAlign.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "FaceSketch.h"

using namespace cv;

class CVideoRenderer
{
public:
	CVideoRenderer(std::string videoFilePath);
	~CVideoRenderer(void);
	Mat getFirstFrame();
	void render(std::string renderedVideoPath);
	Mat renderKeyFrame( Mat currentSrc);
	Mat propagateFromLastFrame( Mat currentSrc, Mat lastDst );
private:
	VideoCapture *srcVideoCapture;
	Mat firstFrame;
};

