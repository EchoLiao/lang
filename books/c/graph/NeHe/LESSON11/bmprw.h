/*
 * =====================================================================================
 *
 *       Filename:
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


#ifndef _BMPRW_H
#define _BMPRW_H


#define NALDEBUG

#ifdef NALDEBUG
#    define NALReadBMPFile ReadBMPFile
#    define NALWriteBMPFile WriteBMPFile
#else
#    define NALReadBMPFile
#    define NALWriteBMPFile
#endif


typedef unsigned int 		NALINT;
typedef unsigned long 		NALLONG;
typedef unsigned int        NALDWORD;
typedef unsigned short      NALWORD;
typedef unsigned char       NALBYTE;
typedef int					NALBOOL;

#ifndef NALFALSE
#define NALFALSE               0
#endif

#ifndef NALTRUE
#define NALTRUE                1
#endif


#define     BMP_HEADER_OFFSET           0x0
#define     BMP_INFO_HEADER_OFFSET      0xE
#define     BMP_DATA_OFFSET             0x36    // 真彩图的



#pragma pack(1)  /* arm-linux-gcc 不支持该选项? QQQQQ */
typedef struct
{
    NALWORD  bfType;
    NALDWORD bfSize; /* file size */
    NALWORD  bfReserved1;
    NALWORD  bfReserved2;
    NALDWORD bfOffBits; /* data offset */
} sbmpHeader;

typedef struct
{
    NALDWORD biSize; /* size of tagBITMAPINFOHEADER */
    NALDWORD biWidth;
    NALDWORD biHeight;
    NALWORD  biPlanes; /* set to 1 */
    NALWORD  biBitCount; /* bits per pixel */
    NALDWORD biCompression; /* compression type: 0(BI_RGB), ... */
    NALDWORD biSizeImage; /* if is BI_RGB set to 0 */
    NALDWORD biXPelsPerMeter;
    NALDWORD biYPelsPerMeter;
    NALDWORD biClrUsed; /* 0: use all Color Planes */
    NALDWORD biClrImportant;
} sbmpInfoHeader;
#pragma pack()


enum eDataFormat
{
    EBMP_UNKNOWN = 0,
    EBMP_RGB,
    EBMP_BGR,
    EBMP_RGBA,
    EBMP_BGRA,
    EBMP_FORMAT_COUNT,
};

typedef struct
{
    eDataFormat edformat;
    NALINT      w;          // in pixels
    NALINT      h;          // in pixels
    NALINT      iBitCount;  // in bits
    NALBOOL     isRevert;
    NALBYTE     *pdata;
} sbitData;

NALBOOL ReadBMPFile(const char* fname, sbitData *bdata);
NALBOOL WriteBMPFile(const char* fname, sbitData *bdata);

#endif // _BMPRW_H
