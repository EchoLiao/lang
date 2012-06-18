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
    int         i, ret;
    TSLFile     tslFile;

    memset(&tslFile, 0, sizeof(tslFile));
    ret = tslOpen(&tslFile, file);
    assert(ret == 1);
    ret = tslRead(&tslFile);
    assert(ret == 1);

    printf("title: type=%d, eff=%d, times=%d, btime=%d\n",
            tslFile.title.type, tslFile.title.effect,
            tslFile.title.showtime, tslFile.title.begintime);
    for ( i = 0; i < NMEDIASMAX; i++ )
    {
        printf("       font=%d, pos=%d, %s\n", tslFile.title.contents[i].fontid,
                tslFile.title.contents[i].posid,
                tslFile.title.contents[i].content);
    }

    for ( i = 0; i < tslFile.head.nlyrics; i++ )
    {
        printf("id=%d: ncell=%d, bt=%d, s=%s\n",
                i+1, tslFile.lyrics[i].ncell, tslFile.lyrics[i].begintime,
                tslFile.lyrics[i].content);
    }

    ret = tslClose(&tslFile);
    assert(ret == 1);

    return 0;
}
