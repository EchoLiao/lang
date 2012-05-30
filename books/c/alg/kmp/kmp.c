/*
 * ===========================================================================
 *
 *       Filename:  kmp.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月29日 21时38分53秒
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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "kmp.h"


static void kmp_ST_next(const char *p, int *next, int n)
{
    assert(p != NULL && next != NULL && n > 0);
    assert(strlen(p) == n);

    int j = 0, k = -1;
    next[0] = -1;

    while ( j < n - 1 )
    {
        if ( k == -1 || p[j] == p[k] )
        {
            j++; k++;
            next[j] = k;
        }
        else
        {
            /* 此刻, 有: (k!=-1) 且 (P[j]!=P[k]) 且 (next[j]==k);
             * next[j]==(k-1)+1 ==> "Pj-k Pj-k+1 ... Pj-1" == "P0 P1 ... Pk-1"
             *
             * 即: 串"P0 P1 ... Pj"的next值为k, 而P[j]!=P[k],
             *     所以, 串"P0 P1 ... Pj+1"的next值(next[j+1])只可能出现在
             *     串"P0 P1 ... Pk-1"中!!
             *
             *   由 "Pj-k Pj-k+1 ... Pj-1" == "P0 P1 ... Pk-1" 与
             *      "P0 P1 ... Pa" == "Pk-a-1 Pk-a ... Pk-1" (next[k]=a+1)
             *   得, "Pj-a-1 Pj-a ... Pj-1" == "P0 P1 ... Pa"
             *
             *   所以接下来只需要判断Pj是否等于Pa+1, 而Pa+1的下标即是next[k]!
             *   若相等, 则next[j+1]=next[k]+1; 若...
             * */
            k = next[k];
        }
    }
}

/*==========================================================================*
*  @Description:
*       KMP匹配算法.
*       O(m+n)
*
*  @Param  t
*  @Param  p
*
*  @Returns:
*==========================================================================*/
int kmp_find(const char *t, const char *p)
{
    assert(t != NULL && p != NULL);

    int nT = strlen(t);
    int nP = strlen(p);
    int posT = 0, posP = 0;
    int *next = NULL;

    if ( nP == 0 ) return 0;

    if ( NULL == (next = (int *)malloc(nP * sizeof(int))) ) return -1;
    kmp_ST_next(p, next, nP);

    while ( posP < nP && posT < nT )
    {
        if ( posP == -1 || p[posP] == t[posT] )
        {
            posP++;
            posT++;
        }
        else
        {
            posP = next[posP];
        }
    }
    if ( next != NULL ) free(next);

    if ( posP < nP ) return -1;

    return posT - posP;
}

/*==========================================================================*
*  @Description:
*       朴素的模式匹配算法.
*       O(m*n)
*
*  @Param  t
*  @Param  p
*
*  @Returns:
*
*==========================================================================*/
int bf_find(const char *t, const char *p)
{
    assert(t != NULL && p != NULL);

    int i, j;
    int nT = strlen(t);
    int nP = strlen(p);

    for ( i = 0; i <= nT - nP; i++ )
    {
        for ( j = 0; j < nP; j++ )
            if ( t[i+j] != p [j] )
                break;
        if ( j == nP )
            return i;
    }

    return -1;
}

