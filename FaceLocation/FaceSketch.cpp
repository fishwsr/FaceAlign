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

void CFaceSketch::componentSketch(faceElement element, std::string componetName)
{
	string eyeTemplateNumber = "2";
	string browTemplateNumber = "7";
	string noseTemplateNumber = "4";
	string mouthTemplateNumber = "31";

	string templatePath = "sketchMode\\" + componetName + "\\";
	string modelPicPath = "";
	string ptsFilePath = "";
	
	//string savetemplatePath = "temp\\" + componetName + "\\";
	if(element == RIGHTBROW || element == LEFTBROW) {
		modelPicPath = templatePath + componetName + browTemplateNumber + ".jpg";
		ptsFilePath = templatePath + "eyebrow" + browTemplateNumber + ".pts";
	} else if(element == RIGHTEYE || element == LEFTEYE) {
		modelPicPath = templatePath + componetName + eyeTemplateNumber + ".jpg";
		ptsFilePath = templatePath + componetName + eyeTemplateNumber + ".pts";
	} else if(element == NOSE) {
		modelPicPath = templatePath +componetName+  noseTemplateNumber + ".jpg";
		ptsFilePath = templatePath + componetName + noseTemplateNumber + ".pts";
	} else if(element == MOUTH) {
		modelPicPath = templatePath + componetName + mouthTemplateNumber + ".jpg";
		ptsFilePath = templatePath + componetName + mouthTemplateNumber + ".pts";
	} else {
		modelPicPath = templatePath + componetName + "1.jpg";
		ptsFilePath = templatePath + componetName + "1.pts";
	}
	
	cv::Mat templateImg = cv::imread(modelPicPath, -1);
	cv::Mat bigTemplateImg(height, width, templateImg.type(), Scalar(1,2,3));
	Mat(templateImg, cvRect(0,0,templateImg.cols, templateImg.rows)).copyTo(Mat(bigTemplateImg, cvRect(0,0,templateImg.cols, templateImg.rows)));

	std::vector<cv::Point> cmpnControlPts;
	int cmpnPtsNum;
	int featurePointsNum;

	std::ifstream fin;
	
	
	fin.open(ptsFilePath);
	if(!fin)
	{
		cerr << "File Not Found " << templatePath;
		return;
	}
	fin >> featurePointsNum;
	cmpnPtsNum = featurePointsNum;
	if(element == NOSE) {
		cmpnPtsNum = 12;
	}
	
	cmpnControlPts.resize(cmpnPtsNum);

	for (int i = 0; i < featurePointsNum; i++)
	{
		double ptsX, ptsY;
		fin >> ptsX >>ptsY;
		int width = templateImg.cols;

		switch(element) {
		case NOSE:
			if(i < 6) {
				cmpnControlPts[i].x = ptsX;
				cmpnControlPts[i].y = ptsY; 
			}else if(i == 6) {
				continue;
			} else if(i > 6) {
				cmpnControlPts[i-1].x = ptsX;
				cmpnControlPts[i-1].y = ptsY;
			}
			break;
		case RIGHTBROW:
			cmpnControlPts[i].x = width - ptsX;
			cmpnControlPts[i].y = ptsY;
			break;
		default:
			cmpnControlPts[i].x = ptsX;
			cmpnControlPts[i].y = ptsY;
		}
	}
	fin.close();

	QVector<Node*> cmpnNodes;
	cmpnNodes = getElementNodes(element);
	std::vector<cv::Point> nosePts;
	nosePts.resize(cmpnPtsNum);
	for(int i = 0; i<cmpnPtsNum; i++)
	{
		switch(element) {
		case LEFTBROW:
			if(i == 0) {
				nosePts[i].x = cmpnNodes[9]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[9]->sceneBoundingRect().center().y();
			} else if ( i < 5){
				nosePts[i].x = cmpnNodes[i]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[i]->sceneBoundingRect().center().y();
			} else if ( i >= 5) {
				nosePts[i].x = cmpnNodes[i+1]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[i+1]->sceneBoundingRect().center().y();
			}
			break;
		case RIGHTBROW:
			if(i == 0) {
				nosePts[i].x = cmpnNodes[4]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[4]->sceneBoundingRect().center().y();
			} else if ( i < 4){
				nosePts[i].x = cmpnNodes[4-i]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[4-i]->sceneBoundingRect().center().y();
			} else if ( i >= 4) {
				nosePts[i].x = cmpnNodes[13-i]->sceneBoundingRect().center().x();
				nosePts[i].y = cmpnNodes[13-i]->sceneBoundingRect().center().y();
			}
			break;
		default:
			nosePts[i].x = cmpnNodes[i]->sceneBoundingRect().center().x();
			nosePts[i].y = cmpnNodes[i]->sceneBoundingRect().center().y();
		}
	}
	CThinPlateSpline tps(cmpnControlPts,nosePts);
	Mat warpedTemplate;
	tps.warpImage(bigTemplateImg, warpedTemplate,0.01,INTER_CUBIC,BACK_WARP);
	wholeFace.push_back(warpedTemplate);
	//cv::imwrite(savetemplatePath + "t1.jpg", bigTemplateImg);
	//cv::imwrite(savetemplatePath + "r1.jpg", warpedTemplate);
	
	//string s = savetemplatePath + "r1.jpg";
	//IplImage *warpedTemplate2 = cvLoadImage(s.c_str());
	//cvFloodFill(warpedTemplate2, cvPoint(0,0), cvScalarAll(255), cvScalarAll(0), cvScalarAll(255), 0, 8, 0);
	//cv::imwrite(savetemplatePath + "r2.jpg", warpedTemplate2);
}

