#pragma once
#include "cv.h"

class ORBMatching
{
public:
	ORBMatching(void);
	~ORBMatching(void);
	void findMatchigPoint( cv::Mat previousFrame, float* facePoint1, cv::Mat currentFrame, float* facePoint2);
};

