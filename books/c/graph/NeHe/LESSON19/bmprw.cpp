/*
 * ===========================================================================
 *
 *       Filename:
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
#include "bmprw.h"


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

NALBOOL st_read_bmp_file(FILE *file, sbitData *bdata)
{
    assert(file != NULL && bdata != NULL);
    assert(bdata->pdata == NULL);

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
        printf("Only support 16 24 32 bits Format for read!\n");
        return NALFALSE;
    }

    // bmp文件格式要求行以 4 字节对齐
    linepich = ((((info.biWidth*info.biBitCount)>>3)+3)>>2)<<2;
    assert( linepich - ((info.biWidth*info.biBitCount)>>3) <= 3 );

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
            st_format_transform(dbuf, linepich, bdata->iBitCount,
                    bdata->edformat);
            dbuf -= linepich;
        }
    }
    else
    {
        for ( i = 0; i < bdata->h; i++ )
        {
            if ( fread(dbuf, 1, linepich, file) != linepich )
                goto _READ_ERROR;
            st_format_transform(dbuf, linepich, bdata->iBitCount,
                    bdata->edformat);
            dbuf += linepich;
        }
    }
    return NALTRUE;

_READ_ERROR:
    free(dbuf);
    dbuf = NULL;
    printf("fread Error!!");
_NO_MEMORY:
    return NALFALSE;
}

/* 
 * */
NALBOOL st_write_bmp_file(FILE *file, sbitData *bdata)
{
    assert(file != NULL && bdata != NULL);
    assert(bdata->pdata != NULL);

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

NALBOOL bmpRead(const char* fname, sbitData *bdata)
{
    assert(fname != NULL && bdata != NULL);
    assert(bdata->pdata == NULL);

    NALBOOL         ret = NALFALSE;
    FILE            *pFile = NULL;
    sbmpHeader      header;

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

NALBOOL bmpWrite(const char* fname, sbitData *bdata)
{
    assert(fname != NULL && bdata != NULL);
    assert(bdata->pdata != NULL);

    NALBOOL         ret;
    FILE            *pFile=NULL;

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


sbitData* bmpCreateObjForRead(eDataFormat edf, NALBOOL isRevert)
{
    sbitData *pbd;

    if ( (pbd=(sbitData *)calloc(1, sizeof(sbitData))) == NULL )
        return NULL;

    pbd->edformat = edf;
    pbd->isRevert = isRevert;

    return pbd;
}

NALBOOL bmpDestroyObjForRead(sbitData **bdata)
{
    assert(bdata != NULL && *bdata != NULL && (*bdata)->pdata != NULL);

    free((*bdata)->pdata);
    (*bdata)->pdata = NULL;

    free(*bdata);
    *bdata = NULL;

    return NALTRUE;
}

sbitData* bmpCreateObjForWrite(eDataFormat edf, NALBOOL isRevert, 
        NALINT w, NALINT h, NALINT bpp, NALBYTE *pd)
{
    assert(pd != NULL);

    sbitData *pbd;

    if ( (pbd=(sbitData *)calloc(1, sizeof(sbitData))) == NULL )
        return NULL;

    pbd->edformat = edf;
    pbd->isRevert = isRevert;
    pbd->w = w;
    pbd->h = h;
    pbd->iBitCount = bpp;
    pbd->pdata = pd;

    return pbd;
}

NALBOOL bmpDestroyObjForWrite(sbitData **bdata)
{
    assert(bdata != NULL && *bdata != NULL && (*bdata)->pdata != NULL);

    free(*bdata);
    *bdata = NULL;

    return NALTRUE;
}
