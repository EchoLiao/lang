/*
 * ===========================================================================
 *
 *       Filename:  soduku_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月21日 16时37分02秒
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
#include "soduku.h"


static int g_sodukuTab[SODK_ROWS * SODK_COLS];




void st_print(int *tab)
{
    int i, j;

    for ( i = 0; i < SODK_ROWS; i++ )
    {
        if ( i % 3 == 0 )
            printf("=====================================\n");
        else
            printf("-------------------------------------\n");

        for ( j = 0; j < SODK_COLS; j++ )
        {
            if ( j % 3 == 0 )
                printf("|");
            else
                printf(" ");
            if ( sodk_DE_isValidDigi(tab[i*SODK_ROWS+j]) )
                printf(" %d ", tab[i*SODK_ROWS+j]);
            else
                printf("   ");
        }
        printf("|");
        printf("\n");
    }
    printf("=====================================\n");
}

int  st_verification(int *tab)
{
     return sodk_verification(tab);
}


int main (int argc, char *argv[])
{
    int i, ret;

    for ( i = 0; i < 100; i++ )
    {
        sodk_create(g_sodukuTab);
        if ( st_verification(g_sodukuTab) )
        {
            // st_print(g_sodukuTab);
        }
        else 
        {
            printf("NAL **(( HAD ERROR!!!! ))*** i=%d\n", i);
            st_print(g_sodukuTab);
            assert(0);
        }

        // ret = sodk_dig(g_sodukuTab, SODK_GRADE_LOW);
        // ret = sodk_dig(g_sodukuTab, SODK_GRADE_PRIMARY);
        // ret = sodk_dig(g_sodukuTab, SODK_GRADE_MIDDLE);
        // ret = sodk_dig(g_sodukuTab, SODK_GRADE_HIGH);
        ret = sodk_dig(g_sodukuTab, SODK_GRADE_ASHES);
        st_print(g_sodukuTab);
        printf("NAL **(())*** i=%d, ret=0x%x\n", i, ret);
    }

    return 0;
}
