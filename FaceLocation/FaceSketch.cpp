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


cv::Mat CFaceSketch::sketchFace( QFaceModel* ASMModel, cv::Mat srcImg)
{
	double t = (double)getTickCount();
	qDebug("Face Threshold %i, BG Threshold %i", fcThresholdValue, bgThresholdValue);
	facemodel = ASMModel;
	width = srcImg.cols;
	height = srcImg.rows;
	wholeFace.clear();

	std::vector<CFaceComponent*> faceComps;
	CLeftEyeComponent leftEyeComp(eyeIndex, facemodel);
	faceComps.push_back(&leftEyeComp);
	CRightEyeComponent rightEyeComp(eyeIndex, facemodel);
	faceComps.push_back(&rightEyeComp);
	CLeftBrowComponent leftBrowComp(browIndex, facemodel);
	faceComps.push_back(&leftBrowComp);
	CRightBrowComponent rightBrowComp(browIndex, facemodel);
	faceComps.push_back(&rightBrowComp);
	CNoseComponent noseComp(noseIndex, facemodel);
	faceComps.push_back(&noseComp);
	CMouthComponent mouthComp(mouthIndex, facemodel);
	faceComps.push_back(&mouthComp);
	CFaceContourComponent faceContourComp(faceIndex, facemodel);
	faceComps.push_back(&faceContourComp);

	for(int i = 0; i < faceComps.size(); i++) {
		Mat compMat = faceComps[i]->wrapTemplate(width, height);
		wholeFace.push_back(compMat);
	}

	backgroudSketch(srcImg);
	backgroundColor(srcImg);
	combineSketch();
	t = ((double)getTickCount() - t)/getTickFrequency();
	qDebug("Sketch -- Times passed in seconds: %f\n", t);
	return bgColor;
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
	addTopToBottom(bgCurve, bgColor);
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
	backgroundColor(srcImg);
	combineSketch(false);
	imwrite("temp\\wholeSketch.jpg",bgColor);
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

void CFaceSketch::backgroundColor( cv::Mat srcImg )
{
	bgColor = srcImg.clone();
	for (int i = 0; i < 8; i++)
	{
		swap(srcImg, bgColor);
		bilateralFilter(srcImg, bgColor, 5, 150, 150);		
	}
	colorQuantization();
}

void CFaceSketch::colorQuantization()
{
	double q = 25, qFy = 4;
	double qNearest;
	double luminNow;
	double quantization;
	for(int j = 0; j < bgColor.rows; j++)
	{
		uchar *data = bgColor.ptr<uchar>(j);
		for(int i = 0; i < bgColor.cols; i++)
		{		
			luminNow = (double)(*data)*100/255;
			qNearest = floor(luminNow/q) * q;
			quantization = qNearest + (double)(q/2)*tanh(qFy*(luminNow-qNearest));

			if(quantization > 100)
			{
				*data = 255;
			}
			else
			{
				int a = (int)quantization*255/100;
				*data = a;
			}
			data = data + 3;
		}
	}
}
