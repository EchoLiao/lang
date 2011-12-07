/*
 * ===========================================================================
 *
 *       Filename:  N3D_plane.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年12月07日 10时56分33秒
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

#include "M3D_plane.h"


void m3dLoadPlanef(M3DPlane4f plane, float a, float b, float c, float d)
{
    plane[0] = a;
    plane[1] = b;
    plane[2] = c;
    plane[3] = d;
}

void m3dLoadPlanev(M3DPlane4f plane, M3DVector3f n, float d)
{
    plane[0] = n[0];
    plane[1] = n[1];
    plane[2] = n[2];
    plane[3] = d;
}

int m3dPointIsOnPlane(M3DPlane4f plane, M3DVector3f point)
{
    return (m3dDistanceToPlane(plane, point) == 0.0);
}

float m3dDistanceToPlane(M3DPlane4f plane, M3DVector3f point)
{
    return (point[0]*plane[0] + point[1]*plane[1] + point[2]*plane[2] 
            + plane[3]); 
}

int m3dRayIntersection(M3DPlane4f plane, M3DVector3f insecOut,
        M3DVector3f rayPos, M3DVector3f rayDir)
{
    const float a = plane[0]*rayDir[0] + plane[1]*rayDir[1] + 
                    plane[2]*rayDir[2];
    if ( a == 0.0 )     // ray is parallel to plane
        return 0;

    const float dis = m3dDistanceToPlane(plane, rayPos) / a;

    insecOut[0] = rayPos[0] - rayDir[0]*dis;
    insecOut[1] = rayPos[1] - rayDir[1]*dis;
    insecOut[2] = rayPos[2] - rayDir[2]*dis;

    return 1;
}

