#include "VideoRenderer.h"
#include "ORBMatching.h"
#include "CThinPlateSpline.h"
#include <QtGui>


CVideoRenderer::CVideoRenderer(std::string videoFilePath)
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

void CVideoRenderer::render( std::string renderedVideoPath )
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
	vector<cv::Point> currentFace, lastFace;
	int i = 0;
	int j = 1;
	
	for(;;) //Show the image captured in the window and repeat
	{
		(*srcVideoCapture) >> currentSrc;           // read

		if (currentSrc.empty()) {
			break;         // check if at end
		}
		
		if(i % j == 0) {
			currentDst = renderKeyFrame(currentSrc);
		} else {
			currentDst = propagateFromLastFrame(currentSrc, currentFace, lastSrc, lastDst, lastFace);
		}
				
		//outputVideo.write(res); //save or
		outputVideo << currentDst;
		lastDst = currentDst;
		lastFace = currentFace;
	}
}

cv::Mat CVideoRenderer::renderKeyFrame( Mat currentSrc )
{
	return currentSrc;
}

cv::Mat CVideoRenderer::propagateFromLastFrame( Mat currentSrc, vector<cv::Point> currentFace, Mat lastSrc, Mat lastDst, vector<cv::Point> lastFace )
{
	ORBMatching orb;
	vector<cv::Point> controlPoint1, controlPoint2;
	orb.findMatchigPoint(currentSrc, lastSrc, lastFace, controlPoint1, controlPoint2);
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
