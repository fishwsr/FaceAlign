#include "VideoRenderer.h"
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

	Mat currentSrc, currentDst, lastDst;
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
			currentDst = propagateFromLastFrame(currentSrc, lastDst);
		}
				
		//outputVideo.write(res); //save or
		outputVideo << currentDst;
		lastDst = currentDst;
	}
}

cv::Mat CVideoRenderer::renderKeyFrame( Mat currentSrc )
{
	return currentSrc;
}

cv::Mat CVideoRenderer::propagateFromLastFrame( Mat currentSrc, Mat lastDst )
{
	return currentSrc;
}
