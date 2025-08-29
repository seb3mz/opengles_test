#ifndef CONFIG_STRUCT_H
#define CONFIG_STRUCT_H

typedef struct PaniniConfigInfo
{
    int iWidth;
	int iHeight;
	float fD;
	float fTop;
	float fBottom;
	float fAzimuth;
	float fTheta;
	float fRotDegX;
	float fRotDegY;
	float fRotDegZ;
	int iTransY;
	int iTransZ;
	int iMirrorInputImg;
	int iMirrorPoint;
	int iOffsetX;
	int iOffsetY;
} PaniniConfigInfo;

#endif //CONFIG_STRUCT_H