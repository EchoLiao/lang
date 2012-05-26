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
#define SODK_CAL_MAX_COUNT      1000000


enum em_sodkAlg {
    SODK_ALG_RANDOM,
    SODK_ALG_INTERVAL,
    SODK_ALG_SNAKE,
    SODK_ALG_ORDER,
    SODK_ALG_COUNT
};

typedef struct tagSodkDigInfos {
    enum em_sodkGrade   mGrade;
    enum em_sodkAlg     mAlg;
    int                 mAreaLow;
    int                 mRowLow;
    int                 mColLow;
} SodkDigInfos;



static unsigned int Rand_int(unsigned int a, unsigned int b);
static void Rand_randXN(int *X, int n);

static void sodk_ST_resetBanDig();
static void sodk_ST_initHerp();

static int  sodk_ST_constraintRowCol(int *tab, int idx,
        const SodkDigInfos *dig);
static int  sodk_ST_canDig(int *tab, int idx);
static int  sodk_ST_algVri(int *tab, const int *sq, const SodkDigInfos *dig);
// static int  sodk_ST_verifySixs();
static int  sodk_ST_oriIsOK(int *tab);
static int  sodk_ST_isOK(const int *tab, int idx);

static int  sodk_ST_cal(int *tab);
static int  sodk_ST_algRandom(int *tab, const SodkDigInfos *dig);
static int  sodk_ST_algInterval(int *tab, const SodkDigInfos *dig);
static int  sodk_ST_algSnake(int *tab, const SodkDigInfos *dig);
static int  sodk_ST_algOrder(int *tab, const SodkDigInfos *dig);


