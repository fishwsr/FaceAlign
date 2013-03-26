#pragma once
#include "QFaceModel.h"
#include <string>
#include "cv.h"

class CFaceSketch
{
public:
	CFaceSketch(void);
	~CFaceSketch(void);
	void componentSketch(std::string componetName, QFaceModel* facemodel, int width, int height);
};

