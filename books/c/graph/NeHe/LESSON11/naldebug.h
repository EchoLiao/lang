/*
 * =====================================================================================
 *
 *       Filename:  naldebug.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011年05月06日 09时43分31秒
 *        Created:
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  nuoerlz (nuoliu), nuoerlz@gmail.com
 *        Company:  mhtt
 *
 *      CopyRight:  Reserve
 *
 * =====================================================================================
 */


#ifndef _NALDEBUG_H
#define _NALDEBUG_H


#define NALDEBUG

#ifdef NALDEBUG
#    define NALReadBMPFile ReadBMPFile
#    define NALWriteBMPFile WriteBMPFile
#else
#    define NALReadBMPFile
#    define NALWriteBMPFile
#endif


typedef long				NALLONG;
typedef unsigned long       NALDWORD;
typedef unsigned short      NALWORD;
typedef unsigned char       NALBYTE;
typedef int					NALBOOL;

#ifndef NALFALSE
#define NALFALSE               0
#endif

#ifndef NALTRUE
#define NALTRUE                1
#endif




/* #pragma pack(1)  [>arm-linux-gcc 不支持该选项? QQQQQ<] */
typedef struct
{
    NALWORD  bfType;
    NALDWORD bfSize; /* file size */
    NALWORD  bfReserved1;
    NALWORD  bfReserved2;
    NALDWORD bfOffBits; /* data offset */
} NALBITMAPFILEHEADER,*NALLPBITMAPFILEHEADER;
/* #pragma pack() */

typedef struct
{
    NALDWORD biSize; /* size of tagBITMAPINFOHEADER */
    NALLONG  biWidth;
    NALLONG  biHeight;
    NALWORD  biPlanes; /* set to 1 */
    NALWORD  biBitCount; /* bits per pixel */
    NALDWORD biCompression; /* compression type: 0(BI_RGB), ... */
    NALDWORD biSizeImage; /* if is BI_RGB set to 0 */
    NALLONG  biXPelsPerMeter;
    NALLONG  biYPelsPerMeter;
    NALDWORD biClrUsed; /* 0: use all Color Planes */
    NALDWORD biClrImportant;
} NALBITMAPINFOHEADER;


NALBOOL ReadBMPFile(char* lpszName, NALBYTE **pRGBBuf, long *m_nWidth,
        long *m_nHeight, int *iBitCount, NALBOOL isRevert);
NALBOOL WriteBMPFile(char* BMPFilename, NALBYTE *pRGBBuf, long m_nWidth,
        long m_nHeight, int iBitCount, NALBOOL isRevert);

#endif // _NALDEBUG_H
