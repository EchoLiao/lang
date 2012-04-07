/*
 * ===========================================================================
 *
 *       Filename:  nal_fps.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年03月02日 10时59分52秒
 *        Created:  
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

#ifndef NAL_FPS_H_
#define NAL_FPS_H_


extern void  Fps_initProgTime();
extern float Fps_getProgTime();
extern void  Fps_updateFPS();
extern float Fps_getFPS();

#endif
