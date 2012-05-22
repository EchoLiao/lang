/*
 * ===========================================================================
 *
 *       Filename:  soduku.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月21日 16时16分12秒
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


#ifndef  SOKUKU_H_
#define SOKUKU_H_


#define SODK_ROWS     9
#define SODK_COLS     9



extern int  sodk_create(int *tab);
extern int  sodk_cal(int *tab);
extern int  sodk_verification(int *tab);



#endif
