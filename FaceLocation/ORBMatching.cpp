#include "ORBMatching.h"
#include "faceAlign.h"
#include <cxcore.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

ORBMatching::ORBMatching(void)
{
}


ORBMatching::~ORBMatching(void)
{
}

void ORBMatching::findMatchigPoint( cv::Mat previousFrame, float* facePoint1, cv::Mat currentFrame, std::vector<cv::Point> kp1, std::vector<cv::Point> kp2 )
{
	CFaceAlign currentFace;
	ORB orb;
	vector<KeyPoint> keyPoints_1_Orb, keyPoints_2_Orb;
	Mat descriptors_1_Orb, descriptors_2_Orb;
	keyPoints_1_Orb.resize(currentFace.PointNum());
	for(int i=0;i<currentFace.PointNum();i++)
	{
		keyPoints_1_Orb[i].pt.x = facePoint1[2*i];
		keyPoints_1_Orb[i].pt.y = facePoint1[2*i+1];
	}
	
	RECT rcBox;
	currentFace.detectAndDisplay(currentFrame, &rcBox); 
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

}
