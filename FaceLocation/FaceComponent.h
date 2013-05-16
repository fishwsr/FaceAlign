#pragma once

#include <opencv2\core\core.hpp>
#include "QFaceModel.h"
#include <fstream>

class CFaceComponent
{
public:
	CFaceComponent(int templateIndex, QFaceModel* faceModel);
	~CFaceComponent(void);
	virtual cv::Mat renderComponent(int width, int height);
	std::vector<cv::Point> getLocatedPointsToWrap();
	cv::Mat getRenderedImage() const { return warpedTemplate; }
	
protected:
	virtual std::vector<cv::Point> getTemplatePointsFromFile(std::ifstream* fin);
	virtual std::vector<cv::Point> getLocatedPoints();
	virtual QVector<Node*> getLocatedNodes() = 0;
	virtual std::vector<cv::Point> filterPoints(std::vector<cv::Point> points);
	virtual void preProcessTemplateImage();
	virtual void doRender(std::vector<cv::Point> templatePoints, cv::Mat templateMat);
	int templateIndex;
	std::string templatePath;
	std::string folderName;
	std::string picNamePrefix;
	std::string ptsNamePrefix;
	QFaceModel* faceModel;
	int actualComponentPtsNum;
	cv::Mat templateImg;
	cv::Mat warpedTemplate;
	std::vector<cv::Point> locatedPointsToWrap;
	std::vector<std::vector<cv::Point>> getTemplateKeyPoints();
private:
	cv::Mat getTemplateImage(std::string templateIndexString, int width, int height);
	std::vector<cv::Point> getTemplatePoints();
	
};

