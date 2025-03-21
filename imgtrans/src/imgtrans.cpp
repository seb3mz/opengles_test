#include <iostream>
#include "imgtrans.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc_c.h"

using namespace std;

AVM_Int clamp0(AVM_Int v)
{
    return ((-(v) >> 31) & (v));
}

AVM_Int clamp255(AVM_Int v)
{
    return (((255 - (v)) >> 31) | (v)) & 255;
}

AVM_DWord Clamp(AVM_Int val)
{
    AVM_Int v = clamp0(val);
    return (AVM_DWord)(clamp255(v));
}

void YuvPixel(AVM_Byte y, AVM_Byte u, AVM_Byte v,
						   AVM_Byte* rgb565High, AVM_Byte* rgb565Low)
{
    AVM_Byte b, g, r;
	const AVM_DWord y1 = (static_cast<AVM_Int>(y) - 16) * YG;
	b = Clamp(static_cast<AVM_Int>((u * UB + v * VB) - (BB) + y1) >> 6);
	g = Clamp(static_cast<AVM_Int>((u * UG + v * VG) - (BG) + y1) >> 6);
	r = Clamp(static_cast<AVM_Int>((u * UR + v * VR) - (BR) + y1) >> 6);

	r >>= 3;
	g >>= 2;
	b >>= 3;

	const AVM_Word wRGB565  = (r <<11) | (g<<5) | (b);

	*rgb565High = static_cast<AVM_Byte>(wRGB565 & 0xFF);
	*rgb565Low = static_cast<AVM_Byte>(wRGB565 >>8);

}

void Rgb565Pixel(AVM_Word rgb565, AVM_Byte &y, AVM_Byte &u, AVM_Byte &v)
{
    AVM_Byte b, g, r;
    b = (rgb565 & 0x001f) << 3;
    r = (rgb565 & 0xf800) >> 8;
    g = (rgb565 & 0x07e0) >> 3;

    y = Clamp(static_cast<AVM_Int>(0.257*r + 0.504*g + 0.098*b + 16));

    u = Clamp(static_cast<AVM_Int>(-0.148*r - 0.291*g + 0.439*b + 128));

    v = Clamp(static_cast<AVM_Int>(0.439*r - 0.368*g - 0.071*b + 128));

}

void InterpolationCameraData(AVM_Byte *pBuff, const AVM_DWord iWidth, const AVM_DWord iHeight, AVM_DWord iPixelType, AVM_Byte* pDest)
{
    AVM_Byte *pSrcBuff = pBuff;
    AVM_Byte *pInterpolationBuff = pDest;
    const AVM_DWord iNewWidth = iPixelType * iWidth;
    for (AVM_DWord i = 0; i < iNewWidth; ++i) {
        pInterpolationBuff[i] = pSrcBuff[i];
    }

    for (AVM_DWord i = 0; i < iNewWidth; ++i) {
        for (AVM_DWord j = 0; j < iHeight; ++j) {
            const AVM_Int iNewHeightIndex = 2 * j + 1;
            pInterpolationBuff[iNewHeightIndex * iNewWidth + i] = pSrcBuff[j * iNewWidth + i];
        }
    }

    for (AVM_DWord i = 0; i < iNewWidth; ++i) {
        for (AVM_DWord j = 1; j < iHeight * 2 - 1; j += 2) {
            const AVM_Int iNewHeightIndex = j + 1;
            pInterpolationBuff[iNewHeightIndex * iNewWidth + i] = (pInterpolationBuff[j * iNewWidth + i] + pInterpolationBuff[(j + 2) * iNewWidth + i])/2;
        }
    }
}

void ConvertRGB565ToUYVY(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pVYUYBuff = pDest;
    AVM_Word *pRGB565Buff = (AVM_Word *)pBuff;
    for (AVM_DWord i = 0; i < iHeight; i++) {
        for (AVM_DWord x = 0; x < iWidth/2; x++) {
            Rgb565Pixel(pRGB565Buff[0], pVYUYBuff[1], pVYUYBuff[0], pVYUYBuff[2]);
            Rgb565Pixel(pRGB565Buff[1], pVYUYBuff[3], pVYUYBuff[0], pVYUYBuff[2]);
            pVYUYBuff += 4;
            pRGB565Buff += 2;  // Advance 2 pixels.
        }
    }
}
void ConvertRGB565ToRGB888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pRGB888Buff = pDest;
    AVM_Word *pRGB565Buff = (AVM_Word *)pBuff;
    for (AVM_DWord i = 0; i < iHeight; i++)
        for (AVM_DWord j = 0; j < iWidth; j++)
        {
            pRGB888Buff[0] = (pRGB565Buff[0] & 0x001f) << 3;
            pRGB888Buff[1] = (pRGB565Buff[0] & 0x07e0) >> 3;
            pRGB888Buff[2] = (pRGB565Buff[0] & 0xf800) >> 8;
            // pRGB888Buff[3] = 0x00;
            pRGB888Buff += 3;
            pRGB565Buff += 1;
        }
}

