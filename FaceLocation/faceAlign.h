#pragma once
#include <stdlib.h>
#include <iostream>
#include <afxwin.h>
#include <windows.h> 
#include <gdiplus.h> 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "FaceAlignDll.h"

using namespace cv;

#ifndef FACEALIGN_H
#define FACEALIGN_H

class CFaceAlign
{
public:
	CFaceAlign();
	~CFaceAlign();
	float* procPic(string strFilePath);
	void detectAndDisplay(Mat& image, RECT* dectBox);
	int PointNum();
protected:
private:
	CFaceAlignDll* g_pAlign;
	static CascadeClassifier cascade;
	const char* FACE_CASCADE_NAME;

	bool ConvertToGrayBits(BYTE* pOrgBits, int width, int height, int nChannels, BYTE* pGrayBits, int nOrgStride, int nGrayStride);
	void DestroyAlign();
	HRESULT InitAlign(const WCHAR* wzModelFile);
	int Align(BYTE* pImgData, int iImgW, int iImgH, int iStride, RECT& rcFace, float* pPoints);
	void AvgShape( float* pPoints );
	void showAlignedFace(Mat& img, float* ptsPos, int ptsNum);
};

#endif // FACEALIGN_H