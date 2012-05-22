/*
 * ===========================================================================
 *
 *       Filename:  soduku.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月21日 16时16分08秒
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
#include <stdlib.h>
#include <assert.h>
#include "soduku.h"


// #define NAL_DEBUG


static int  sodk_ST_oriIsOK(int *tab);
static int  sodk_ST_isOK(const int *tab, int idx);


static long long    g_calCount;
static int          g_curTabIdx;
static int          g_sodkHerp[SODK_ROWS * SODK_COLS];
static const int    g_sodkSixs[9][9] = {
    { 0, 1, 2, 9, 10, 11, 18, 19, 20 },
    { 3, 4, 5, 12, 13, 14, 21, 22, 23 },
    { 6, 7, 8, 15, 16, 17, 24, 25, 26 },
    { 27, 28, 29, 36, 37, 38, 45, 46, 47 },
    { 30, 31, 32, 39, 40, 41, 48, 49, 50 },
    { 33, 34, 35, 42, 43, 44, 51, 52, 53 },
    { 54, 55, 56, 63, 64, 65, 72, 73, 74 },
    { 57, 58, 59, 66, 67, 68, 75, 76, 77 },
    { 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};



/*==========================================================================*
 * @Description:
 *      返回一个整形随机数, 值介于a和b之间, 包括a和b;
 *      return a random number, it's value is in [a, b].
 *
 * @Param   a
 * @Param   b
 *
 * @Returns:
 *      return a random number, it's value is in [a, b].
 *
 *==========================================================================*/
static unsigned int Rand_int(unsigned int a, unsigned int b)
{
    assert( a <= b);

    return a + rand() % (b + 1 - a);
}

int sodk_ST_verifySixs()
{
    int i, j, m, n;

    for ( i = 0; i < 9; i++ )
    {
        for ( j = 0; j < 9; j++ )
        {
            int iTmp = g_sodkSixs[i][j];
            int idx1 = i * 9 + j;
            assert(iTmp >= 0 && iTmp <= 80);
            for ( m = 0; m < 9; m++ )
            {
                for ( n = 0; n < 9; n++ )
                {
                    int idx2 = m * 9 + n;
                    if ( idx1 != idx2 && iTmp == g_sodkSixs[m][n] )
                        return 0;
                }
            }
        }
    }

    return 1;
}

int sodk_create(int *tab)
{
    assert(tab != NULL);

    int i, j, k;

#if 0
    assert(sodk_ST_verifySixs());
#endif

    do {
        do { 
            for ( i = 0; i < SODK_ROWS; i++ )
            {
                for ( j = 0; j < SODK_COLS; j++ )
                {
                    tab[i*SODK_ROWS+j] = 0;
                    g_sodkHerp[i*SODK_ROWS+j] = 1;
                }
            }
            for ( k = 0; k < 11; k++ )
            {
                int rIdx = Rand_int(0, SODK_ROWS*SODK_COLS-1);
                int rVal = Rand_int(1, 9);
                tab[rIdx] = rVal;
            }
        } while ( ! sodk_ST_oriIsOK(tab) );
        g_curTabIdx++;
        // printf("HHHHHHHHHHHHHHHHHHHHHHHH g_curTabIdx=%d\n", g_curTabIdx);
    } while ( ! sodk_cal(tab) );

    return 1;
}

static int sodk_ST_oriIsOK(int *tab)
{
    int i;
    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
        if ( tab[i] != 0 && !sodk_ST_isOK(tab, i) )
            return 0;

    return 1;
}

static int sodk_ST_isOK(const int *tab, int idx)
{
    int i, j, k;
    int row = idx / SODK_ROWS;
    int col = idx % SODK_ROWS;

    g_calCount++;

    for ( i = row*SODK_ROWS; i < row*SODK_ROWS+SODK_COLS; i++ )
        if ( i != idx && tab[i] != 0 && tab[idx] == tab[i] )
            return 0;


    for ( i = col; i < SODK_COLS*SODK_ROWS; i += SODK_ROWS )
        if ( i != idx && tab[i] != 0 && tab[idx] == tab[i] )
            return 0;


    for ( i = 0; i < 9; i++ )
        for ( j = 0; j < 9; j++ )
            if ( idx == g_sodkSixs[i][j] )
                goto _FINDED_IDX;

_FINDED_IDX:
    assert( i != 9 && j != 9 );
    for ( k = 0; k < 9; k++ )
        if ( k != j && tab[idx] == tab[g_sodkSixs[i][k]] )
            return 0;

    return 1;
}

int sodk_verification(int *tab)
{
    int i;
    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
    {
        if ( tab[i] < 1 || tab[i] > 9 )
            return 0;
        if ( ! sodk_ST_isOK(tab, i) )
            return 0;
    }
    return 1;
}

int sodk_cal(int *tab)
{
    assert(tab != NULL);
    int i, j, k, idx;
    int iFirstSpace = -1;

    g_calCount = 0;
    for ( i = 0; i < SODK_ROWS; i++ )
        for ( j = 0; j < SODK_COLS; j++ )
            if ( tab[i*SODK_ROWS+j] == 0 )
            {
                iFirstSpace = i*SODK_ROWS+j;
                goto _FINDED_FIRST_SPACE;
            }
_FINDED_FIRST_SPACE:
    assert(iFirstSpace != -1);

    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
    {
        idx = i;
        if ( tab[idx] == 0 )
        {
            for ( k = g_sodkHerp[idx]; k <= 9; k++ )
            {
                tab[idx] = k;
                g_sodkHerp[idx] = k + 1;
                if ( sodk_ST_isOK(tab, idx) )
                {
                    break;
                }
            }
            if ( k > 9 && idx > iFirstSpace )
            {
                g_sodkHerp[idx] = 1;
                tab[idx] = 0;
                tab[idx-1] = 0;
                i -= 2;
            }
            else if ( k > 9 && idx <= iFirstSpace )
            {
#ifdef NAL_DEBUG
                printf("NAL && iF=%d, idx=%d\n", iFirstSpace, idx);
                st_print(tab);
                assert(0);
#endif
                return 0;
            }
        }
        if ( g_calCount > 1000000 ) // MAGIC
            return 0;
    }

    return 1;
}

