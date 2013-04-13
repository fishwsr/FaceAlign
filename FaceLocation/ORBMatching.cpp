#include "ORBMatching.h"
#include <cxcore.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <qt\qdebug.h>


ORBMatching::ORBMatching(void)
{
}


ORBMatching::~ORBMatching(void)
{
}

void ORBMatching::findMatchigPoint( cv::Mat currentFrame, cv::Mat previousFrame, std::vector<cv::Point> previousFace, std::vector<cv::Point> &kp1, std::vector<cv::Point> &kp2 )
{
	double t = (double)getTickCount();
	
	ORB orb;
	vector<KeyPoint> keyPoints_1_Orb, keyPoints_2_Orb;
	Mat descriptors_1_Orb, descriptors_2_Orb;
	keyPoints_1_Orb.resize(previousFace.size());
	for(int i=0;i<previousFace.size();i++)
	{
		keyPoints_1_Orb[i].pt.x = previousFace[i].x;
		keyPoints_1_Orb[i].pt.y = previousFace[i].y;
	}
	
	RECT rcBox;
	faceDetect.detectAndDisplay(currentFrame, &rcBox); 
	int rcNum = (rcBox.right-rcBox.left) * (rcBox.bottom-rcBox.top);
	keyPoints_2_Orb.resize(rcNum);
	for(int i=rcBox.left, k=0; i<rcBox.right; i++)
	{
		for (int j=rcBox.top; j<rcBox.bottom; j++,k++)
		{
			keyPoints_2_Orb[k].pt.x = i;
			keyPoints_2_Orb[k].pt.y = j;
		}
	}

	orb(previousFrame, Mat(), keyPoints_1_Orb, descriptors_1_Orb, true);
	orb(currentFrame, Mat(), keyPoints_2_Orb, descriptors_2_Orb, true);

	BruteForceMatcher<Hamming> matcher_Orb;
	vector<DMatch> matches_Orb;
	matcher_Orb.match(descriptors_1_Orb, descriptors_2_Orb, matches_Orb);

	for(int i=0; i<matches_Orb.size(); i++)
	{
		DMatch k = matches_Orb[i];
		kp1.push_back(keyPoints_1_Orb[k.queryIdx].pt);
		kp2.push_back(keyPoints_2_Orb[k.trainIdx].pt);
	}
	t = ((double)getTickCount() - t)/getTickFrequency();
	qDebug("ORB Matching -- Times passed in seconds: %f\n", t);
}
