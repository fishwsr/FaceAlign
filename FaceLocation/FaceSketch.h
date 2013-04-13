#pragma once
#include "QFaceModel.h"
#include <iostream>
#include "cv.h"

class CFaceSketch
{
public:
	CFaceSketch();
	~CFaceSketch(void);
	cv::Mat sketchFace( QFaceModel* ASMModel, cv::Mat srcImg);
	void updateBackground(cv::Mat srcImg, int bgThresholdValue, int faceThresholdValue);
	
private:
	int width, height;
	int bgThresholdValue;
	int fcThresholdValue;
	QFaceModel* facemodel;
	cv::Mat bgCurve;
	std::vector<cv::Mat> wholeFace;
	enum faceElement{NOSE, LEFTEYE, RIGHTEYE, LEFTBROW, RIGHTBROW, MOUTH, PROFIILE};
	void componentSketch(faceElement element, std::string componetName);
	QVector<Node*> getElementNodes(faceElement element);
	void combineSketch(bool combineFace = true);
	void addTopToBottom( cv::Mat &top, cv::Mat &botom);
	bool isWhite(cv::MatIterator_<cv::Vec3b> point);
	bool isBackground(cv::MatIterator_<cv::Vec3b> point);
	void combineComponent();
	std::vector<cv::Point> getLocatedFaceContour();
	void backgroudSketch(cv::Mat srcImg);
	cv::Mat getFaceMask();
	void backgroudSketch2( cv::Mat srcImg);
};