static long long    g_calCount;
static int          g_curTabIdx;
static int          g_sodkHerp[SODK_ROWS * SODK_COLS];
static int          g_sodkBanDig[SODK_ROWS * SODK_COLS];
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
static const SodkDigInfos g_sodkDigs[SODK_GRADE_COUNT] = {
    { SODK_GRADE_LOW,     SODK_ALG_RANDOM,   55, 6, 6 },
    { SODK_GRADE_PRIMARY, SODK_ALG_RANDOM,   45, 4, 4 },
    { SODK_GRADE_MIDDLE,  SODK_ALG_INTERVAL, 40, 3, 3 },
    { SODK_GRADE_HIGH,    SODK_ALG_SNAKE,    27, 1, 1 },
    { SODK_GRADE_ASHES,   SODK_ALG_ORDER,    26, 0, 0 },
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

/*==========================================================================*
 * @Description:
 *      返回一个整形随机数组, 大小为n, 元素是唯一的, 且其元素值介于0和n-1之间.
 *
 * @Param   n
 *
 *==========================================================================*/
static void Rand_randXN(int *X, int n)
{
    assert(n >= 1);

    int i, a, t;

    for ( i = 0; i < n; i++ )
        X[i] = i;

    // srand((unsigned int)time(NULL));
    for ( i = 0; i < n; i++ )
    {
        a = Rand_int(i, n - 1);
        t = X[a];
        X[a] = X[i];
        X[i] = t;
    }
}

#if 0
static int sodk_ST_verifySixs()
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
#endif

static void sodk_ST_resetBanDig()
{
    int i;
    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
        g_sodkBanDig[i] = SODK_DIGI_CANDIG;
}

static int sodk_ST_oriIsOK(int *tab)
{
    int i;
    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
        if ( sodk_DE_isValidDigi(tab[i]) && !sodk_ST_isOK(tab, i) )
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
        if ( i != idx && sodk_DE_isValidDigi(tab[i]) && tab[idx] == tab[i] )
            return 0;


    for ( i = col; i < SODK_COLS*SODK_ROWS; i += SODK_ROWS )
        if ( i != idx && sodk_DE_isValidDigi(tab[i]) && tab[idx] == tab[i] )
            return 0;


    for ( i = 0; i < SODK_ROWS; i++ )
        for ( j = 0; j < SODK_COLS; j++ )
            if ( idx == g_sodkSixs[i][j] )
                goto _FINDED_IDX;

_FINDED_IDX:
    assert( i != SODK_ROWS && j != SODK_COLS );
    for ( k = 0; k < SODK_ROWS; k++ )
        if ( k != j && tab[idx] == tab[g_sodkSixs[i][k]] )
            return 0;

    return 1;
}

static void sodk_ST_initTab(int *tab)
{
    int i, j;

    for ( i = 0; i < SODK_ROWS; i++ )
    {
        for ( j = 0; j < SODK_COLS; j++ )
        {
            tab[i*SODK_ROWS+j] = SODK_DIGI_ORI;
        }
    }
}

static void sodk_ST_initHerp()
{
    int i, j;

    for ( i = 0; i < SODK_ROWS; i++ )
    {
        for ( j = 0; j < SODK_COLS; j++ )
        {
            g_sodkHerp[i*SODK_ROWS+j] = SODK_DIGI_LOW;
        }
    }
}

static int sodk_ST_cal(int *tab)
{
    assert(tab != NULL);
    int i, j, k, idx;
    int iFirstSpace = -1;

    sodk_ST_initHerp();

    g_calCount = 0;
    for ( i = 0; i < SODK_ROWS; i++ )
        for ( j = 0; j < SODK_COLS; j++ )
            if ( !sodk_DE_isValidDigi(tab[i*SODK_ROWS+j]) )
            {
                iFirstSpace = i*SODK_ROWS+j;
                goto _FINDED_FIRST_SPACE;
            }
_FINDED_FIRST_SPACE:
    assert(iFirstSpace != -1);

    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
    {
        idx = i;
        if ( !sodk_DE_isValidDigi(tab[idx]) )
        {
            for ( k = g_sodkHerp[idx]; k <= SODK_DIGI_HIGH; k++ )
            {
                tab[idx] = k;
                g_sodkHerp[idx] = k + 1;
                if ( sodk_ST_isOK(tab, idx) )
                {
                    break;
                }
            }
            if ( k > SODK_DIGI_HIGH && idx > iFirstSpace )
            {
                g_sodkHerp[idx] = SODK_DIGI_LOW;
                tab[idx-1] = tab[idx] = SODK_DIGI_ORI;
                i -= 2;
            }
            else if ( k > SODK_DIGI_HIGH && idx <= iFirstSpace )
            {
#ifdef NAL_DEBUG
                printf("NAL && iF=%d, idx=%d\n", iFirstSpace, idx);
                st_print(tab);
                assert(0);
#endif
                return 0;
            }
        }
        if ( g_calCount > SODK_CAL_MAX_COUNT ) // MAGIC
            return 0;
    }

    return 1;
}

static int sodk_ST_constraintRowCol(int *tab, int idx,
        const SodkDigInfos *dig)
{
    int i;
    int row = idx / SODK_ROWS;
    int col = idx % SODK_COLS;
    int nrRowOri = 0, nrColOri = 0;

    if ( g_sodkBanDig[idx] == SODK_DIGI_BANDIG )
        return 0;

    if ( !sodk_DE_isValidDigi(tab[idx]) )
        return 0;

    for ( i = row*SODK_ROWS; i < row*SODK_ROWS+SODK_COLS; i++ )
        if ( sodk_DE_isValidDigi(tab[i]) )
            nrRowOri++;

    for ( i = col; i < SODK_COLS*SODK_ROWS; i += SODK_ROWS )
        if ( sodk_DE_isValidDigi(tab[i]) )
            nrColOri++;

    if ( nrRowOri < dig->mRowLow || nrColOri < dig->mColLow )
        return 0;

    return 1;
}

static int sodk_ST_canDig(int *tab, int idx)
{
    int i;
    int tabTmp[SODK_ROWS*SODK_COLS];
    int iTmp = tab[idx];

    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
        tabTmp[i] = tab[i];

    for ( i = SODK_DIGI_LOW; i <= SODK_DIGI_HIGH; i++ )
    {
        if ( i != iTmp )
        {
            tabTmp[idx] = i;
            if ( sodk_ST_isOK(tabTmp, idx) )
            {
                if ( sodk_ST_cal(tabTmp) )
                    return 0;
            }
        }
    }

    return 1;
}

static int sodk_ST_algVri(int *tab, const int *sq, const SodkDigInfos *dig)
{
    int i, k, idx;

    sodk_ST_resetBanDig();
    for ( i = SODK_ROWS * SODK_COLS, k = 0; i > dig->mAreaLow; )
    {
        do {
            if ( k >= SODK_ROWS * SODK_COLS )
                return 0;
            idx = sq[k++];
        } while ( !sodk_ST_constraintRowCol(tab, idx, dig) );
        // printf("NAL 2 +======+ i=%d\n", i);

        if ( sodk_ST_canDig(tab, idx) )
        {
            // printf("NAL 4 +===========+ i=%d\n", i);
            tab[idx] = SODK_DIGI_DIG;
            i--;
        }
        else
        {
            // printf("NAL 5 +===========+ i=%d\n", i);
            g_sodkBanDig[idx] = SODK_DIGI_BANDIG;
        }
    }

    return 1;
}

static int sodk_ST_algRandom(int *tab, const SodkDigInfos *dig)
{
    int rA[SODK_ROWS*SODK_COLS];

    Rand_randXN(rA, SODK_ROWS*SODK_COLS);

    return sodk_ST_algVri(tab, rA, dig);
}

static int sodk_ST_algInterval(int *tab, const SodkDigInfos *dig)
{
#if 0
    int j, m;
    int iA[SODK_ROWS * SODK_COLS];
    for ( j = 0, m = 0; m < 2; m++ )
    {
        for ( i = 0; i < SODK_COLS; i++ )
        {
            if ( i % 2 == 0 )
            {
                for ( k = i*SODK_ROWS; k < i*SODK_ROWS+SODK_COLS; k++ )
                    if ( k % 2 == m )
                        iA[j++] = k;
            }
            else 
            {
                for ( k = i*SODK_ROWS+SODK_COLS - 1; k >= i*SODK_ROWS; k-- )
                    if ( k % 2 == m )
                        iA[j++] = k;
            }
        }
    }
    assert(j == SODK_ROWS * SODK_COLS);
#else
    int iA[SODK_ROWS * SODK_COLS] = {
        0, 2, 4, 6, 8, 16, 14, 12, 10, 
        18, 20, 22, 24, 26, 34, 32, 30, 28, 
        36, 38, 40, 42, 44, 52, 50, 48, 46, 
        54, 56, 58, 60, 62, 70, 68, 66, 64, 
        72, 74, 76, 78, 80,
        1, 3, 5, 7, 17, 15, 13, 11, 9,
        19, 21, 23, 25, 35, 33, 31, 29, 27,
        37, 39, 41, 43, 53, 51, 49, 47, 45,
        55, 57, 59, 61, 71, 69, 67, 65, 63,
        73, 75, 77, 79
    };
    assert(sizeof(iA) / sizeof(iA[0]) == SODK_ROWS*SODK_COLS);
#endif

    return sodk_ST_algVri(tab, iA, dig);
}

static int sodk_ST_algSnake(int *tab, const SodkDigInfos *dig)
{
    int iA[SODK_ROWS * SODK_COLS] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8,
        17, 16, 15, 14, 13, 12, 11, 10, 9,
        18, 19, 20, 21, 22, 23, 24, 25, 26,
        35, 34, 33, 32, 31, 30, 29, 28, 27,
        36, 37, 38, 39, 40, 41, 42, 43, 44,
        53, 52, 51, 50, 49, 48, 47, 46, 45,
        54, 55, 56, 57, 58, 59, 60, 61, 62,
        63, 64, 65, 66, 67, 68, 69, 70, 71,
        80, 79, 78, 77, 76, 75, 74, 73, 72
    };
    assert(sizeof(iA) / sizeof(iA[0]) == SODK_ROWS*SODK_COLS);

    return sodk_ST_algVri(tab, iA, dig);
}

