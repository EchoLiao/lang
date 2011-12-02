/*
 * ===========================================================================
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
 * ===========================================================================
 */

// #define NDEBUG
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naldebug.h"


void st_read_byte(NALBYTE *byte, FILE *file)
{
    if ( fread(byte, 1, 1, file) != 1 )
        printf("NAL fread Error!");
}

void st_read_word(NALWORD *word, FILE *file)
{
    if ( fread(word, 1, 2, file) != 2 )
        printf("NAL fread Error!");
}

void st_read_dword(NALDWORD *dword, FILE *file)
{
    if ( fread(dword, 1, 4, file) != 4 )
        printf("NAL fread Error!");
}

void st_write_byte(NALBYTE *byte, FILE *file)
{
    if ( fwrite(byte, 1, 1, file) != 1 )
        printf("NAL fwrite Error!");
}

void st_write_word(NALWORD *word, FILE *file)
{
    if ( fwrite(word, 1, 2, file) != 2 )
        printf("NAL fwrite Error!");
}

void st_write_dword(NALDWORD *dword, FILE *file)
{
    if ( fwrite(dword, 1, 4, file) != 4 )
        printf("NAL fwrite Error!");
}

void st_read_bmp_header(sbmpHeader *head, FILE *file)
{
    assert(head != NULL && file != NULL);

    //读文件头
    fseek(file, BMP_HEADER_OFFSET, SEEK_SET);
    st_read_word (&head->bfType,        file);
    st_read_dword(&head->bfSize,        file);
    st_read_word (&head->bfReserved1,   file);
    st_read_word (&head->bfReserved2,   file);
    st_read_dword(&head->bfOffBits,     file);
}

void st_read_bmp_info_header(sbmpInfoHeader *info, FILE *file)
{
    assert(info != NULL && file != NULL);

    //读信息头
    fseek(file, BMP_INFO_HEADER_OFFSET, SEEK_SET);
    st_read_dword(&info->biSize,           file);
    st_read_dword(&info->biWidth,          file);
    st_read_dword(&info->biHeight,         file);
    st_read_word (&info->biPlanes,         file);
    st_read_word (&info->biBitCount,       file);
    st_read_dword(&info->biCompression,    file);
    st_read_dword(&info->biSizeImage,      file);
    st_read_dword(&info->biXPelsPerMeter,  file);
    st_read_dword(&info->biYPelsPerMeter,  file);
    st_read_dword(&info->biClrUsed,        file);
    st_read_dword(&info->biClrImportant,   file);
}

void st_write_bmp_header(sbmpHeader *head, FILE *file)
{
    assert(head != NULL && file != NULL);

    fseek(file, BMP_HEADER_OFFSET, SEEK_SET);
    st_write_word (&head->bfType,        file);
    st_write_dword(&head->bfSize,        file);
    st_write_word (&head->bfReserved1,   file);
    st_write_word (&head->bfReserved2,   file);
    st_write_dword(&head->bfOffBits,     file);
}

void st_write_bmp_info_header(sbmpInfoHeader *info, FILE *file)
{
    assert(info != NULL && file != NULL);

    fseek(file, BMP_INFO_HEADER_OFFSET, SEEK_SET);
    st_write_dword(&info->biSize,           file);
    st_write_dword(&info->biWidth,          file);
    st_write_dword(&info->biHeight,         file);
    st_write_word (&info->biPlanes,         file);
    st_write_word (&info->biBitCount,       file);
    st_write_dword(&info->biCompression,    file);
    st_write_dword(&info->biSizeImage,      file);
    st_write_dword(&info->biXPelsPerMeter,  file);
    st_write_dword(&info->biYPelsPerMeter,  file);
    st_write_dword(&info->biClrUsed,        file);
    st_write_dword(&info->biClrImportant,   file);
}

void st_set_bitmap_format(sbmpInfoHeader *info, sbitData *bdata)
{
    if ( info->biCompression == 0 ) // Format is BI_RGB ?
    {
        if ( info->biBitCount == 24 )
            bdata->edformat = EBMP_BGR;
        else if ( info->biBitCount == 32 )
            bdata->edformat = EBMP_BGRA;
        else
            bdata->edformat = EBMP_UNKNOWN;
    }
    else
    {
        bdata->edformat = EBMP_UNKNOWN;
    }
}

NALBOOL st_format_transform(unsigned char *buf, int blen, 
        int bpp, enum eDataFormat dft)
{
    int i, pixelsize, w;
    unsigned char t, *p;

    if ( bpp != 24 && bpp != 32 )
        return NALTRUE;

    if ( dft == EBMP_BGR || dft == EBMP_BGRA )
        return NALTRUE;

    pixelsize = (bpp >> 3);
    w = blen / pixelsize;

    // bmp文件颜色分量的存储顺序是: BGR 或 BGRA
    // RGB  ==> BGR 
    // RGBA ==> BGRA
    if ( dft == EBMP_RGB || dft == EBMP_RGBA )
    {
        p = buf;
        for ( i = 0; i < w; i++ )
        {
            // assert(*p == 255);
            t = *p;
            *p = *(p+2);
            *(p+2) = t;
            p += pixelsize;
        }
        return NALTRUE;
    }

    printf("Unknown Format!\n");

    return NALFALSE;
}

