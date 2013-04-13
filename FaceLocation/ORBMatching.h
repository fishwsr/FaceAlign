#pragma once
#include "cv.h"
#include "FaceDetect.h"

class ORBMatching
{
public:
	ORBMatching(void);
	~ORBMatching(void);
	void findMatchigPoint( cv::Mat currentFrame, cv::Mat previousFrame, std::vector<cv::Point> previousFace, std::vector<cv::Point> &kp1, std::vector<cv::Point> &kp2 );
private:
	CFaceDetect faceDetect;
};

