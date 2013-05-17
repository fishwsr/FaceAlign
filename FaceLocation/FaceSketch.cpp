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
#include <omp.h>
#include <QMap>

CFaceSketch::CFaceSketch()
{
	bgThresholdValue = 60;
	fcThresholdValue = 80;
	qtzThresholdValue = 15;
	hasColor = true;
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


cv::Mat CFaceSketch::sketchFace( QFaceModel* ASMModel, cv::Mat srcImg, bool isRenderVideo, int bgThresholdValue, int faceThresholdValue)
{
	double t = (double)getTickCount();
	//qDebug("Face Threshold %i, BG Threshold %i", fcThresholdValue, bgThresholdValue);
	this->bgThresholdValue = bgThresholdValue;
	this->qtzThresholdValue = qtzThresholdValue;

	facemodel = ASMModel;
	width = srcImg.cols;
	height = srcImg.rows;
	wrappedFaceCompMap.clear();

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
	//no need to wrap face contour when there is no color
	if(!hasColor || !isRenderVideo) {
		faceComps.push_back(&faceContourComp);
	}

	pointsToWrap.clear();
	wrappedFaceCompMap.clear();
	int faceCompNumber = faceComps.size();

	double t1 = (double)getTickCount();
	#pragma omp parallel for
	for(int i = 0; i < faceCompNumber; ++i) {
		faceComps[i]->renderComponent(width, height);
	}

	for(int i = 0; i < faceCompNumber; ++i) {
		wrappedFaceCompMap.insert(i, faceComps[i]->getRenderedImage());
		std::vector<cv::Point> pts = faceComps[i]->getLocatedPointsToWrap();
		for(int j = 0; j < pts.size(); j++) {
			pointsToWrap.push_back(pts[j]);
		}
	}
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("Wrap -- Times passed in seconds: %f\n", t1);
	
	t1 = (double)getTickCount();
	backgroudSketch(srcImg);
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("BG Sketch -- Times passed in seconds: %f\n", t1);

	t1 = (double)getTickCount();
	backgroundColor(srcImg);
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("BG Color -- Times passed in seconds: %f\n", t1);

	t1 = (double)getTickCount();
	combineSketch();
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("Combine -- Times passed in seconds: %f\n", t1);


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
	addTopToBottom(wrappedFaceCompMap[LEFTBROW], face);
	addTopToBottom(wrappedFaceCompMap[RIGHTBROW], face);
	addTopToBottom(wrappedFaceCompMap[LEFTEYE], face);
	addTopToBottom(wrappedFaceCompMap[RIGHTEYE], face);
	addTopToBottom(wrappedFaceCompMap[NOSE], face);
	if(7 == wrappedFaceCompMap.size())
	{
		//wrappedFaceCompMap[PROFIILE]->w
		addTopToBottom(face, wrappedFaceCompMap[PROFIILE]);
		face = wrappedFaceCompMap[PROFIILE].clone();
	}
	
	Mat facialSketch(height, width, CV_8UC3, Scalar::all(255));
	addTopToBottom(face, facialSketch);
	fixTemplate(facialSketch);
	addTopToBottom(wrappedFaceCompMap[MOUTH], facialSketch);
	/*int size = 1;
	Mat element = getStructuringElement( MORPH_RECT,	Size( 2*size+1, 2*size+1 ),Point( size, size ) );
	erode(facialSetch, facialSetch, element);*/
	wrappedFaceCompMap[ALL] = facialSketch;
	cv::imwrite("temp\\wholeFace.jpg",facialSketch);
}

void CFaceSketch::combineSketch(bool combineFace)
{
	if (combineFace == true)
	{
		combineComponent();
	}
	Mat facialSketch = wrappedFaceCompMap[ALL];
	double t1 = (double)getTickCount();
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("Fix Template -- Times passed in seconds: %f\n", t1);
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

		bool noPixelOfFace = isBackground(itOfTop) || ( isWhite(itOfTop));

		if(!noPixelOfFace){
			(*itOfBottom)[0] = (*itOfTop)[0];
			(*itOfBottom)[1] = (*itOfTop)[1];
			(*itOfBottom)[2] = (*itOfTop)[2];
		} 
	}

}

void CFaceSketch::updateBackground(cv::Mat srcImg, int bgThresholdValue, int qtzThresholdValue)
{
	this->bgThresholdValue = bgThresholdValue;
	this->qtzThresholdValue = qtzThresholdValue;
	width = srcImg.cols;
	height = srcImg.rows;
	backgroudSketch(srcImg);
	backgroundColor(srcImg);
	combineSketch();
	imwrite("temp\\wholeSketch.jpg",bgColor);
}

