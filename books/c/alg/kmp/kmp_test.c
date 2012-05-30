/*
 * ===========================================================================
 *
 *       Filename:  kmp_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月29日 21时58分02秒
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
#include <stdlib.h>
#include <stdio.h>

#include "kmp.h"
#include "kmp_test.h"


#define SIZE_T  1024
#define SIZE_P  1024


void verification(int idx)
{
    int i, ret1, ret2;
    char T[SIZE_T], P[SIZE_P] = "CGMRQ";

    for ( i = 0; i < SIZE_T; i++ )
    {
        T[i] = 'A' + rand() % ('Z' - 'A') + 1;
    }
    T[SIZE_T - 1] = '\0';

    ret1 = kmp_find(T, P);
    ret2 = bf_find(T, P);
    assert(ret1 == ret2);

    if ( ret1 != -1 )
        printf("idx = %d : ret1 = %d; ret2 = %d\n", idx, ret1, ret2);
}


int main (int argc, char *argv[])
{
    int i;

    for ( i = 0; i < 10000000; i++ )
        verification(i);

    return 0;
}

