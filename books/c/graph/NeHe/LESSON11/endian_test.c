/*
 * ===========================================================================
 *
 *       Filename:  endian_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011年12月01日 10时05分28秒
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

/*
// 加一种测试大小端的方法
static union {
    char c[4];
    unsigned int l;
} endian_test = { { 'l', '?', '?', 'b' } };

// (如果ENDIANNESS='l'表示系统为little endian,为'b'表示big endian ). 
#define ENDIANNESS ((char)endian_test.l)
*/

void endian_test(void)
{
    printf("ENDIAN sizeof(int*)         = %d\n", sizeof(int*));
    printf("ENDIAN sizeof(char)         = %d\n", sizeof(char));
    printf("ENDIAN sizeof(short)        = %d\n", sizeof(short));
    printf("ENDIAN sizeof(int)          = %d\n", sizeof(int));
    printf("ENDIAN sizeof(long)         = %d\n", sizeof(long));
    printf("ENDIAN sizeof(long long)    = %d\n", sizeof(long long));
    printf("ENDIAN sizeof(float)        = %d\n", sizeof(float));
    printf("ENDIAN sizeof(double)       = %d\n", sizeof(double));
    printf("\n");

    union
    {
        short  s;
        char   c[sizeof(short)];
    } un;
    un.s = 0x0102;
    /* 
     * short:    0x0102
     * char[2]:    0102     ==>  big-endian
     * char[2]:    0201     ==>  little-endian
     */
    if (sizeof(short) == 2)
    {
        if (un.c[0] == 1 && un.c[1] == 2)
            printf("ENDIAN: big-endian\n");
        else if (un.c[0] == 2 && un.c[1] == 1)
            printf("ENDIAN: little-endian\n");
        else
            printf("ENDIAN: unknown\n");
    } else
        printf("ENDIAN sizeof(short) = %d\n", sizeof(short));
    printf("\n");

    // long long lli = 0x33;
    long long lli = 0x0000003300000000;
    long *l1, *l2;
    l1 = (long*)&lli;
    l2 = ((long*)(&lli)) + 1;
    if ( un.c[0] == 2 && un.c[1] == 1 )
    {
        printf("Assert test:");
        assert(*l1 == 0x00000000);
        assert(*l2 == 0x00000033);
        printf("\tTrue!\n\n");
    }
    printf("DBP NUM lli is(%%llx) 0x%llx \n", lli);
    printf("DBP NUM lli is(%%lx) 0x%lx \n", lli);
    printf("DBP NUM l1=0x%lx, l2=0x%lx\n", *l1, *l2);
}


int main()
{

    endian_test();

    return 0;
}
