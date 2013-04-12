#pragma once

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class CVideoRenderer
{
public:
	CVideoRenderer(std::string videoFilePath);
	~CVideoRenderer(void);
	Mat getFirstFrame();
	void render(std::string renderedVideoPath);
	Mat renderKeyFrame( Mat currentSrc );
	Mat propagateFromLastFrame( Mat currentSrc, vector<cv::Point> currentFace, Mat lastSrc, Mat lastDst, vector<cv::Point> lastFace );
private:
	VideoCapture *srcVideoCapture;
	Mat firstFrame;
	int frameWidth;
    int frameHeight;
};

