#include "FaceAlignDll.h"

class Align
{
public:
	void procPic();
	void detectAndDisplay(Mat& image, RECT* dectBox);
	void showAlignedFace(Mat& img, float* ptsPos, int ptsNum);
	int PointNum();
protected:
private:
	CFaceAlignDll* g_pAlign;
	static CascadeClassifier cascade;
	const char* face_cascade_name = "haarcascade_frontalface_alt2.xml";
	static bool isShowFace = false;
	bool ConvertToGrayBits(BYTE* pOrgBits, int width, int height, int nChannels, BYTE* pGrayBits, int nOrgStride, int nGrayStride);
	void DestroyAlign();
	int InitAlign(const WCHAR* wzModelFile);
	int Align(BYTE* pImgData, int iImgW, int iImgH, int iStride, RECT& rcFace, float* pPoints);
	void AvgShape( float* pPoints );
};
