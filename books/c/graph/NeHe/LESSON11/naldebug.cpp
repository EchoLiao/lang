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

    if ( info.biCompression == 0 ) // Format is BI_RGB ?
        bdata->edformat = EBMP_BGR;
    else
        bdata->edformat = EBMP_UNKNOWN;
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

    unsigned int i, linepich, igap;
    unsigned char   gaps[3] = { 0 };
    unsigned char   *sbuf;
    sbmpHeader      header;
    sbmpInfoHeader  info;

    linepich = (((bdata->w * (bdata->iBitCount>>3))+3)>>2)<<2;
    igap = linepich - ((bdata->w * bdata->iBitCount)>>3);
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

    unsigned int srcw = bdata->w * (bdata->iBitCount >> 3);
    fseek(file, BMP_DATA_OFFSET, SEEK_SET);
    if ( bdata->isRevert )
    {
        sbuf = bdata->pdata + linepich * (bdata->h - 1);
        for ( i = 0; i < bdata->h; i++ )
        {
            if ( fwrite(sbuf, 1, srcw, file) != srcw )
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
            if ( fwrite(sbuf, 1, srcw, file) != srcw )
                goto _WRITE_ERROR;
            if ( igap != 0 )
                if ( fwrite(gaps, 1, igap, file) != igap )
                    goto _WRITE_ERROR;
            sbuf += linepich;
        }
    }

    return NALTRUE;

_WRITE_ERROR:
    printf("fwrite Error!!\n");
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
    linepich = bdata->w * bytespp;
    for ( y = 0; y < bdata->h; y++ )
    {
        pb = bdata->pdata + y * linepich;
        for ( x = 0; x < bdata->w; x++ )
        {
            if ( *pb != 0 && *(pb+1) != 0 && *(pb+2) != 0 )
                printf("x");
            else
                printf("0");
            pb += 3;
        }
        printf("\n");
    }

}

#if 1
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

    // bmp_test(&bdata);

    sprintf(dstfile, "%s_dst.bmp", argv[1]);

    // bdata.w -= 1;
    if ( ! WriteBMPFile(dstfile, &bdata) )
    {
        printf("NAL WriteBMPFile Error!!!\n");
        exit(1);
    }

    free(bdata.pdata);

    return 0;
}
#endif
