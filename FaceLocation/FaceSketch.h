#pragma once
#include "QFaceModel.h"
#include <iostream>
#include "cv.h"

class CFaceSketch
{
public:
	CFaceSketch(int imgwidth, int imgheight);
	~CFaceSketch(void);
	void sketchFace( QFaceModel* ASMModel, std::string srcImgPath );
private:
	int width, height;
	QFaceModel* facemodel;
	cv::Mat bgCurve;
	std::vector<cv::Mat> wholeFace;
	enum faceElement{NOSE, LEFTEYE, RIGHTEYE, LEFTBROW, RIGHTBROW, MOUTH, PROFIILE};
	void componentSketch(faceElement element, std::string componetName);
	QVector<Node*> getElementNodes(faceElement element);
	void backgroudSketch(std::string srcImgPath);
	void combineSketch();
	void addTopToBottom( cv::Mat &top, cv::Mat &botom);
	bool isWhite(cv::MatIterator_<cv::Vec3b> point);
	bool isBackground(cv::MatIterator_<cv::Vec3b> point);
};

