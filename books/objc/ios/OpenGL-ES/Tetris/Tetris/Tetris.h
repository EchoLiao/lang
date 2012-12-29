/*
 * ===========================================================================
 *
 *       Filename:  Tetris.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012/12/29 10时40分31秒
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

#ifndef Tetris_h_
#define Tetris_h_

#define kMapW   14
#define kMapH   26
#define kWallW   (kMapW + 2)
#define kWallH   (kMapH + 1)
#define kShapeW   4
#define kShapeH   4

extern int x;
extern int y;
extern int map[kWallW][kWallH];

int activatingShapeFlag(int pos);
int Tetrisblok();
int turn();
int left();
int right();
int down();
int actionPerformed();


#endif

