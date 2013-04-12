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
private:
	VideoCapture *srcVideoCapture;
	Mat firstFrame;
};

