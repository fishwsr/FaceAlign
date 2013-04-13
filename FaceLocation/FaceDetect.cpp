#include "FaceDetect.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <qt/qdebug.h>

CFaceDetect::CFaceDetect(void)
{
	FACE_CASCADE_NAME = "haarcascade_frontalface_alt2.xml";
	if( !cascade.load( FACE_CASCADE_NAME ) )
	{ 
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" ); 
		exit(0); 
	}
	qDebug("CFaceDetect Created.\n");
}


CFaceDetect::~CFaceDetect(void)
{
	
}

void CFaceDetect::detectAndDisplay(Mat& img, RECT* detectBox) 
{
	double scale=1.2;
	Mat small_img(cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1);
	Mat gray(cvRound (img.rows/scale),cvRound(img.cols/scale),8,1);
	cvtColor(img, gray, CV_BGR2GRAY);
	resize(gray, small_img, small_img.size(), 0, 0, INTER_LINEAR);
	equalizeHist(small_img,small_img); //Ö±·½Í¼¾ùºâ

	//Detect the biggest face
	double t = (double)cvGetTickCount(); 
	std::vector<cv::Rect> faces; 
	cascade.detectMultiScale(small_img, faces, 1.1, 2, CV_HAAR_FIND_BIGGEST_OBJECT, cvSize(30, 30)); 
	t = (double)cvGetTickCount() - t; 
	printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

	//found objects and draw boxes around them 
	if(faces.size() == 1)
	{
		(*detectBox).left = faces[0].x * scale;
		(*detectBox).right = (faces[0].x + faces[0].width) * scale;		
		(*detectBox).top = faces[0].y * scale + 10;
		(*detectBox).bottom = (faces[0].y + faces[0].height) * scale + 10;
		rectangle(img, cvPoint((*detectBox).left,(*detectBox).top), cvPoint((*detectBox).right,(*detectBox).bottom), Scalar( 255, 0, 0 ));
	}
	else
	{
		printf("Fail to detect faces.\n");
	}
	return;
}