/* 
 * You NEED release bdata->pdata by youself!
 * */
NALBOOL st_read_bmp_file(FILE *file, sbitData *bdata)
{
    assert(file != NULL && bdata != NULL);

    printf("NAL test 3 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

    unsigned int i, linepich;
    unsigned char *dbuf;
    sbmpHeader      header;
    sbmpInfoHeader  info;

    st_read_bmp_header(&header, file);
    st_read_bmp_info_header(&info, file);
    assert(header.bfType == 0x4D42);
    if ( info.biBitCount != 16 && info.biBitCount != 24 
            && info.biBitCount != 32 )
    {
        printf("Only support 16 24 32 bits Format!\n");
        return NALFALSE;
    }

    // bmp文件格式要求行以 4 字节对齐
    linepich = ((((info.biWidth*info.biBitCount)>>3)+3)>>2)<<2;
    assert( linepich - ((info.biWidth*info.biBitCount)>>3) <= 3 );

    st_set_bitmap_format(&info, bdata);
    bdata->w = linepich / (info.biBitCount>>3); // MUST: info.biHeight >= 8
    bdata->h = info.biHeight;   // 高度值可能是负数? QQQQQ
    bdata->iBitCount = info.biBitCount;
    if ( (dbuf=(unsigned char*)calloc(1, linepich * bdata->h)) == NULL )
        goto _NO_MEMORY;
    bdata->pdata = dbuf;

    //定位到数据段
    fseek(file, header.bfOffBits, SEEK_SET);

    if ( bdata->isRevert )
    {
        dbuf = dbuf + (linepich * bdata->h) - linepich;
        for ( i = 0; i < bdata->h; i++ )
        {
            if ( fread(dbuf, 1, linepich, file) != linepich )
                goto _READ_ERROR;
            dbuf -= linepich;
        }
    }
    else
    {
        for ( i = 0; i < bdata->h; i++ )
        {
            if ( fread(dbuf, 1, linepich, file) != linepich )
                goto _READ_ERROR;
            dbuf += linepich;
        }
    }
    return NALTRUE;

_READ_ERROR:
    printf("fread Error!!");
_NO_MEMORY:
    return NALFALSE;
}

/* 
 * */
NALBOOL st_write_bmp_file(FILE *file, sbitData *bdata)
{
    assert(file != NULL && bdata != NULL);

    unsigned int i, srcw, linepich, igap;
    unsigned char   gaps[3] = { 0 };
    unsigned char   *sbuf, *ssbuf;
    sbmpHeader      header;
    sbmpInfoHeader  info;

    srcw = bdata->w * (bdata->iBitCount >> 3);
    linepich = ((srcw+3)>>2)<<2;
    igap = linepich - srcw;
    assert(igap <= 3);

    header.bfType = 0x4D42;
    // 目前只支持真彩图
    header.bfSize = BMP_DATA_OFFSET + linepich * bdata->h;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits = BMP_DATA_OFFSET;
    
    info.biSize = BMP_DATA_OFFSET - BMP_INFO_HEADER_OFFSET;
    info.biWidth = linepich / (bdata->iBitCount>>3);
    info.biHeight = bdata->h;
    info.biPlanes = 1;
    info.biBitCount = bdata->iBitCount;
    info.biCompression = 0; // BI_RGB
    info.biSizeImage = 0;
    info.biXPelsPerMeter = 0;
    info.biYPelsPerMeter = 0;
    info.biClrUsed = 0;
    info.biClrImportant = 0;

    st_write_bmp_header(&header, file);
    st_write_bmp_info_header(&info, file);

    if ( (ssbuf=(unsigned char*)calloc(1, linepich)) == NULL )
        goto _NO_MEMORY;
    fseek(file, BMP_DATA_OFFSET, SEEK_SET);
    if ( bdata->isRevert )
    {
        sbuf = bdata->pdata + linepich * (bdata->h - 1);
        for ( i = 0; i < bdata->h; i++ )
        {
            memcpy(ssbuf, sbuf, linepich);
            st_format_transform(ssbuf, linepich, bdata->iBitCount,
                    bdata->edformat);
            if ( fwrite(ssbuf, 1, srcw, file) != srcw )
                goto _WRITE_ERROR;
            if ( igap != 0 )
                if ( fwrite(gaps, 1, igap, file) != igap )
                    goto _WRITE_ERROR;
            sbuf -= linepich;
        }
    }
    else 
    {
        sbuf = bdata->pdata;
        for ( i = 0; i < bdata->h; i++ )
        {
            memcpy(ssbuf, sbuf, linepich);
            st_format_transform(ssbuf, linepich, bdata->iBitCount,
                    bdata->edformat);
            if ( fwrite(ssbuf, 1, srcw, file) != srcw )
                goto _WRITE_ERROR;
            if ( igap != 0 )
                if ( fwrite(gaps, 1, igap, file) != igap )
                    goto _WRITE_ERROR;
            sbuf += linepich;
        }
    }

    free(ssbuf);
    return NALTRUE;

_WRITE_ERROR:
    free(ssbuf);
    printf("fwrite Error!!\n");
_NO_MEMORY:
    return NALFALSE;
}

NALBOOL ReadBMPFile(const char* fname, sbitData *bdata)
{
    NALBOOL         ret = NALFALSE;
    FILE            *pFile = NULL;
    sbmpHeader      header;

    printf("NAL %d &&&&&&&&& %s &&& %s\n", __LINE__, __func__, __FILE__);

    pFile = fopen(fname, "rb");
    if ( NULL == pFile )
    {
        printf("Cannot open BMP file '%s'\n", fname);
        return NALFALSE;
    }

    st_read_bmp_header(&header, pFile);

    switch ( header.bfType )
    {
        case 0x4D42: // bmp
            ret = st_read_bmp_file(pFile, bdata);
            break;
        default:
            assert(!"Unknown file Format!");
            break;
    }

    fclose(pFile);

    return ret;
}

NALBOOL WriteBMPFile(const char* fname, sbitData *bdata)
{
    NALBOOL         ret;
    FILE            *pFile=NULL;

    printf("NAL %d &&&&&&&&& %s &&& %s\n", __LINE__, __func__, __FILE__);

    pFile = fopen(fname, "wb");
    if ( NULL == pFile )
    {
        printf("Cannot open BMP file '%s'\n", fname);
        return NALFALSE;
    }

    ret = st_write_bmp_file(pFile, bdata);

    fclose(pFile);

    return ret;
}

void bmp_test(sbitData *bdata)
{
    unsigned int x, y, bytespp, linepich;
    NALBYTE *pb;

    bytespp = bdata->iBitCount >> 3;
    linepich = (((bdata->w * bytespp) + 3)>>2)<<2;
#if 0
    for ( y = 0; y < bdata->h; y++ )
    {
        pb = bdata->pdata + y * linepich;
        for ( x = 0; x < bdata->w; x++ )
        {
            // if ( *pb != 0 && *(pb+1) != 0 && *(pb+2) != 0 )
            if ( *pb != 255 )
                printf("x");
            else
                printf("0");
            pb += 3;
        }
        printf("\n");
    }
#else
    for ( y = 0; y < bdata->h; y++ )
    {
        pb = bdata->pdata + y * linepich;
        for ( x = 0; x < bdata->w; x++ )
        {
            *pb = 0xFF;
            *(pb+1) = 0x0;
            if ( bytespp == 3 || bytespp == 4 )
                *(pb+2) = 0x0;
            if ( bytespp == 4 )
                *(pb+3) = 0xFF;

            pb += bytespp;
        }
    }
#endif
}

#if 1

/*
    << 24bits.bmp 文件分析: >>

$ xxd -l 64 24bits.bmp
         0 1  2 3  4 5  6 7  8 9  A B  C D  E F   0123456789ABCDEF
------------------------------------------------------------------
0000000: 424d 46c7 0000 0000 0000 3600 0000 2800  BMF.......6...(.
0000010: 0000 8200 0000 8200 0000 0100 1800 0000  ................
0000020: 0000 10c7 0000 0000 0000 0000 0000 0000  ................
0000030: 0000 0000 0000 ffff ffff ffff ffff ff62  ...............b

图像数据大小: (10c7 0000)
    0x0000c710 = 50960


$ /bin/ls -l 24bits.bmp
-rw-rw-rw-. 1 sre sre 51028 11月 24 16:06 24bits.bmp

文件大小 = 文件头 + 图像数据大小
    54 + 50960 = 51014
    x = 51028 - 51014 = 14
==>
    文件 24bits.bmp 中有 14 字节的无用数据, 位于文件尾.


$ file 24bits.bmp
24bits.bmp: PC bitmap, Windows 3.x format, 130 x 130 x 24

130 * 3 * 130 == 50700 != 50960
xxd使命令显示图像数据大小 50960 字节是怎么回事? 

   答: (宽以 4 字节对齐)
                   _       _
          50960 = | 130*3/4 | * 4 * 130
                = 98 * 4 * 130
*/


int main (int argc, char *argv[])
{
    sbitData bdata;
    char     dstfile[128];
    
    bdata.isRevert = 1;

    if ( argv[1] == NULL )
    {
        printf("Usage: %s bmpfile\n", argv[0]);
        exit(1);
    }

    if ( ! ReadBMPFile(argv[1], &bdata) )
    {
        printf("NAL ReadBMPFile Error!!!\n");
        exit(1);
    }

    printf("NAL && %d\n", bdata.edformat);

#if 1 // For test
    bmp_test(&bdata);
    bdata.edformat = EBMP_RGB;
#endif

    sprintf(dstfile, "%s_dst.bmp", argv[1]);
    if ( ! WriteBMPFile(dstfile, &bdata) )
    {
        printf("NAL WriteBMPFile Error!!!\n");
        exit(1);
    }

    free(bdata.pdata);

    return 0;
}
#endif
