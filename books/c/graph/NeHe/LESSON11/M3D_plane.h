/*
 * ===========================================================================
 *
 *       Filename:  N3D_plane.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月07日 10时56分02秒
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

#ifndef N3D_PLANE_H_
#define N3D_PLANE_H_

#include "M3D_math3d.h"


/* 
 *
 *   A*x + B*y + C*z + D = 0
 *
 *   normal vertex: N = (A, B, C)
 *
 * */
typedef float M3DPlane4f[4];



// Load plane
void m3dLoadPlanef(M3DPlane4f plane, float a, float b, float c, float d);
void m3dLoadPlanev(M3DPlane4f plane, M3DVector3f n, float d);

// is point on this plane?
int m3dPointIsOnPlane(M3DPlane4f plane, M3DVector3f point);

// return the distance of point to the plane
float m3dDistanceToPlane(M3DPlane4f plane, M3DVector3f point);

// return the intersection point of the ray to this plane
int m3dRayIntersection(M3DPlane4f plane, M3DVector3f insec,
        M3DVector3f rayPos, M3DVector3f rayDir);

#endif // N3D_PLANE_H_
