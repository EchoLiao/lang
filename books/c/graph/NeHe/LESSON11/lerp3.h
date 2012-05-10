/*
 * ===========================================================================
 *
 *       Filename:  lerp3.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年05月09日 16时02分36秒
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

#ifndef LERP3_H_
#define LERP3_H_


typedef struct N3D_GodPos N3D_GodPos;



int  N3D_lineInsertPos(float k1, float b1, float k2, float b2,
        float *x, float *y);
int  N3D_lineConstruct(float x1, float y1, float x2, float y2,
        float *k, float *b);


int  N3D_godCreate(N3D_GodPos *god);
void N3D_godinit(N3D_GodPos *god);
void N3D_godDestroy(N3D_GodPos *god);

void N3D_godClear();
void N3D_godFlush();


void N3D_godUpdatePos(N3D_GodPos *god);
void N3D_godDraw(N3D_GodPos *god);

void N3D_godUpdatePosByLine(N3D_GodPos *god, int curLine);
void N3D_godDrawByLine(N3D_GodPos *god, int curLine);
void N3D_godDrawAminByLine(N3D_GodPos *god, int curLine);


#endif
