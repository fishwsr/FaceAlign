#include "FaceComponent.h"
#include <opencv2\imgproc\imgproc.hpp>
#include "CThinPlateSpline.h"
#include <QString>
#include <opencv2\highgui\highgui.hpp>
#include "UIParams.h"

CFaceComponent::CFaceComponent(int templateIndex, QFaceModel* faceModel)
{
	this->templateIndex = templateIndex;
	this->templatePath = "data\\sketchMode\\";
	this->faceModel = faceModel;
}


CFaceComponent::~CFaceComponent(void)
{
}

cv::Mat CFaceComponent::renderComponent(int width, int height)
{
	std::string templateIndexString = QString::number(templateIndex).toLocal8Bit();
	cv::Mat templateMat = getTemplateImage(templateIndexString, width, height);
	std::vector<cv::Point> templatePoints = filterPoints(getTemplatePoints());
	locatedPointsToWrap = filterPoints(getLocatedPoints());
	
	if(UIParams::hasColor) {
		renderComponentInColor(templatePoints, templateMat, width, height);
	} else {
		renderComponentInGray(templatePoints, templateMat, width, height);
	}

	return warpedTemplate;
}


void CFaceComponent::renderComponentInGray( std::vector<cv::Point> templatePoints, cv::Mat templateMat,int width, int height )
{
	CThinPlateSpline tps(templatePoints,locatedPointsToWrap);
	tps.warpImage(templateMat, warpedTemplate,0.01,INTER_CUBIC,BACK_WARP);
}


void CFaceComponent::renderComponentInColor( std::vector<cv::Point> templatePoints, cv::Mat templateMat, int width, int height )
{
	return renderComponentInGray(templatePoints, templateMat, width, height);
}


cv::Mat CFaceComponent::getTemplateImage(std::string templateIndexString, int width, int height)
{
	std::string modelPicPath = templatePath + folderName + "\\" + picNamePrefix +  templateIndexString + ".jpg";
	templateImg = cv::imread(modelPicPath, -1);
	if(templateImg.empty()){
		qDebug("Template Image can not be found");
		exit(-1);
	};

	if(UIParams::hasColor) {
		preProcessTemplateImage();
	}
	
	cv::Mat bigTemplateImg(height, width, templateImg.type(), Scalar(1,2,3));
	Mat(templateImg, cvRect(0,0,templateImg.cols, templateImg.rows)).copyTo(Mat(bigTemplateImg, cvRect(0,0,templateImg.cols, templateImg.rows)));
	return bigTemplateImg;
}

void CFaceComponent::preProcessTemplateImage()
{
	//do nothing currently
}

std::vector<cv::Point> CFaceComponent::getTemplatePoints()
{
	std::string templateIndexString = QString::number(templateIndex).toLocal8Bit();
	std::string ptsFilePath = templatePath + folderName + "\\" + ptsNamePrefix +  templateIndexString + ".pts";
	std::ifstream fin;

	fin.open(ptsFilePath);
	if(!fin)
	{
		qDebug("PTS File Not Found ");
		exit(-1);
	}
	
	return getTemplatePointsFromFile(&fin);
}

std::vector<cv::Point> CFaceComponent::getTemplatePointsFromFile(std::ifstream* fin)
{
	int ptsNumInFile;
	*fin >> ptsNumInFile;

	std::vector<cv::Point> templatePoints;
	if(actualComponentPtsNum < ptsNumInFile){
		actualComponentPtsNum = ptsNumInFile;
	}
	templatePoints.resize(actualComponentPtsNum);

	for (int i = 0; i < ptsNumInFile; i++) {
		double ptsX, ptsY;
		*fin >> ptsX >>ptsY;
		templatePoints[i].x = ptsX;
		templatePoints[i].y = ptsY;
	}
	return templatePoints;
}

std::vector<cv::Point> CFaceComponent::getLocatedPoints()
{
	QVector<Node*> locatedNodes = getLocatedNodes();
	std::vector<cv::Point> locatedPointsToUse;
	locatedPointsToUse.resize(actualComponentPtsNum);
	for(int i = 0; i<actualComponentPtsNum; i++) {
		locatedPointsToUse[i].x = locatedNodes[i]->sceneBoundingRect().center().x();
		locatedPointsToUse[i].y = locatedNodes[i]->sceneBoundingRect().center().y();
	}

	return locatedPointsToUse;
}

std::vector<cv::Point> CFaceComponent::filterPoints( std::vector<cv::Point> points )
{
	std::vector<cv::Point> filteredPoints;

	for(int i = 0; i < points.size(); i++){
		if(i%2 == 0) {
			filteredPoints.push_back(points.at(i));
		}
	}
	return filteredPoints;
}

std::vector<cv::Point> CFaceComponent::getLocatedPointsToWrap()
{
	return locatedPointsToWrap;
}

std::vector<std::vector<cv::Point>> CFaceComponent::getTemplateKeyPoints()
{
	std::string templateIndexString = QString::number(templateIndex).toLocal8Bit();
	std::string ptsFilePath = templatePath + folderName + "\\keyPoints" +  templateIndexString + ".pts";
	std::ifstream fin;

	fin.open(ptsFilePath);
	if(!fin)
	{
		qDebug("PTS File Not Found ");
		exit(-1);
	}

	int lineNumInFile;
	fin >> lineNumInFile;

	std::vector<vector<cv::Point>> templateKeyPoints;
	templateKeyPoints.resize(lineNumInFile);
	for (int i = 0; i < lineNumInFile; i++) 
	{
		int ptsNumInLine;
		fin >> ptsNumInLine;
		templateKeyPoints[i].resize(ptsNumInLine);
		for(int j = 0; j < ptsNumInLine; j++)
		{
			double ptsX, ptsY;
			fin >> ptsX >>ptsY;
			templateKeyPoints[i][j].x = ptsX;
			templateKeyPoints[i][j].y = ptsY;
		}
	}
	return templateKeyPoints;
}