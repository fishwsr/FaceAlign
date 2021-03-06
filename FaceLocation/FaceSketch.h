#pragma once
#include "QFaceModel.h"
#include <iostream>
#include "cv.h"

class CFaceSketch
{
public:
	CFaceSketch();
	~CFaceSketch(void);
	cv::Mat sketchFace( QFaceModel* ASMModel, cv::Mat srcImg, bool isRenderVideo = false, int bgThresholdValue = 60, int faceThresholdValue = 15);
	std::vector<cv::Point> const getPointsToWrap(){return pointsToWrap;}
	void updateBackground(cv::Mat srcImg, int bgThresholdValue, int faceThresholdValue);
	int getMouthIndex() const { return mouthIndex; }
	void setMouthIndex(int val) { mouthIndex = val; }
	int getNoseIndex() const { return noseIndex; }
	void setNoseIndex(int val) { noseIndex = val; }
	int getFaceIndex() const { return faceIndex; }
	void setFaceIndex(int val) { faceIndex = val; }
	int getEyeIndex() const { return eyeIndex; }
	void setEyeIndex(int val) { eyeIndex = val; }
	int getBrowIndex() const { return browIndex; }
	void setBrowIndex(int val) { browIndex = val; }
	void setHasColor(bool val) { hasColor = val; }
private:
	int width, height;
	int bgThresholdValue;
	int fcThresholdValue;
	int qtzThresholdValue;
	bool hasColor;
	QFaceModel* facemodel;
	cv::Mat bgCurve;
	cv::Mat bgColor;
	QMap<int, cv::Mat> wrappedFaceCompMap;
	std::vector<cv::Point> pointsToWrap;
	int browIndex, eyeIndex, noseIndex, mouthIndex, faceIndex;
	enum faceElement{LEFTEYE, RIGHTEYE, LEFTBROW, RIGHTBROW, NOSE, MOUTH, PROFIILE, ALL};
	void combineSketch(bool combineFace = true);
	void addTopToBottom( cv::Mat &top, cv::Mat &botom);
	bool isWhite(cv::MatIterator_<cv::Vec3b> point);
	bool isBackground(cv::MatIterator_<cv::Vec3b> point);
	void combineComponent();
	std::vector<cv::Point> getLocatedFaceContour();
	void backgroudSketch(cv::Mat srcImg);
	cv::Mat getFaceMask();
	void backgroudSketch2(cv::Mat srcImg);
	void backgroundColor(cv::Mat srcImg);
	void backgroundSmooth( cv::Mat &srcImg );
	void colorQuantization();
	void fixTemplate( cv::Mat &srcImg);
	bool isInEyeBall(std::vector<cv::Point> leftPoints, std::vector<cv::Point> rightPoints, float x, float y);
};