static int  sodk_ST_algOrder(int *tab, const SodkDigInfos *dig)
{
    int i, iA[SODK_ROWS * SODK_COLS];

    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
    {
        iA[i] = i;
    }

    return sodk_ST_algVri(tab, iA, dig);
}

int sodk_create(int *tab)
{
    assert(tab != NULL);

    int k;

#if 0
    assert(sodk_ST_verifySixs());
#endif

    do {
        do { 
            sodk_ST_initTab(tab);
            sodk_ST_initHerp();
            for ( k = 0; k < 11; k++ ) /* 11 is a MAGIC Number */
            {
                int rIdx = Rand_int(0, SODK_ROWS*SODK_COLS-1);
                int rVal = Rand_int(SODK_DIGI_LOW, SODK_DIGI_HIGH);
                tab[rIdx] = rVal;
            }
        } while ( ! sodk_ST_oriIsOK(tab) );
        g_curTabIdx++;
        // printf("HHHHHHHHHHHHHHHHHHHHHHHH g_curTabIdx=%d\n", g_curTabIdx);
    } while ( ! sodk_ST_cal(tab) );

    return 1;
}

int sodk_verification(int *tab)
{
    int i;
    for ( i = 0; i < SODK_ROWS * SODK_COLS; i++ )
    {
        if ( ! sodk_DE_isValidDigi(tab[i]) )
            return 0;
        if ( ! sodk_ST_isOK(tab, i) )
            return 0;
    }
    return 1;
}

int sodk_dig(int *tab, enum em_sodkGrade egd)
{
    assert(tab != NULL && egd >= 0 && egd < SODK_GRADE_COUNT);

    int                ret;
    const SodkDigInfos *pDigInfo = &g_sodkDigs[egd];

    assert(pDigInfo->mGrade == egd);
    switch ( pDigInfo->mAlg )
    {
        case SODK_ALG_RANDOM:
            ret = sodk_ST_algRandom(tab, pDigInfo);
            break;
        case SODK_ALG_INTERVAL:
            ret = sodk_ST_algInterval(tab, pDigInfo);
            break;
        case SODK_ALG_SNAKE:
            ret = sodk_ST_algSnake(tab, pDigInfo);
            break;
        case SODK_ALG_ORDER:
            ret = sodk_ST_algOrder(tab, pDigInfo);
            break;
        default:
            assert(0);
            break;
    }

    return ret;
}
