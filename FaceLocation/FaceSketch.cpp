#include "FaceSketch.h"
#include "CThinPlateSpline.h"
#include "Node.h"
#include <QVector>
#include <fstream>
#include <iostream>
#include "highgui.h"
#include <math.h>
#include "LeftBrowComponent.h"
#include "RightBrowComponent.h"
#include "LeftEyeComponent.h"
#include "RightEyeComponent.h"
#include "NoseComponent.h"
#include "MouthComponent.h"
#include "FaceContourComponent.h"

CFaceSketch::CFaceSketch()
{
	bgThresholdValue = 60;
	fcThresholdValue = 80;
	eyeIndex = 1;
	browIndex = 7;
	noseIndex = 1;
	mouthIndex = 31;
	faceIndex = 1;
	qDebug("FaceSketch Created. FaceSketch Created. FaceSketch Created.\n");
}


CFaceSketch::~CFaceSketch(void)
{
}

void CFaceSketch::componentSketch(faceElement element, std::string componetName)
{
	string eyeTemplateNumber = QString::number(eyeIndex).toLocal8Bit();
	string browTemplateNumber = QString::number(browIndex).toLocal8Bit();
	string noseTemplateNumber = QString::number(noseIndex).toLocal8Bit();
	string mouthTemplateNumber = QString::number(mouthIndex).toLocal8Bit();
	string faceTempalteNumber = QString::number(faceIndex).toLocal8Bit();

	string templatePath = "data\\colorMode\\" + componetName + "\\";
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
		modelPicPath = templatePath + componetName + faceTempalteNumber + ".jpg";
		ptsFilePath = templatePath + componetName +  faceTempalteNumber + ".pts";
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
		std::cerr << "File Not Found " << templatePath;
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

cv::Mat CFaceSketch::sketchFace( QFaceModel* ASMModel, cv::Mat srcImg)
{
	double t = (double)getTickCount();
	qDebug("Face Threshold %i, BG Threshold %i", fcThresholdValue, bgThresholdValue);
	facemodel = ASMModel;
	width = srcImg.cols;
	height = srcImg.rows;
	wholeFace.clear();
	
	CLeftEyeComponent leftEyeComponent(eyeIndex, facemodel);
	Mat leftEye = leftEyeComponent.wrapTemplate(width, height);
	wholeFace.push_back(leftEye);

	CRightEyeComponent rightEyeComponent(eyeIndex, facemodel);
	Mat rightEye = rightEyeComponent.wrapTemplate(width, height);
	wholeFace.push_back(rightEye);
	
	CLeftBrowComponent leftBrowComponent(browIndex, facemodel);
	Mat leftBrow = leftBrowComponent.wrapTemplate(width, height);
	wholeFace.push_back(leftBrow);

	CRightBrowComponent rightBrowComp(browIndex, facemodel);
	Mat rightBrow = rightBrowComp.wrapTemplate(width, height);
	wholeFace.push_back(rightBrow);

	CNoseComponent noseComp(noseIndex, facemodel);
	Mat nose = noseComp.wrapTemplate(width, height);
	wholeFace.push_back(nose);

	CMouthComponent mouthComp(mouthIndex, facemodel);
	Mat mouth = mouthComp.wrapTemplate(width, height);
	wholeFace.push_back(mouth);

	CFaceContourComponent faceContourComp(faceIndex, facemodel);
	Mat faceContour = faceContourComp.wrapTemplate(width, height);
	wholeFace.push_back(faceContour);

	backgroudSketch(srcImg);
	combineSketch();
	t = ((double)getTickCount() - t)/getTickFrequency();
	qDebug("Sketch -- Times passed in seconds: %f\n", t);
	return bgCurve;
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

bool CFaceSketch::isWhite(MatIterator_<Vec3b> point){
	return ((*point)[0] == 255) && ((*point)[1] == 255) && ((*point)[2] == 255);
}

bool CFaceSketch::isBackground(MatIterator_<Vec3b> point){
	return ((*point)[0] == 1) && ((*point)[1] == 2) && ((*point)[2] == 3);
}

void CFaceSketch::combineComponent()
{
	Mat face(height, width, CV_8UC3, Scalar(1,2,3));
	addTopToBottom(wholeFace[LEFTBROW], face);
	addTopToBottom(wholeFace[RIGHTBROW], face);
	addTopToBottom(wholeFace[MOUTH], face);
	addTopToBottom(wholeFace[LEFTEYE], face);
	addTopToBottom(wholeFace[RIGHTEYE], face);
	addTopToBottom(wholeFace[NOSE], face);
	addTopToBottom(face, wholeFace[PROFIILE]);

	Mat facialSetch(height, width, CV_8UC3, Scalar::all(255));
	addTopToBottom(wholeFace[PROFIILE], facialSetch);
	cv::imwrite("temp\\wholeFace.jpg",facialSetch);
}
void CFaceSketch::combineSketch(bool combineFace)
{
	if (combineFace == true)
	{
		combineComponent();
	}
	Mat facialSketch = imread("temp\\wholeFace.jpg");
	addTopToBottom(facialSketch, bgCurve);
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

		bool noPixelOfFace = isBackground(itOfTop) || isWhite(itOfTop);

		if(!noPixelOfFace){
			(*itOfBottom)[0] = (*itOfTop)[0];
			(*itOfBottom)[1] = (*itOfTop)[1];
			(*itOfBottom)[2] = (*itOfTop)[2];
		} 
	}

}

void CFaceSketch::updateBackground(cv::Mat srcImg, int bgThresholdValue, int fcThresholdValue )
{
	this->bgThresholdValue = bgThresholdValue;
	this->fcThresholdValue = fcThresholdValue;
	width = srcImg.cols;
	height = srcImg.rows;
	backgroudSketch(srcImg);
	combineSketch(false);
	imwrite("temp\\wholeSketch.jpg",bgCurve);
}

void CFaceSketch::backgroudSketch2( cv::Mat srcImg)
{
	vector<Point> faceOutLine = getLocatedFaceContour();

	Mat tempImg1, tempImg2;
	cv::Canny(srcImg, tempImg1, bgThresholdValue, bgThresholdValue*3, 3);
	cv::bitwise_not(tempImg1, tempImg2);
	cv::cvtColor(tempImg2, bgCurve, CV_GRAY2BGR );

	Mat faceTempImg1, faceTempImg2, faceBgCurv;
	cv::Canny(srcImg, faceTempImg1, fcThresholdValue, fcThresholdValue*3, 3);
	cv::bitwise_not(faceTempImg1, faceTempImg2);
	cv::cvtColor(faceTempImg2, faceBgCurv, CV_GRAY2BGR );

	MatIterator_<Vec3b> itOfBg, itOfFace, endOfBg, endOfFace;
	itOfBg = bgCurve.begin<Vec3b>();
	endOfBg = bgCurve.end<Vec3b>();
	itOfFace = faceBgCurv.begin<Vec3b>();
	endOfFace = faceBgCurv.end<Vec3b>();

	for( ;(itOfBg != endOfBg) && (itOfFace != endOfFace); ++itOfFace, ++itOfBg)
	{
		double isFacePoint = pointPolygonTest( faceOutLine, Point2f(itOfBg.pos().x,itOfBg.pos().y), false );
		
		if(isFacePoint > 0){
			(*itOfBg)[0] = (*itOfFace)[0];
			(*itOfBg)[1] = (*itOfFace)[1];
			(*itOfBg)[2] = (*itOfFace)[2];
		} 
	}
	
}


std::vector<cv::Point> CFaceSketch::getLocatedFaceContour()
{
	
	QVector<Node*> allQNodes = facemodel->getAllNodes();
	// get face points
	std::vector<cv::Point> points;
	int count = allQNodes.size();
	for( int i = 0; i < count; i++ )
	{
		Point pt;
		pt.x = allQNodes.at(i)->sceneBoundingRect().center().x();
		pt.y = allQNodes.at(i)->sceneBoundingRect().center().y();

		points.push_back(pt);
	}

	//get hull
	vector<int> hull;
	convexHull(cv::Mat(points), hull, true);

	Mat img(width, height, CV_8UC1, Scalar::all(0));
	/*for(int i = 0; i < count; i++ )
		circle(img, points[i], 3, Scalar(255), CV_FILLED, CV_AA);*/

	int hullcount = (int)hull.size();
	Point pt0 = points[hull[hullcount-1]];

	for(int i = 0; i < hullcount; i++ )
	{
		Point pt = points[hull[i]];
		line(img, pt0, pt, Scalar( 255), 3, 8);
		pt0 = pt;
	}

	/// Get the contours
	vector<vector<Point> > contours;

	findContours( img.clone(), contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	
	Mat img2(width, height, CV_8UC1, Scalar::all(0));

	Point pt00 = contours[0][contours[0].size()-1];
	for(int i = 0; i <  contours[0].size(); i++) {
		Point pt = contours[0][i];
		line(img2, pt00, pt, Scalar(255), 1, CV_AA);
		pt00 = pt;
	}
	
	return contours[0];
}

Mat CFaceSketch::getFaceMask()
{
	cv::Mat faceMask(height, width,CV_8U, Scalar(0));
	QVector<Node*> allQNodes = facemodel->getAllNodes();
	// get face points
	std::vector<cv::Point> points;
	int count = allQNodes.size();
	for( int i = 0; i < count; i++ )
	{
		Point pt;
		pt.x = allQNodes.at(i)->sceneBoundingRect().center().x();
		pt.y = allQNodes.at(i)->sceneBoundingRect().center().y();

		points.push_back(pt);
	}
	Rect bdRect = boundingRect(points);
	Mat maskROI(faceMask,bdRect);
	maskROI.setTo(1);
	return faceMask;
}

void CFaceSketch::backgroudSketch(cv::Mat srcImg)
{
	Mat tempImg1, tempImg2;
	cv::Canny(srcImg, tempImg1, bgThresholdValue, bgThresholdValue*3, 3);

	Mat faceBgCurv;
	cv::Canny(srcImg, faceBgCurv, fcThresholdValue, fcThresholdValue*3, 3);

	Mat faceMask = getFaceMask();
	//Mat tempImg3 = tempImg1;
	cv::bitwise_and(tempImg1,faceBgCurv,tempImg1,faceMask);
	cv::bitwise_not(tempImg1, tempImg2);
	cv::cvtColor(tempImg2, bgCurve, CV_GRAY2BGR);
}