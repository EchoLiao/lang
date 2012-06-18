/*
 * ===========================================================================
 *
 *       Filename:  tsl_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年06月14日 14时16分29秒
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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tsl.h"


int main (int argc, char *argv[])
{
    const char  *file = "./000020.tsl";
    int         i, j, ret;
    FILE        *fp = NULL;

    TSLFileHead  head;
    TSLMediaHead title;
    TSLLyrics   *lyrics = NULL;
    int         nline = 0;

    ret = tslOpen(&fp, file);
    assert(ret == 1);
    ret = tslReadHead(&head, fp);
    assert(ret == 1);
    ret = tslRead(&lyrics, &nline, &title, fp);
    assert(ret == 1);

    printf("head=%s, Version=%d, crc=%d, fonts=%d, pos=%d,"
            "nlyrics=%d, ctype=%d\n",
            head.head, head.version, head.crc, head.fonts,
            head.positions, head.nlyrics ,head.codetype);

    printf("title: type=%d, eff=%d, times=%d, btime=%d\n",
            title.type, title.effect, title.showtime, title.begintime);
    for ( i = 0; i < NMEDIASMAX; i++ )
        printf("     font=%d, pos=%d, %s\n", title.contents[i].fontid,
                title.contents[i].posid, title.contents[i].content);

    for ( i = 0; i < nline; i++ )
    {
        printf("id=%d: ncell=%d, bt=%d, s=%s\n",
                i+1, lyrics[i].ncell, lyrics[i].begintime, lyrics[i].content);
        for ( j = 0; j < lyrics[i].ncell; j++ )
        {
            // printf(" (%.4f,%d)", lyrics[i].cells[j].percent / 10000.0,
            //         lyrics[i].cells[j].totaltimes);
        }
        // printf("\n");
    }

    return 0;
}
