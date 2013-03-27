#pragma once
#include "QFaceModel.h"
#include <string>
#include "cv.h"

class CFaceSketch
{
public:
	CFaceSketch(void);
	~CFaceSketch(void);
	void sketchFace( QFaceModel* ASMModel, int width, int height);
private:
	int width, height;
	QFaceModel* facemodel;
	std::vector<cv::Mat> wholeFace;
	enum faceElement{NOSE, LEFTEYE, RIGHTEYE, LEFTBROW, RIGHTBROW, MOUTH, PROFIILE};
	void componentSketch(faceElement element, std::string componetName);
	QVector<Node*> getElementNodes(faceElement element);
	void combineSketch();
};