void CFaceSketch::sketchFace( QFaceModel* ASMModel, int imgwidth, int imgheight )
{
	width = imgwidth;
	height = imgheight;
	facemodel = ASMModel;
	componentSketch(LEFTEYE, "leftEye");
	componentSketch(RIGHTEYE, "rightEye");
	componentSketch(LEFTBROW, "leftEyeBrow");
	componentSketch(RIGHTBROW, "rightEyeBrow");
	componentSketch(NOSE, "nose");
	componentSketch(MOUTH, "mouth");
	componentSketch(PROFIILE, "faceContour");
	combineSketch();
}

QVector<Node*> CFaceSketch::getElementNodes( faceElement element )
{
	switch(element)
	{
	case NOSE:return facemodel->getNoseNodes();
	case LEFTEYE:return facemodel->getLeftEyeNodes();
	case RIGHTEYE:return facemodel->getRightEyeNodes();
	case LEFTBROW:return facemodel->getLeftBrowNodes();
	case RIGHTBROW:return facemodel->getRightBrowNodes();
	case MOUTH:return facemodel->getMouthNodes();
	case PROFIILE:return facemodel->getFaceProfileNodes();
	}
}

boolean CFaceSketch::isWhite(MatIterator_<Vec3b> point){
	return ((*point)[0] == 255) && ((*point)[1] == 255) && ((*point)[2] == 255);
}

boolean CFaceSketch::isBackground(MatIterator_<Vec3b> point){
	return ((*point)[0] == 1) && ((*point)[1] == 2) && ((*point)[2] == 3);
}

void CFaceSketch::combineSketch()
{
	Mat face(height, width, CV_8UC3, Scalar(1,2,3));
	addTopToBottom(wholeFace[LEFTBROW], face);
	addTopToBottom(wholeFace[RIGHTBROW], face);
	addTopToBottom(wholeFace[MOUTH], face);
	addTopToBottom(wholeFace[LEFTEYE], face);
	addTopToBottom(wholeFace[RIGHTEYE], face);
	addTopToBottom(wholeFace[NOSE], face);
	addTopToBottom(face, wholeFace[PROFIILE]);
	Mat whole(height, width, CV_8UC3, Scalar::all(255));
	addTopToBottom(wholeFace[PROFIILE], whole);
	
	namedWindow("bbb", 1);
	imshow("bbb", whole);
	cv::imwrite("temp\\wholeSketch.jpg", whole);
}

void CFaceSketch::addTopToBottom( Mat &top, Mat &botom) 
{
	MatIterator_<Vec3b> itOfTop, itOfBottom, endOfTop, endOfBottom;
	itOfTop = top.begin<Vec3b>();
	endOfTop = top.end<Vec3b>();
	itOfBottom = botom.begin<Vec3b>();
	endOfBottom = botom.end<Vec3b>();

	for( ;(itOfTop != endOfTop) && (itOfBottom != endOfBottom); ++itOfTop, ++itOfBottom)
	{

		boolean noPixelOfFace = isBackground(itOfTop) || isWhite(itOfTop);

		if(!noPixelOfFace){
			(*itOfBottom)[0] = (*itOfTop)[0];
			(*itOfBottom)[1] = (*itOfTop)[1];
			(*itOfBottom)[2] = (*itOfTop)[2];
		} 
	}

}




