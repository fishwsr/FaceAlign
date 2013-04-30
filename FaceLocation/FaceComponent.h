#pragma once

#include <opencv2\core\core.hpp>
#include "QFaceModel.h"
#include <fstream>

class CFaceComponent
{
public:
	CFaceComponent(int templateIndex, QFaceModel* faceModel);
	~CFaceComponent(void);
	cv::Mat wrapTemplate(int width, int height);
protected:
	virtual std::vector<cv::Point> getTemplatePointsFromFile(std::ifstream* fin);
	virtual std::vector<cv::Point> getLocatedPoints();
	virtual QVector<Node*> getLocatedNodes();
	int templateIndex;
	std::string templatePath;
	std::string folderName;
	std::string picNamePrefix;
	std::string ptsNamePrefix;
	QFaceModel* faceModel;
	int actualComponentPtsNum;
	cv::Mat templateImg;
private:
	cv::Mat getTemplateImage(std::string templateIndexString, int width, int height);
	std::vector<cv::Point> getTemplatePoints(std::string templateIndexString);
};

