
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "bmprw.h"


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

int main (int argc, char *argv[])
{
    sbitData *bmpw, *bmpr;
    char     dstfile[128];

    if ( argv[1] == NULL )
    {
        printf("Usage: %s bmpfile\n", argv[0]);
        exit(1);
    }
    sprintf(dstfile, "%s_dst.bmp", argv[1]);

    assert( (bmpr=bmpCreateObjForRead(EBMP_RGB, 0)) );
    assert( bmpRead(argv[1], bmpr) );

    // bmpw = bmpCreateObjForWrite(bmpr->edformat, 0, bmpr->w, bmpr->h,
    //         bmpr->iBitCount, bmpr->pdata);
    bmpw = bmpCreateObjForWrite(EBMP_BGR, 0, bmpr->w, bmpr->h,
            bmpr->iBitCount, bmpr->pdata);
    assert( bmpw != NULL );
    assert( bmpWrite(dstfile, bmpw) );

    assert( bmpDestroyObjForRead(&bmpr) );
    assert( bmpDestroyObjForWrite(&bmpw) );

    return 0;
}
