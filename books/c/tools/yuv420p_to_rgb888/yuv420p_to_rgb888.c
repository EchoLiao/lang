/*
 * ===========================================================================
 *
 *       Filename:  yuv420p_to_rgb888.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012/09/07 10时36分05秒
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

/*
 *    *.yuv420p file format:
 *
 *    bytes       contents
 *  ---------------------------
 *    0  -  3     width
 *    4  -  7     height
 *    8  -  ...   Y U V data
 * 
 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bmprw.h"

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;


typedef struct stYUV420P
{
    int w;
    int h;
    uint8_t *y;
    uint8_t *u;
    uint8_t *v;
} stYUV420P;

typedef struct stRGB888
{
    int w;
    int h;
    uint8_t *rgb;
} stRGB888;




static uint8_t clamp(uint16_t vv)
{
    // while ( vv > 255 ) {
    //     vv -= 256;
    // }
    // return vv;

    return vv % 256;
}


int st_YUV420P_to_RGB888(uint8_t *rgb, int width, int height,
        const uint8_t *yuvY, const uint8_t *yuvU, const uint8_t *yuvV)
{
    int w, h;
    uint16_t y, u, v, r, g, b;
    // uint16_t c, d, e;

    for ( h = 0; h < height; h++ ) {
        for ( w = 0; w < width; w++ ) {
            y = *(yuvY + h * width + w);
            u = *(yuvU + (h>>1) * (width>>1) + (w>>1));
            v = *(yuvV + (h>>1) * (width>>1) + (w>>1));

            // // Algorithm 1:
            // c = y - 16;
            // d = u - 128;
            // e = v - 128;
            // r = (298 * c + 409 * e + 128)>>8;
            // g = (298 * c - 100 * d - 208 * e + 128)>>8;
            // b = (298 * c + 516 * d + 128)>>8;

            // // Algorithm 2:
            u = u - 128;
            v = v - 128;
            r = y + v + (v>>2) + (v>>3) + (v>>5);
            g = y - ((u>>2) + (u>>4) + (u>>5)) - ((v>>1) + (v>>3) + (v>>4) + (v>>5));
            b = y + u + (u>>1) + (u>>2) + (u>>6);

            // // Algorithm 3:
            // r = y + 1.402 * (v - 128);
            // g = y - 0.344 * (u - 128) - 0.714 * (v - 128);
            // b = y + 1.772 * (u - 128);

            r = clamp(r);
            g = clamp(g);
            b = clamp(b);
            rgb[h * width * 3 + w * 3 + 0] = r;
            rgb[h * width * 3 + w * 3 + 1] = g;
            rgb[h * width * 3 + w * 3 + 2] = b;
        }
    }
    return 1;
}


int YUV420P_to_RGB888(stRGB888 *rgb, const stYUV420P *yuv)
{
    rgb->w = yuv->w;
    rgb->h = yuv->h;
    return st_YUV420P_to_RGB888(rgb->rgb, yuv->w, yuv->h,
            yuv->y, yuv->u, yuv->v);
}

int file_yuv420p_to_bmp24(const char *bmpFile, const char *yuvFile)
{
    int ret;
    FILE *pYUV = NULL;
    stYUV420P stYUV;
    stRGB888 stRGB;

    pYUV = fopen(yuvFile, "r");
    if ( pYUV == NULL ) return 0;

    ret = fread(&stYUV.w, 1, 4, pYUV);
    ret = fread(&stYUV.h, 1, 4, pYUV);
    printf("NAL &&& w=%d, h=%d\n", stYUV.w, stYUV.h);

    stYUV.y = malloc(stYUV.w * stYUV.h * sizeof(uint8_t));
    stYUV.u = malloc((stYUV.w/2) * (stYUV.h/2) * sizeof(uint8_t));
    stYUV.v = malloc((stYUV.w/2) * (stYUV.h/2) * sizeof(uint8_t));
    stRGB.rgb = malloc(stYUV.w * stYUV.h * 3 * sizeof(uint8_t));
    assert(stYUV.y && stYUV.u && stYUV.v && stRGB.rgb);

    ret = fread(stYUV.y, 1, stYUV.w * stYUV.h, pYUV);
    ret = fread(stYUV.u, 1, (stYUV.w/2) * (stYUV.h/2), pYUV);
    ret = fread(stYUV.v, 1, (stYUV.w/2) * (stYUV.h/2), pYUV);
    assert(ret == (stYUV.w/2) * (stYUV.h/2));
    fclose(pYUV);

    YUV420P_to_RGB888(&stRGB, &stYUV);

    sbitData *bmpw;
    bmpw = bmpCreateObjForWrite(EBMP_RGB, 1, stRGB.w, stRGB.h, 24, stRGB.rgb);
    assert( bmpw != NULL );
    ret = bmpWrite(bmpFile, bmpw);
    assert(ret);
    ret = bmpDestroyObjForWrite(&bmpw);
    assert(ret);

    free(stYUV.y);
    free(stYUV.u);
    free(stYUV.v);
    free(stRGB.rgb);

    return 1;
}

