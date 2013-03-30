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
	string templatePath = "colorMode\\" + componetName + "\\" +componetName + "1";
	string savetemplatePath = "temp\\" + componetName + "\\";
	cv::Mat templateImg = cv::imread(templatePath + ".jpg", -1);
	cv::Mat bigTemplateImg(width, height, templateImg.type(), Scalar(1,2,3));
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
	cmpnNodes = getElementNodes(element);
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
	wholeFace.push_back(warpedTemplate);
	cv::imwrite(savetemplatePath + "t1.jpg", bigTemplateImg);
	cv::imwrite(savetemplatePath + "r1.jpg", warpedTemplate);
	
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
	componentSketch(LEFTBROW, "leftBrow");
	componentSketch(RIGHTBROW, "rightBrow");
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
	Mat face(width, height, CV_8UC3, Scalar(1,2,3));
	addTopToBottom(wholeFace[LEFTEYE], face);
	addTopToBottom(wholeFace[RIGHTEYE], face);
	addTopToBottom(wholeFace[LEFTBROW], face);
	addTopToBottom(wholeFace[RIGHTBROW], face);
	addTopToBottom(wholeFace[MOUTH], face);
	addTopToBottom(wholeFace[NOSE], face);
	addTopToBottom(face, wholeFace[PROFIILE]);
	Mat whole(width, height, CV_8UC3, Scalar::all(255));
	addTopToBottom(wholeFace[PROFIILE], whole);
	
	namedWindow("bbb", 1);
	imshow("bbb", whole);

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




