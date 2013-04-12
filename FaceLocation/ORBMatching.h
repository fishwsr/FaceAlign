#pragma once
#include "cv.h"

class ORBMatching
{
public:
	ORBMatching(void);
	~ORBMatching(void);
	void findMatchigPoint( cv::Mat previousFrame, float* facePoint1, cv::Mat currentFrame, std::vector<cv::Point> kp1, std::vector<cv::Point> kp2);
};

