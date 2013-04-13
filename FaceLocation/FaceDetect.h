#pragma once

#include <afxwin.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace cv;

class CFaceDetect
{
public:
	CFaceDetect(void);
	~CFaceDetect(void);
	void detectAndDisplay(cv::Mat& image, RECT* dectBox);
private:
	cv::CascadeClassifier cascade;
	const char* FACE_CASCADE_NAME;
};

