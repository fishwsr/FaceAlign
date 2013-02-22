#include "FaceAlignDll.h"
#include <stdlib.h>
#include <iostream>
#include <afxwin.h>
#include <windows.h> 
#include <gdiplus.h> 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;

class CFaceAlign
{
public:
	CFaceAlign();
	~CFaceAlign();
	void procPic(CString strFilePath);
	void detectAndDisplay(Mat& image, RECT* dectBox);
	void showAlignedFace(Mat& img, float* ptsPos, int ptsNum);
	int PointNum();
protected:
private:
	CFaceAlignDll* g_pAlign;
	static CascadeClassifier cascade;
	const char* face_cascade_name;
	static bool isShowFace;
	bool ConvertToGrayBits(BYTE* pOrgBits, int width, int height, int nChannels, BYTE* pGrayBits, int nOrgStride, int nGrayStride);
	void DestroyAlign();
	int InitAlign(const WCHAR* wzModelFile);
	int Align(BYTE* pImgData, int iImgW, int iImgH, int iStride, RECT& rcFace, float* pPoints);
	void AvgShape( float* pPoints );
};

bool CFaceAlign::isShowFace = false;
