/*
 * =====================================================================================
 *
 *       Filename:  naldebug.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011年05月06日 09时40分50秒
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naldebug.h"

// =================================================================================
NALBOOL ReadBMPFile(char* lpszName, NALBYTE **pRGBBuf, long *m_nWidth,
        long *m_nHeight, int *iBitCount, NALBOOL isRevert)
{
    FILE *pFile=NULL;
    NALBITMAPFILEHEADER bfh1;
    NALBITMAPINFOHEADER bih1;
    NALBOOL isOK = NALFALSE;

    printf("NAL %d &&&&&&&&& %s &&& %s\n", __LINE__, __func__, __FILE__);
    pFile = fopen(lpszName, "rb");
    if ( NULL == pFile )
    {
        printf("Cannot open BMP file '%s'\n", lpszName);
        return NALFALSE;
    }
    printf("NAL %d &&&&&&&&& %s &&& %s\n", __LINE__, __func__, __FILE__);
    fread(&bfh1, 1, 2, pFile); /* read into bfh1.bfType */
    printf("NAL %d &&&&&&&&& %s &&& %s\n", __LINE__, __func__, __FILE__);
    switch (bfh1.bfType)
    {
        case 0x4D42: // bmp
            printf("NAL test 3 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
            fseek(pFile,0,SEEK_SET);

            unsigned char *DestBuf, *SrcLineBuf;
            int SrcPitch, ImgHeight;
            /* NALBITMAPFILEHEADER bfh1; */
            int i1;
            //读文件头
            fread(&bfh1, 1, 2, pFile);
            fread(&bfh1.bfSize, 1, 4, pFile);
            fread(&bfh1.bfReserved1, 1, 2, pFile);
            fread(&bfh1.bfReserved2, 1, 2, pFile);
            fread(&bfh1.bfOffBits, 1, 4, pFile);

            //读信息头
            fread(&bih1, 1, 12, pFile);
            fread(&bih1.biPlanes, 1, 2, pFile);
            fread(&bih1.biBitCount, 1, 2, pFile);
            fread(&bih1.biCompression, 1, 24, pFile);

            ImgHeight = abs(bih1.biHeight);

            *m_nWidth = bih1.biWidth;
            *m_nHeight = bih1.biHeight;
            *iBitCount = bih1.biBitCount;

            if ( (SrcLineBuf = (unsigned char *)malloc(bih1.biWidth * (bih1.biBitCount/8))) == NULL )
                printf("malloc ERROR!\n");
            if ( (DestBuf = (unsigned char *)malloc(bih1.biWidth * ImgHeight * (bih1.biBitCount)/8)) == NULL )
                printf("malloc ERROR!\n");
            *pRGBBuf = DestBuf;
            //定位
            fseek(pFile,bfh1.bfOffBits, SEEK_SET);
            //
            SrcPitch = bih1.biBitCount * bih1.biWidth / 8;
            if ( isRevert ) // 倒置图象
                DestBuf = DestBuf + (bih1.biWidth * ImgHeight * (bih1.biBitCount/8)) - SrcPitch;
            //读取指定行数据
            fread(SrcLineBuf, 1, SrcPitch, pFile);
            if (bih1.biHeight > 0)
            {
                printf("NAL test 4 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
                //计算目的缓冲区指针
                for (i1 = 0;i1 < ImgHeight; i1++)
                {
                    switch (bih1.biBitCount)
                    {
                        case 24:
                        case 32:
                            // printf("NAL test 5 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
                            memcpy(DestBuf, SrcLineBuf, SrcPitch);
                            if ( isRevert )
                                DestBuf -= SrcPitch;
                            else
                                DestBuf += SrcPitch;
                            break;
                    }
                    fread(SrcLineBuf, 1, SrcPitch, pFile);
                }
                isOK = NALTRUE;
            }
            else
            {
                printf("暂不支持正置图象\n");
                isOK = NALFALSE;
            }
            free(SrcLineBuf);
            break;
    }

    fclose(pFile);

    return isOK;
}

NALBOOL WriteBMPFile(char* BMPFilename, NALBYTE *pRGBBuf, long m_nWidth,
        long m_nHeight, int iBitCount, NALBOOL isRevert)
{
    long RGB_SIZE = (m_nWidth * m_nHeight * iBitCount) >> 3;
    // if(iBitCount == 32)
    {
        FILE *fp;
        NALDWORD count=0;
        NALBITMAPFILEHEADER bmpHeader;
        NALBITMAPINFOHEADER bmpInfo;

        if( (fp = fopen(BMPFilename, "wb")) == NULL )
        {
            printf( "Can not create BMP file: %s\n", BMPFilename );
            return NALFALSE;
        }
        printf("NAL test (%s) w1 NALBITMAPFILEHEADER: %d, NALBITMAPINFOHEADER: %d, &&&&&&&&&&&\n",
                BMPFilename, sizeof(NALBITMAPFILEHEADER), sizeof(NALBITMAPINFOHEADER));

        bmpHeader.bfType = (NALWORD)(('M' << 8) | 'B');
        // bmpHeader.bfSize = (NALDWORD)(RGB_SIZE + sizeof(NALBITMAPFILEHEADER) + sizeof(NALBITMAPINFOHEADER));
        bmpHeader.bfSize = (NALDWORD)(RGB_SIZE + 0x36);
        bmpHeader.bfReserved1 = 0;
        bmpHeader.bfReserved2 = 0;
        bmpHeader.bfOffBits = 0x36; // sizeof(NALBITMAPFILEHEADER) + sizeof(NALBITMAPINFOHEADER);

        bmpInfo.biSize = sizeof(NALBITMAPINFOHEADER);
        bmpInfo.biWidth = m_nWidth;
        bmpInfo.biHeight = m_nHeight;
        bmpInfo.biPlanes = 1;
        bmpInfo.biBitCount = iBitCount;
        bmpInfo.biCompression = 0 /* BI_RGB */;
        bmpInfo.biSizeImage = (((m_nWidth*iBitCount/8+3)/4)*4) * m_nHeight;
        bmpInfo.biXPelsPerMeter = 0;
        bmpInfo.biYPelsPerMeter = 0;
        bmpInfo.biClrUsed = 0;
        bmpInfo.biClrImportant = 0;

        if ( (count=fwrite(&bmpHeader.bfType, 1, 2, fp)) != 2 )
            printf( "write BMP file header failed: count=%ld\n", count);
        if ( (count=fwrite(&bmpHeader.bfSize, 1, 4, fp)) != 4 )
            printf( "write BMP file header failed: count=%ld\n", count);
        if ( (count=fwrite(&bmpHeader.bfReserved1, 1, 2, fp)) != 2 )
            printf( "write BMP file header failed: count=%ld\n", count);
        if ( (count=fwrite(&bmpHeader.bfReserved2, 1, 2, fp)) != 2 )
            printf( "write BMP file header failed: count=%ld\n", count);
        if ( (count=fwrite(&bmpHeader.bfOffBits, 1, 4, fp)) != 4 )
            printf( "write BMP file header failed: count=%ld\n", count);
        printf("NAL header info\n");
        /* if ( (count=fwrite(&bmpHeader, 1, sizeof(NALBITMAPFILEHEADER), fp)) != sizeof(NALBITMAPFILEHEADER)) */
        /*    printf( "write BMP file header failed: count=%ld\n", count);                                */

        if ( (count=fwrite(&bmpInfo, 1, sizeof(NALBITMAPINFOHEADER), fp)) != sizeof(NALBITMAPINFOHEADER))
            printf( "Read BMP file info failed: count=%ld\n", count);

        if ( ! isRevert )
        {
            if ( (count=fwrite(pRGBBuf, 1, RGB_SIZE, fp)) != RGB_SIZE)
                printf( "write BMP file data failed: count=%ld\n", count);
        }
        else
        {
            // int lineBs = (m_nWidth * iBitCount) >> 3;
            int lineBs = (((m_nWidth*iBitCount/8+3)/4)*4);
            int i = RGB_SIZE - m_nWidth*iBitCount/8;
            for ( ; i >= 0; i -= lineBs )
            {
                if ( (count=fwrite(pRGBBuf + i, 1, m_nWidth*iBitCount/8, fp)) != m_nWidth*iBitCount/8 )
                    printf( "write BMP file data failed: count=%ld\n", count );
            }
        }

        fclose(fp);
        return NALTRUE;
    }
    // else
    //     return NALFALSE;
}


