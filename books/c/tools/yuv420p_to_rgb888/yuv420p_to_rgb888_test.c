/*
 * ===========================================================================
 *
 *       Filename:  yuv420p_to_rgb888_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012/09/07 11时21分12秒
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
#include "yuv420p_to_rgb888.h"
#include "yuv420p_to_rgb888_test.h"



// $ ./yuv420p_to_rgb888_test nal.yuv420p

int main (int argc, char *argv[])
{
    assert(argc == 2);

    const char *saveBmpFile = "out.bmp";
        
    file_yuv420p_to_bmp24(saveBmpFile, argv[1]);

    return 0;
}
