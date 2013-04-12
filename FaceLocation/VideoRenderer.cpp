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

	firstFrame;
	//
	*srcVideoCapture >> firstFrame;
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