void ConvertRGB888ToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pRGB888Buff = pBuff;
    AVM_Word *pRGB565Buff = (AVM_Word *)pDest;
    for (AVM_DWord i = 0; i < iHeight; i++)
    {
        for (AVM_DWord j = 0; j < iWidth; j++)
        {
            pRGB565Buff[0] = 0;
            pRGB565Buff[0] = pRGB888Buff[0] >> 3;
            const AVM_Word mtmp = pRGB888Buff[1];
            pRGB565Buff[0] |= (mtmp >> 2) << 5;
            const AVM_Word mtmp2 = pRGB888Buff[2];
            pRGB565Buff[0] |= (mtmp2 >> 3) << 11;
            pRGB888Buff += 3;
            pRGB565Buff += 1;
        }
    }
}

void ConvertVYUYToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pVYUYBuff = pBuff;
    AVM_Byte *pRGB565Buff = pDest;
    for (AVM_DWord i = 0; i < iHeight; i++) {
        for (AVM_DWord x = 0; x < iWidth/2; x++) {
            YuvPixel(pVYUYBuff[1], pVYUYBuff[0], pVYUYBuff[2], pRGB565Buff + 0, pRGB565Buff + 1);
            YuvPixel(pVYUYBuff[3], pVYUYBuff[0], pVYUYBuff[2],pRGB565Buff + 2, pRGB565Buff + 3);
            pVYUYBuff += 4;
            pRGB565Buff += 4;  // Advance 2 pixels.
        }
    }
}

void ConvertUYVYToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pUYVYBuff = pBuff;
    AVM_Byte *pRGB565Buff = pDest;
    for (AVM_DWord i = 0; i < iHeight; i++) {
        for (AVM_DWord x = 0; x < iWidth/2; x++) {
            YuvPixel(pUYVYBuff[3], pUYVYBuff[0], pUYVYBuff[2], pRGB565Buff + 0, pRGB565Buff + 1);
            YuvPixel(pUYVYBuff[1], pUYVYBuff[0], pUYVYBuff[2],pRGB565Buff + 2, pRGB565Buff + 3);
            pUYVYBuff += 4;
            pRGB565Buff += 4;  // Advance 2 pixels.
        }
    }
}

void ConvertUYVYToRGB888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pUYVYBuff = pBuff;
    AVM_Byte *pRGB888Buff = pDest;
	AVM_Byte u, v, y1, y2;

	if (pUYVYBuff == NULL || pRGB888Buff == NULL) {
		cout << "error: input data null!\n";
		return;
	}

	const AVM_Int iLength = iWidth * iHeight;

	for(AVM_DWord i = 0; i < iLength; i += 2){
		y1 = pUYVYBuff[2*i + 1];
		y2 = pUYVYBuff[2*i + 3];
		u = pUYVYBuff[2*i];
		v = pUYVYBuff[2*i + 2];

		pRGB888Buff[3*i]     = static_cast<AVM_Byte>(y1 + (u - 128) + ((104*(u - 128))>>8));
		pRGB888Buff[3*i + 1] = static_cast<AVM_Byte>(y1 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));
		pRGB888Buff[3*i + 2] = static_cast<AVM_Byte>(y1 + (v - 128) + ((199*(v - 128))>>8));

		pRGB888Buff[3*i + 3] = static_cast<AVM_Byte>(y2 + (u - 128) + ((104*(u - 128))>>8));
		pRGB888Buff[3*i + 4] = static_cast<AVM_Byte>(y2 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));
		pRGB888Buff[3*i + 5] = static_cast<AVM_Byte>(y2 + (v - 128) + ((199*(v - 128))>>8));
	}
}

void ConvertUYVYToBGR888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest)
{
    AVM_Byte *pUYVYBuff = pBuff;
    AVM_Byte *pDestBGR = pDest;
	AVM_Byte u, v, y1, y2;

	if (pUYVYBuff == NULL || pDestBGR == NULL) {
		cout << "error: input data null!\n";
		return;
	}

	const AVM_Int iLength = iWidth * iHeight;

	for(AVM_DWord i = 0; i < iLength; i += 2){
		y1 = pUYVYBuff[2*i + 1];
		y2 = pUYVYBuff[2*i + 3];
		u = pUYVYBuff[2*i];
		v = pUYVYBuff[2*i + 2];

		pDestBGR[3*i + 2]     = static_cast<AVM_Byte>(y1 + (u - 128) + ((104*(u - 128))>>8));
		pDestBGR[3*i + 1] = static_cast<AVM_Byte>(y1 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));
		pDestBGR[3*i] = static_cast<AVM_Byte>(y1 + (v - 128) + ((199*(v - 128))>>8));

		pDestBGR[3*i + 5] = static_cast<AVM_Byte>(y2 + (u - 128) + ((104*(u - 128))>>8));
		pDestBGR[3*i + 4] = static_cast<AVM_Byte>(y2 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));
		pDestBGR[3*i + 3] = static_cast<AVM_Byte>(y2 + (v - 128) + ((199*(v - 128))>>8));
	}
}