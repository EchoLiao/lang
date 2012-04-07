/*
 * ===========================================================================
 *
 *       Filename:  nal_fps.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年03月02日 10时59分57秒
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

#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "nal_fps.h"



// Some global variables used to measure the time
static float            gfTimeAtProgStart;
static int              giTickPerSecond;

// Global variables for measuring fps
static float            gfLastUpdate        = 0.0;
static float            gfFpsUpdateInterval = 0.5f;
static unsigned int     giNumFrames         = 0;
static float            gfFps               = 0.0;



float Fps_getFPS()
{
    return gfFps;
}

/*==========================================================================*
 * @Description:    
 *      Called once at the start of the game
 *
 *==========================================================================*/
void Fps_initProgTime()
{
    int ret;
    struct timespec  timeRes;

    memset(&timeRes, 0, sizeof(timeRes));
    ret = clock_getres(CLOCK_MONOTONIC, &timeRes);
    if ( ret != 0 )
        giTickPerSecond = 1000;

    gfTimeAtProgStart = 0.0;
    gfTimeAtProgStart = Fps_getProgTime();
}

/*==========================================================================*
 * @Description:    
 *      Called every time you need the current game time

 * @Returns:
 *
 *==========================================================================*/
float Fps_getProgTime()
{
    float time;
    int   ret;
    struct timespec  tp;

    ret = clock_gettime(CLOCK_MONOTONIC, &tp);
    time = tp.tv_sec + (float)tp.tv_nsec / 1000000000;
    time -= gfTimeAtProgStart;

    return time;
}

/*==========================================================================*
 * @Description:
 *      Called once for every frame
 *
 *==========================================================================*/
void Fps_updateFPS()
{
    float currentUpdate = Fps_getProgTime();

    giNumFrames++;
    if( currentUpdate - gfLastUpdate > gfFpsUpdateInterval )
    {
        gfFps = giNumFrames / (currentUpdate - gfLastUpdate);
        gfLastUpdate = currentUpdate;
        giNumFrames = 0;
        printf("JJ %28.13f\n", gfFps);
    }
}
