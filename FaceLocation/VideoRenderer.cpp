#include "VideoRenderer.h"
#include "ORBMatching.h"
#include "CThinPlateSpline.h"
#include <QtGui>


CVideoRenderer::CVideoRenderer(QString* videoFilePath, CFaceSketch* faceSketch)
{
	std::string filePathAsString = (const char *)videoFilePath->toLocal8Bit();
	srcVideoCapture = new VideoCapture(filePathAsString);
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
	srcVideoCapture->open(filePathAsString);
	frameCount = srcVideoCapture->get(CV_CAP_PROP_FRAME_COUNT);
	//imshow("test", firstFrame);
	frameWidth = firstFrame.cols;
	frameHeight = firstFrame.rows;
	interval = 1;
	this->faceSketch = faceSketch;
	renderedVideoPath = "temp/"+ QFileInfo(*videoFilePath).baseName() + ".avi";
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

void CVideoRenderer::render(int bgThresholdValue, int qtzThresholdValue )
{
	VideoWriter outputVideo;
	CFaceAlign* faceAlign = new CFaceAlign();
	int ex = static_cast<int>(srcVideoCapture->get(CV_CAP_PROP_FOURCC));
	Size S = Size((int) srcVideoCapture->get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int) srcVideoCapture->get(CV_CAP_PROP_FRAME_HEIGHT));
	outputVideo.open((const char *)renderedVideoPath.toLocal8Bit(), ex, srcVideoCapture->get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		qDebug("Could not open the output video for write:%s \n", renderedVideoPath);
		return;
	}

	Mat currentSrc, currentDst, lastSrc, lastDst;
	int i = 0;
	emit progressChanged(0.0f);
	//namedWindow("test", CV_WINDOW_AUTOSIZE);
	//cvMoveWindow("test", S.width, 0);
	isStop = false;
	for(;;) //Show the image captured in the window and repeat
	{
		double t = (double)getTickCount();
		(*srcVideoCapture) >> currentSrc;           // read

		if (isStop || currentSrc.empty()) {
			break;         // check if at end
		}
		
		if(isKeyFrame(i)) {
			currentDst = renderKeyFrame(faceAlign, currentSrc, bgThresholdValue, qtzThresholdValue);
		} else {
			currentDst = propagateFromLastFrame(currentSrc, lastSrc, lastDst);
		}

		
		//imshow("test", currentDst);
				
		outputVideo << currentDst;
		lastDst = currentDst;
		lastSrc = currentSrc;
		i++;
		emit progressChanged((float)i/frameCount);
		t = ((double)getTickCount() - t)/getTickFrequency();
		qDebug("Frame %d rendered -- Times passed in seconds: %f\n", i, t);
		//cvWaitKey();

	}
	delete faceAlign;
}

void CVideoRenderer::stopRender(){
	isStop = true;
}

QString CVideoRenderer::getRenderedVideoPath(){
	return this->renderedVideoPath;
}

cv::Mat CVideoRenderer::renderKeyFrame( CFaceAlign* faceAlign, Mat currentSrc, int bgThresholdValue, int qtzThresholdValue )
{
	cv::imwrite("temp/currentSrcVideoFrame.jpg", currentSrc);
	float* ptsPos;
	int pointnum = faceAlign->PointNum();
	ptsPos = faceAlign->procPic("temp/currentSrcVideoFrame.jpg");

	QGraphicsPixmapItem imgItem;
	QFaceModel facemodel(ptsPos,pointnum,&imgItem);
	cv::Mat sketchedFace = faceSketch->sketchFace(&facemodel,currentSrc, bgThresholdValue, bgThresholdValue);
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
