#pragma once
#include <opencv2\core\core.hpp>
#include "QFaceModel.h"
#include <iostream>

class CFaceComponent
{
public:
	CFaceComponent(int templateIndex, QFaceModel* faceModel);
	~CFaceComponent(void);
	cv::Mat wrapTemplate(int width, int height);
protected:
	cv::Mat getTemplateImage(std::string templateIndexString, int width, int height);
	std::vector<cv::Point> getTemplatePoints(std::string templateIndexString);
	virtual std::vector<cv::Point> getLocatedPoints();
	int templateIndex;
	std::string templatePath;
	std::string folderName;
	std::string picNamePrefix;
	std::string ptsNamePrefix;
	QFaceModel* faceModel;
	int actualComponentPtsNum;
};

