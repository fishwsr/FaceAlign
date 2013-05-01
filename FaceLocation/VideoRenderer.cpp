#include "VideoRenderer.h"
#include "ORBMatching.h"
#include "CThinPlateSpline.h"
#include <QtGui>


CVideoRenderer::CVideoRenderer(std::string videoFilePath, CFaceSketch* faceSketch)
{
	srcVideoCapture = new VideoCapture(videoFilePath);
	if (!srcVideoCapture->isOpened())
	{
		qDebug("Could not open reference %s\n", videoFilePath);
		return;
	}

	Mat frame;
	//
	*srcVideoCapture >> frame;
	firstFrame = frame.clone();
	srcVideoCapture->release();
	srcVideoCapture->open(videoFilePath);
	//imshow("test", firstFrame);
	frameWidth = firstFrame.cols;
	frameHeight = firstFrame.rows;
	interval = 3;
	this->faceSketch = faceSketch;
}


CVideoRenderer::~CVideoRenderer(void)
{
	if(srcVideoCapture != NULL) {
		delete srcVideoCapture;
	}
}

cv::Mat CVideoRenderer::getFirstFrame()
{
	return firstFrame;
}

void CVideoRenderer::render( std::string renderedVideoPath, int bgThresholdValue, int fcThresholdValue )
{
	VideoWriter outputVideo;
	int ex = static_cast<int>(srcVideoCapture->get(CV_CAP_PROP_FOURCC));
	Size S = Size((int) srcVideoCapture->get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int) srcVideoCapture->get(CV_CAP_PROP_FRAME_HEIGHT));
	outputVideo.open(renderedVideoPath, ex, srcVideoCapture->get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		qDebug("Could not open the output video for write:%s \n", renderedVideoPath);
		return;
	}

	Mat currentSrc, currentDst, lastSrc, lastDst;
	int i = 0;

	//namedWindow("test", CV_WINDOW_AUTOSIZE);
	//cvMoveWindow("test", S.width, 0);
	for(;;) //Show the image captured in the window and repeat
	{
		double t = (double)getTickCount();
		(*srcVideoCapture) >> currentSrc;           // read

		if (currentSrc.empty()) {
			break;         // check if at end
		}
		
		if(isKeyFrame(i)) {
			currentDst = renderKeyFrame(currentSrc);
		} else {
			currentDst = propagateFromLastFrame(currentSrc, lastSrc, lastDst);
		}

		
		//imshow("test", currentDst);
				
		outputVideo << currentDst;
		lastDst = currentDst;
		lastSrc = currentSrc;
		i++;

		t = ((double)getTickCount() - t)/getTickFrequency();
		qDebug("Frame %d rendered -- Times passed in seconds: %f\n", i, t);
		//cvWaitKey();

	}
}

cv::Mat CVideoRenderer::renderKeyFrame( Mat currentSrc, int bgThresholdValue, int fcThresholdValue )
{
	cv::imwrite("temp/currentSrcVideoFrame.jpg", currentSrc);
	CFaceAlign faceAlign;
	float* ptsPos;
	ptsPos = faceAlign.procPic("temp/currentSrcVideoFrame.jpg");
	int pointnum = faceAlign.PointNum();

	QGraphicsPixmapItem imgItem;
	QFaceModel facemodel(ptsPos,pointnum,&imgItem);
	cv::Mat sketchedFace = faceSketch->sketchFace(&facemodel,currentSrc);
	currentFace = faceSketch->getPointsToWrap();
	return sketchedFace;
}

cv::Mat CVideoRenderer::propagateFromLastFrame( Mat currentSrc, Mat lastSrc, Mat lastDst )
{
	vector<cv::Point> controlPoint1, controlPoint2;
	orb.findMatchigPoint(currentSrc, lastSrc, currentFace,  controlPoint1, controlPoint2);
	currentFace = controlPoint2;

	controlPoint1.push_back(cvPoint(0,0));
	controlPoint2.push_back(cvPoint(0,0));
	controlPoint1.push_back(cvPoint(frameWidth-1, 0));
	controlPoint2.push_back(cvPoint(frameWidth-1, 0));
	controlPoint1.push_back(cvPoint(0, frameHeight-1));
	controlPoint2.push_back(cvPoint(0, frameHeight-1));
	controlPoint1.push_back(cvPoint(frameWidth-1, frameHeight-1));
	controlPoint2.push_back(cvPoint(frameWidth-1, frameHeight-1));
	
	Mat currentDst;
	CThinPlateSpline tps(controlPoint1, controlPoint2);
	tps.warpImage(lastDst,currentDst,0.01,INTER_CUBIC,BACK_WARP);
	return currentDst;
}

bool CVideoRenderer::isKeyFrame( int index )
{
	if (index % interval == 0)
	{
		return true;
	} 
	else
	{
		return false;
	}

}
