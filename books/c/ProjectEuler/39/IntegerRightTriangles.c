/*
 * ===========================================================================
 *
 *       Filename:  IntegerRightTriangles.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012/12/31 14时40分31秒
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

/// Problem 39

If p is the perimeter of a right angle triangle with integral length sides,
{a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

*/

#include <string.h>
#include <stdio.h>


int print_right_item_in_perimeter(int p)
{
    int results[p+1];

    memset(results, 0, (p+1)*sizeof(int));
    for ( int i = 1; i <= p; i++ ) {
        for ( int j = i; j <= p; j++ ) {
            for ( int k = j; k <= p; k++ ) {
                if ( i+j+k <= p && i*i + j*j == k*k ) {
                    results[i+j+k]++;
                }
            }
        }
    }

    int max = 0, per = 0;
    for ( int a = 0; a <= p; a++ ) {
        if ( max < results[a] ) {
            max = results[a];
            per = a;
            printf("jj %d, %d\n", max, per);
        }
    }

    return per;
}

int main (int argc, char *argv[])
{
    print_right_item_in_perimeter(1000);

    return 0;
}