void CFaceSketch::backgroudSketch( cv::Mat srcImg)
{
	//set faceCurevOn true to get the curve on the face
	bool faceCurveOn = false;
	vector<Point> faceOutLine = getLocatedFaceContour();

	backgroundSmooth(srcImg);
	Mat tempImg0;
	tempImg0 = bgColor.clone();

	Mat tempImg1, tempImg2;
	cv::Canny(tempImg0, tempImg1, bgThresholdValue, bgThresholdValue*3, 3);
	cv::bitwise_not(tempImg1, tempImg2);
	cv::cvtColor(tempImg2, bgCurve, CV_GRAY2BGR );

	Mat  faceTempImg1, faceTempImg2, faceBgCurv;
	cv::Canny(tempImg0, faceTempImg1, fcThresholdValue, fcThresholdValue*3, 3);
	cv::bitwise_not(faceTempImg1, faceTempImg2);
	cv::cvtColor(faceTempImg2, faceBgCurv, CV_GRAY2BGR );
	if(!faceCurveOn)
	{
		faceBgCurv.setTo(255);
	}
	MatIterator_<Vec3b> itOfBg, itOfFace, endOfBg, endOfFace;
	itOfBg = bgCurve.begin<Vec3b>();
	endOfBg = bgCurve.end<Vec3b>();
	itOfFace = faceBgCurv.begin<Vec3b>();
	endOfFace = faceBgCurv.end<Vec3b>();

	double t1 = (double)getTickCount();
	for( ;(itOfBg != endOfBg) && (itOfFace != endOfFace); ++itOfFace, ++itOfBg)
	{
		double isFacePoint = pointPolygonTest( faceOutLine, Point2f(itOfBg.pos().x,itOfBg.pos().y), false );
		
		if(isFacePoint > 0){
			(*itOfBg)[0] = (*itOfFace)[0];
			(*itOfBg)[1] = (*itOfFace)[1];
			(*itOfBg)[2] = (*itOfFace)[2];
		} 
	}
	t1 = ((double)getTickCount() - t1)/getTickFrequency();
	qDebug("Face Point -- Times passed in seconds: %f\n", t1);

	
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

void CFaceSketch::backgroudSketch2(cv::Mat srcImg)
{
	Mat tempImg1, tempImg2;
	cv::Canny(srcImg, tempImg1, bgThresholdValue, bgThresholdValue*3, 3);

	Mat faceBgCurv;
	cv::Canny(srcImg, faceBgCurv, fcThresholdValue, fcThresholdValue*3, 3);

	Mat faceMask = getFaceMask();
	//Mat tempImg3 = tempImg1;
	//faceBgCurv.setTo(1);
	cv::bitwise_and(tempImg1,faceBgCurv,tempImg1,faceMask);
	cv::bitwise_not(tempImg1, tempImg2);
	cv::cvtColor(tempImg2, bgCurve, CV_GRAY2BGR);
}

void CFaceSketch::backgroundColor( cv::Mat srcImg )
{
	if(!hasColor){
		bgColor.setTo(255);
		return;
	}
	Mat temp;
	temp = bgColor.clone();
	cvtColor(temp, bgColor, CV_RGB2Lab);
	colorQuantization();
	temp = bgColor.clone();
	cvtColor(temp, bgColor, CV_Lab2RGB);
}

void CFaceSketch::colorQuantization()
{
	double dq = qtzThresholdValue, qFy = 4;
	double qNearest;
	double luminNow;
	double quantization;
	for(int j = 0; j < bgColor.rows; j++)
	{
		uchar *data = bgColor.ptr<uchar>(j);
		for(int i = 0; i < bgColor.cols; i++)
		{		
			luminNow = (double)(*data)*100/255;
			qNearest = floor(luminNow/dq) * dq;
			quantization = qNearest + (double)(dq/2)*tanh(qFy*(luminNow-qNearest));

			if(quantization > 100)
			{
				*data = 255;
			}
			else
			{
				int a = (int)quantization*255/100*1.2;
				*data = (a>255)?255:a;
			}
			data = data + 3;
		}
	}
}

void CFaceSketch::backgroundSmooth( cv::Mat &srcImg ) 
{
	bgColor = srcImg.clone();
	for (int i = 0; i < 2; i++)
	{
		swap(srcImg, bgColor);
		bilateralFilter(srcImg, bgColor, 5, 150, 150);		
	}
}

void CFaceSketch::fixTemplate( cv::Mat &templateImg)
{
     QVector<Node*> leftEyeQNodes = facemodel->getLeftEyeNodes();
	 QVector<Node*> rightEyeQNodes = facemodel->getRightEyeNodes();
	 std::vector<cv::Point> leftPoints, rightPoints;
	 int count = leftEyeQNodes.size();
	 for( int i = 0; i < count; i++ )
	 {
		 Point lpt, rpt;
		 lpt.x = leftEyeQNodes.at(i)->sceneBoundingRect().center().x();
		 lpt.y = leftEyeQNodes.at(i)->sceneBoundingRect().center().y();
		 leftPoints.push_back(lpt);
		 rpt.x = rightEyeQNodes.at(i)->sceneBoundingRect().center().x();
		 rpt.y = rightEyeQNodes.at(i)->sceneBoundingRect().center().y();
		 rightPoints.push_back(rpt);
	 }
	 cv::Mat_<cv::Vec3b> _I = templateImg;
	 #pragma omp parallel for
	 for( int row = 0; row < templateImg.rows; ++row){
		 for( int col = 0; col < templateImg.cols; ++col )
		 {
			 int value0 =  _I(row, col)[0];
			 if(value0 > 180 && !isInEyeBall(leftPoints, rightPoints, col, row)){
			 //if(value0 > 180 && !(_I(row, col)[0] == 253 && _I(row, col)[1] == 254 && _I(row, col)[2] == 252)){
				 _I(row, col)[0] = 255;
				 _I(row, col)[1] = 255;
				 _I(row, col)[2] = 255;
			 }
			 if(value0 >100 && value0 <180) {
				 _I(row, col)[0] = 30;
				 _I(row, col)[1] = 30;
				 _I(row, col)[2] = 30;
			 }
		 }
	 }
}

bool CFaceSketch::isInEyeBall(std::vector<cv::Point> leftPoints, std::vector<cv::Point> rightPoints, float x, float y)
{
	double isLEyeBall = pointPolygonTest( leftPoints, Point2f(x,y), false );
	if(isLEyeBall > 0) {
		return true;
	}
	double isREyeBall = pointPolygonTest( rightPoints, Point2f(x,y), false );
	return (isLEyeBall>0)||(isREyeBall>0);
}