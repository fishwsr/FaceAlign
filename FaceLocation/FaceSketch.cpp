#include "FaceSketch.h"
#include "CThinPlateSpline.h"
#include "Node.h"
#include <QVector>
#include <fstream>
#include <iostream>
#include "highgui.h"

CFaceSketch::CFaceSketch(void)
{
}


CFaceSketch::~CFaceSketch(void)
{
}

void CFaceSketch::componentSketch( std::string componetName, QFaceModel* facemodel, int width, int height )
{
	string templatePath = "colorMode\\" + componetName + "\\" +componetName + "1";
	string savetemplatePath = "temp\\" + componetName + "\\";
	cv::Mat templateImg = cv::imread(templatePath + ".jpg");
	cv::Mat bigTemplateImg(width, height, templateImg.type(),Scalar::all(0));
	Mat(templateImg, cvRect(0,0,templateImg.cols, templateImg.rows)).copyTo(Mat(bigTemplateImg, cvRect(0,0,templateImg.cols, templateImg.rows)));

	std::vector<cv::Point> cmpnControlPts;
	int cmpnPtsNum;
	std::ifstream fin;
	fin.open(templatePath + ".pts");
	if(!fin)
	{
		return;
	}
	fin >> cmpnPtsNum;
	cmpnControlPts.resize(cmpnPtsNum);
	for (int i = 0; i < cmpnPtsNum; i++)
	{
		double ptsX, ptsY;
		fin >> ptsX >>ptsY;
		cmpnControlPts[i].x = ptsX;
		cmpnControlPts[i].y = ptsY; 
	}
	fin.close();

	QVector<Node*> cmpnNodes;
	cmpnNodes = facemodel->getNoseNodes();
	std::vector<cv::Point> nosePts;
	nosePts.resize(cmpnPtsNum);
	for(int i = 0; i<cmpnPtsNum; i++)
	{
		nosePts[i].x = cmpnNodes[i]->sceneBoundingRect().center().x();
		nosePts[i].y = cmpnNodes[i]->sceneBoundingRect().center().y();
	}
	CThinPlateSpline tps(cmpnControlPts,nosePts);
	Mat warpedTemplate;
	tps.warpImage(bigTemplateImg, warpedTemplate,0.01,INTER_CUBIC,BACK_WARP);
	cv::imwrite(savetemplatePath + "t1.jpg", bigTemplateImg);
	cv::imwrite(savetemplatePath + "r1.jpg", warpedTemplate);
}
