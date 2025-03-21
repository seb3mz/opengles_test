#ifndef CXX_CAMERABASE_H
#define CXX_CAMERABASE_H


#include "opencv2/opencv.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>


/*
    Y’ = 0.257*R' + 0.504*G' + 0.098*B' + 16

    Cb' = -0.148*R' - 0.291*G' + 0.439*B' + 128

    Cr' = 0.439*R' - 0.368*G' - 0.071*B' + 128

    R' = 1.164*(Y’-16) + 1.596*(Cr'-128)

    G' = 1.164*(Y’-16) - 0.813*(Cr'-128) - 0.392*(Cb'-128)

    B' = 1.164*(Y’-16) + 2.017*(Cb'-128)

*/

#define YG 74 /* (int8)(1.164 * 64 + 0.5) */
#define UB 127 /* min(63,(int8)(2.018 * 64)) */
#define UG -25 /* (int8)(-0.391 * 64 - 0.5) */
#define UR 0
#define VB 0
#define VG -52 /* (int8)(-0.813 * 64 - 0.5) */
#define VR 102 /* (int8)(1.596 * 64 + 0.5) */
#define BB UB * 128 + VB * 128
#define BG UG * 128 + VG * 128
#define BR UR * 128 + VR * 128

#define LIGHT_CALC_FRAME 60
#define BRIGHT_CONTROL_STANDARD 50
#define CONTRAST_CONTROL_STANDARD 50
#define CONTRAST_SCALE 2
#define LOWEST_BRIGHTNESS 68
// #define ENABLE_AUTO_LIGHTING

typedef int                  AVM_Int;
typedef unsigned char        AVM_Byte;
typedef unsigned short       AVM_Word;
typedef unsigned int         AVM_DWord;
typedef double               AVM_Double;

AVM_Int clamp0(AVM_Int v);
AVM_Int clamp255(AVM_Int v);
AVM_DWord Clamp(AVM_Int val);
void YuvPixel(AVM_Byte y, AVM_Byte u, AVM_Byte v,AVM_Byte* rgb565High, AVM_Byte* rgb565Low);
void Rgb565Pixel(AVM_Word rgb565, AVM_Byte &y, AVM_Byte &u, AVM_Byte &v);
void InterpolationCameraData(AVM_Byte *pBuff, const AVM_DWord iWidth, const AVM_DWord iHeight, AVM_DWord iPixelType, AVM_Byte* pDest);
void ConvertRGB565ToUYVY(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
void ConvertVYUYToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
void ConvertUYVYToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
void ConvertUYVYToRGB888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
void ConvertUYVYToBGR888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
void ConvertRGB565ToRGB888(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);
//void InterpolationCameraData_ALL(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_DWord iPixelType, AVM_Byte* pDest);
void ConvertRGB888ToRGB565(AVM_Byte *pBuff, AVM_DWord iWidth, AVM_DWord iHeight, AVM_Byte* pDest);

#endif // CXX_CAMERABASE_H

