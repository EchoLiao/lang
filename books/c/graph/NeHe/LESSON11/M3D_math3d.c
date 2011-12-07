// Implementation of non-inlined functions in the Math3D Library

// These are pretty portable
#include <math.h>
#include "M3D_math3d.h"


////////////////////////////////////////////////////////////
// LoadIdentity
// For 3x3 and 4x4 float and double matricies.
// 3x3 float
void m3dLoadIdentity33(M3DMatrix33f m)
{
    // Don't be fooled, this is still column major
    static M3DMatrix33f	identity = { 1.0f, 0.0f, 0.0f ,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };

    memcpy(m, identity, sizeof(M3DMatrix33f));
}

// 4x4 float
void m3dLoadIdentity44(M3DMatrix44f m)
{
    // Don't be fooled, this is still column major
    static M3DMatrix44f	identity = { 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

    memcpy(m, identity, sizeof(M3DMatrix44f));
}

////////////////////////////////////////////////////////////////////////
// Return the square of the distance between two points
// Should these be inlined...?
float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v)
{
    float x = u[0] - v[0];
    x = x*x;

    float y = u[1] - v[1];
    y = y*y;

    float z = u[2] - v[2];
    z = z*z;

    return (x + y + z);
}

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]

///////////////////////////////////////////////////////////////////////////////
// Multiply two 4x4 matricies
// product = a * b
void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b )
{
    int i;
    for (i = 0; i < 4; i++) {
        float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
        P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
        P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
        P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
        P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
    }
}
#undef A
#undef B
#undef P


#define A33(row,col)  a[(col*3)+row]
#define B33(row,col)  b[(col*3)+row]
#define P33(row,col)  product[(col*3)+row]

///////////////////////////////////////////////////////////////////////////////
// Multiply two 3x3 matricies
void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b )
{
    int i;
    for (i = 0; i < 3; i++) {
        float ai0=A33(i,0), ai1=A33(i,1),  ai2=A33(i,2);
        P33(i,0) = ai0 * B33(0,0) + ai1 * B33(1,0) + ai2 * B33(2,0);
        P33(i,1) = ai0 * B33(0,1) + ai1 * B33(1,1) + ai2 * B33(2,1);
        P33(i,2) = ai0 * B33(0,2) + ai1 * B33(1,2) + ai2 * B33(2,2);
    }
}
#undef A33
#undef B33
#undef P33

#define M33(row,col)  m[col*3+row]

///////////////////////////////////////////////////////////////////////////////
// Creates a 3x3 rotation matrix, takes radians NOT degrees
void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z)
{

    float mag, s, c;
    float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

    s = (sinf(angle));
    c = (cosf(angle));

    mag = (sqrt( x*x + y*y + z*z ));

    // Identity matrix
    if (mag == 0.0f) {
        m3dLoadIdentity33(m);
        return;
    }

    // Rotation matrix is normalized
    x /= mag;
    y /= mag;
    z /= mag;



    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * s;
    ys = y * s;
    zs = z * s;
    one_c = 1.0f - c;

    M33(0,0) = (one_c * xx) + c;
    M33(0,1) = (one_c * xy) - zs;
    M33(0,2) = (one_c * zx) + ys;

    M33(1,0) = (one_c * xy) + zs;
    M33(1,1) = (one_c * yy) + c;
    M33(1,2) = (one_c * yz) - xs;

    M33(2,0) = (one_c * zx) - ys;
    M33(2,1) = (one_c * yz) + xs;
    M33(2,2) = (one_c * zz) + c;
}

#undef M33

///////////////////////////////////////////////////////////////////////////////
// Creates a 4x4 rotation matrix, takes radians NOT degrees
///////////////////////////////////////////////////////////////////////////////
// Creates a 4x4 rotation matrix, takes radians NOT degrees
// 构造一个矩阵, 用于绕任意向量 (x, y, z) 逆时针转 angle 弧度.
// [(<<G:1>> P48)]
void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z)
{
    float mag, s, c;
    float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

    s = (sinf(angle));
    c = (cosf(angle));

    mag = (sqrt( x*x + y*y + z*z ));

    // Identity matrix
    if (mag == 0.0f) {
        m3dLoadIdentity44(m);
        return;
    }

    // Rotation matrix is normalized
    x /= mag;
    y /= mag;
    z /= mag;

#define M(row,col)  m[col*4+row]

    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * s;
    ys = y * s;
    zs = z * s;
    one_c = 1.0f - c;

    M(0,0) = (one_c * xx) + c;
    M(0,1) = (one_c * xy) - zs;
    M(0,2) = (one_c * zx) + ys;
    M(0,3) = 0.0f;

    M(1,0) = (one_c * xy) + zs;
    M(1,1) = (one_c * yy) + c;
    M(1,2) = (one_c * yz) - xs;
    M(1,3) = 0.0f;

    M(2,0) = (one_c * zx) - ys;
    M(2,1) = (one_c * yz) + xs;
    M(2,2) = (one_c * zz) + c;
    M(2,3) = 0.0f;

    M(3,0) = 0.0f;
    M(3,1) = 0.0f;
    M(3,2) = 0.0f;
    M(3,3) = 1.0f;

#undef M
}



/*
 * Compute inverse of 4x4 transformation matrix.
 * Code contributed by Jacques Leroy jle@star.be
 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
 */
int m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src )
{
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

    float wtmp[4][8];
    float m0, m1, m2, m3, s;
    float *r0, *r1, *r2, *r3;

    r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

    r0[0] = MAT(src,0,0), r0[1] = MAT(src,0,1),
        r0[2] = MAT(src,0,2), r0[3] = MAT(src,0,3),
        r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

        r1[0] = MAT(src,1,0), r1[1] = MAT(src,1,1),
        r1[2] = MAT(src,1,2), r1[3] = MAT(src,1,3),
        r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

        r2[0] = MAT(src,2,0), r2[1] = MAT(src,2,1),
        r2[2] = MAT(src,2,2), r2[3] = MAT(src,2,3),
        r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

        r3[0] = MAT(src,3,0), r3[1] = MAT(src,3,1),
        r3[2] = MAT(src,3,2), r3[3] = MAT(src,3,3),
        r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

    /* choose pivot - or die */
    if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
    if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
    if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
    if (0.0 == r0[0])  return 0;

    /* eliminate first variable     */
    m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
    s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
    s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
    s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
    s = r0[4];
    if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
    s = r0[5];
    if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
    s = r0[6];
    if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
    s = r0[7];
    if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

    /* choose pivot - or die */
    if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
    if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
    if (0.0 == r1[1])  return 0;

    /* eliminate second variable */
    m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
    r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
    r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
    s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
    s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
    s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
    s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

    /* choose pivot - or die */
    if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
    if (0.0 == r2[2])  return 0;

    /* eliminate third variable */
    m3 = r3[2]/r2[2];
    r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
        r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
        r3[7] -= m3 * r2[7];

    /* last check */
    if (0.0 == r3[3]) return 0;

    s = 1.0f/r3[3];              /* now back substitute row 3 */
    r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

    m2 = r2[3];                 /* now back substitute row 2 */
    s  = 1.0f/r2[2];
    r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
    m1 = r1[3];
    r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
        r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
    m0 = r0[3];
    r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
        r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

    m1 = r1[2];                 /* now back substitute row 1 */
    s  = 1.0f/r1[1];
    r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
        r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
    m0 = r0[2];
    r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
        r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

    m0 = r0[1];                 /* now back substitute row 0 */
    s  = 1.0f/r0[0];
    r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
        r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

    MAT(dst,0,0) = r0[4]; MAT(dst,0,1) = r0[5],
        MAT(dst,0,2) = r0[6]; MAT(dst,0,3) = r0[7],
        MAT(dst,1,0) = r1[4]; MAT(dst,1,1) = r1[5],
        MAT(dst,1,2) = r1[6]; MAT(dst,1,3) = r1[7],
        MAT(dst,2,0) = r2[4]; MAT(dst,2,1) = r2[5],
        MAT(dst,2,2) = r2[6]; MAT(dst,2,3) = r2[7],
        MAT(dst,3,0) = r3[4]; MAT(dst,3,1) = r3[5],
        MAT(dst,3,2) = r3[6]; MAT(dst,3,3) = r3[7];

    return 1;

#undef MAT
#undef SWAP_ROWS
}

///////////////////////////////////////////////////////////////////////////////////////
// Get Window coordinates, discard Z...
void m3dProjectXY(const M3DMatrix44f mModelView, const M3DMatrix44f
        mProjection, const int iViewPort[4], const M3DVector3f vPointIn,
        M3DVector3f vPointOut)
{
    M3DVector4f vBack, vForth;

    memcpy(vBack, vPointIn, sizeof(float)*3);
    vBack[3] = 1.0f;

    m3dTransformVector4(vForth, vBack, mModelView);
    m3dTransformVector4(vBack, vForth, mProjection);

    if(!m3dCloseEnough(vBack[3], 0.0f, 0.000001f)) {
        float div = 1.0f / vBack[3];
        vBack[0] *= div;
        vBack[1] *= div;
    }


    vPointOut[0] = vBack[0] * 0.5f + 0.5f;
    vPointOut[1] = vBack[1] * 0.5f + 0.5f;

    /* Map x,y to viewport */
    vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
    vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
}

///////////////////////////////////////////////////////////////////////////////////////
// Get window coordinates, we also want Z....
void m3dProjectXYZ(const M3DMatrix44f mModelView, const M3DMatrix44f
        mProjection, const int iViewPort[4], const M3DVector3f vPointIn,
        M3DVector3f vPointOut)
{
    M3DVector4f vBack, vForth;

    memcpy(vBack, vPointIn, sizeof(float)*3);
    vBack[3] = 1.0f;

    m3dTransformVector4(vForth, vBack, mModelView);
    m3dTransformVector4(vBack, vForth, mProjection);

    if(!m3dCloseEnough(vBack[3], 0.0f, 0.000001f)) {
        float div = 1.0f / vBack[3];
        vBack[0] *= div;
        vBack[1] *= div;
        vBack[2] *= div;
    }

    vPointOut[0] = vBack[0] * 0.5f + 0.5f;
    vPointOut[1] = vBack[1] * 0.5f + 0.5f;
    vPointOut[2] = vBack[2] * 0.5f + 0.5f;

    /* Map x,y to viewport */
    vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
    vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misc. Utilities
///////////////////////////////////////////////////////////////////////////////
// Calculates the normal of a triangle specified by the three points
// p1, p2, and p3. Each pointer points to an array of three floats. The
// triangle is assumed to be wound counter clockwise.
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
        const M3DVector3f point3)
{
    M3DVector3f v1,v2;		// Temporary vectors

    // Calculate two vectors from the three points. Assumes counter clockwise
    // winding!
    v1[0] = point1[0] - point2[0];
    v1[1] = point1[1] - point2[1];
    v1[2] = point1[2] - point2[2];

    v2[0] = point2[0] - point3[0];
    v2[1] = point2[1] - point3[1];
    v2[2] = point2[2] - point3[2];

    // Take the cross product of the two vectors to get
    // the normal vector.
    m3dCrossProduct(result, v1, v2);
}



/////////////////////////////////////////////////////////////////////////////////////////
// Calculate the plane equation of the plane that the three specified points lay in. The
// points are given in clockwise winding order, with normal pointing out of clockwise face
// planeEq contains the A,B,C, and D of the plane equation coefficients
//
// 根据给出的不共线的三点求过该三点的平面(Ax+By+Cz+D=0).
// planeEq is (A,B,C,D)
//
// [(<<计算机图形学几何工具算法详解>> P492)]
//
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3)
{
    // Get two vectors... do the cross product
    M3DVector3f v1, v2;

    // V1 = p3 - p1
    v1[0] = p3[0] - p1[0];
    v1[1] = p3[1] - p1[1];
    v1[2] = p3[2] - p1[2];

    // V2 = P2 - p1
    v2[0] = p2[0] - p1[0];
    v2[1] = p2[1] - p1[1];
    v2[2] = p2[2] - p1[2];

    // Unit normal to plane - Not sure which is the best way here
    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    // Back substitute to get D
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// This function does a three dimensional Catmull-Rom curve interpolation. Pass four points, and a
// floating point number between 0.0 and 1.0. The curve is interpolated between the middle two points.
// Coded by RSW
// http://www.mvps.org/directx/articles/catmull/
void m3dCatmullRom3(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t)
{
    // Unrolled loop to speed things up a little bit...
    float t2 = t * t;
    float t3 = t2 * t;

    // X
    vOut[0] = 0.5f * ( ( 2.0f * vP1[0]) +
            (-vP0[0] + vP2[0]) * t +
            (2.0f * vP0[0] - 5.0f *vP1[0] + 4.0f * vP2[0] - vP3[0]) * t2 +
            (-vP0[0] + 3.0f*vP1[0] - 3.0f *vP2[0] + vP3[0]) * t3);
    // Y
    vOut[1] = 0.5f * ( ( 2.0f * vP1[1]) +
            (-vP0[1] + vP2[1]) * t +
            (2.0f * vP0[1] - 5.0f *vP1[1] + 4.0f * vP2[1] - vP3[1]) * t2 +
            (-vP0[1] + 3.0f*vP1[1] - 3.0f *vP2[1] + vP3[1]) * t3);

    // Z
    vOut[2] = 0.5f * ( ( 2.0f * vP1[2]) +
            (-vP0[2] + vP2[2]) * t +
            (2.0f * vP0[2] - 5.0f *vP1[2] + 4.0f * vP2[2] - vP3[2]) * t2 +
            (-vP0[2] + 3.0f*vP1[2] - 3.0f *vP2[2] + vP3[2]) * t3);
}


///////////////////////////////////////////////////////////////////////////////
// Determine if the ray (starting at point) intersects the sphere centered at
// sphereCenter with radius sphereRadius
// Return value is < 0 if the ray does not intersect
// Return value is 0.0 if ray is tangent
// Positive value is distance to the intersection point
// Algorithm from "3D Math Primer for Graphics and Game Development"
///////////////////////////////////////////////////////////////////////////////
// Determine if the ray (starting at point) intersects the sphere centered at
// ditto above, but uses floating point math
float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius)
{
    //m3dNormalizeVectorf(ray);	// Make sure ray is unit length

    M3DVector3f rayToCenter;	// Ray to center of sphere
    rayToCenter[0] =  sphereCenter[0] - point[0];
    rayToCenter[1] =  sphereCenter[1] - point[1];
    rayToCenter[2] =  sphereCenter[2] - point[2];

    // Project rayToCenter on ray to test
    float a = m3dDotProduct(rayToCenter, ray);

    // Distance to center of sphere
    float distance2 = m3dDotProduct(rayToCenter, rayToCenter);	// Or length

    float dRet = (sphereRadius * sphereRadius) - distance2 + (a*a);

    if(dRet > 0.0)			// Return distance to intersection
        dRet = a - sqrtf(dRet);

    return dRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate the tangent basis for a triangle on the surface of a model
// This vector is needed for most normal mapping shaders
void m3dCalculateTangentBasis(const M3DVector3f vTriangle[3], const M3DVector2f vTexCoords[3], const M3DVector3f N, M3DVector3f vTangent)
{
    M3DVector3f dv2v1, dv3v1;
    float dc2c1t, dc2c1b, dc3c1t, dc3c1b;
    float M;

    m3dSubtractVectors3(dv2v1, vTriangle[1], vTriangle[0]);
    m3dSubtractVectors3(dv3v1, vTriangle[2], vTriangle[0]);

    dc2c1t = vTexCoords[1][0] - vTexCoords[0][0];
    dc2c1b = vTexCoords[1][1] - vTexCoords[0][1];
    dc3c1t = vTexCoords[2][0] - vTexCoords[0][0];
    dc3c1b = vTexCoords[2][1] - vTexCoords[0][1];

    M = (dc2c1t * dc3c1b) - (dc3c1t * dc2c1b);
    M = 1.0f / M;

    m3dScaleVector3(dv2v1, dc3c1b);
    m3dScaleVector3(dv3v1, dc2c1b);

    m3dSubtractVectors3(vTangent, dv2v1, dv3v1);
    m3dScaleVector3(vTangent, M);  // This potentially changes the direction of the vector
    m3dNormalizeVector(vTangent);

    M3DVector3f B;
    m3dCrossProduct(B, N, vTangent);
    m3dCrossProduct(vTangent, B, N);
    m3dNormalizeVector(vTangent);
}


////////////////////////////////////////////////////////////////////////////
// Smoothly step between 0 and 1 between edge1 and edge 2
float m3dSmoothStep(float edge1, float edge2, float x)
{
    float t;
    t = (x - edge1) / (edge2 - edge1);
    if(t > 1.0f)
        t = 1.0f;

    if(t < 0.0)
        t = 0.0f;

    return t * t * ( 3.0f - 2.0f * t);
}



///////////////////////////////////////////////////////////////////////////
// Creae a projection to "squish" an object into the plane.
// Use m3dGetPlaneEquationf(planeEq, point1, point2, point3);
// to get a plane equation.
//
// http://www.cppblog.com/summericeyl/archive/2011/03/14/141802.html
// 将模型视图矩阵延灯光方向压平, 所有被绘制的物体都将位于这个平面的二维世界中.
// 灯光方向: |0| - |vLightPos| = -|vLightPos|
//
// 推导过程:
//   假设平面方程 Ax + By + Cz + D = 0 已知, 光的方向L(Lx, Ly, Lz, 0)已知. 则
//   模型视图空间的点 P(Px, Py, Pz, 1), 沿着光的方向投射到平面上的点为 S(Sx,
//   Sy, Sz, 1).其中我们设置向量 N(A, B, C, D). 现在我们需要求的是矩阵M, 使得
//   MP = S. 即 M左乘点P得到点S.
//
//   由于点S是P沿着光的方向L到达的, 所以可假设 S = P + kL (k >= 0).
//   因为点S是平面上的一点, 所以 A*Sx + B*Sy + C*Sz + D = 0. 即 S*N = 0.
//   由于 S = P + kL, 所以 (P + kL) * N = 0, 推导可得出 k = -(P*N)/L*N
//   即:
//       k = -(A*Px + B*Py + C*Pz + D)/(A*Lx + B*Ly + C*Lz),
//
//   代入 S = P + kL, 得
//
//   Sx = Px + k*Lx
//      = Px - (A*Px + B*Py + C*Pz + D)*Lx / (A*Lx + B*Ly + C*Lz)
//      = [Px*(B*Ly + C*Lz) - Py*(B*Lx) - Px*(C*Lx) - 1*D*Lx] / (A*Lx + B*Ly + C*Lz)
//      = P*((B*Ly + C*Lz), -B*Lx, -C*Lx, -D*Lx) / (A*Lx + B*Ly + C*Lz)
//   同理:
//   Sy = P*(-A*Ly, (A*Lx + C*Lz), -C*Ly, -D*Ly) / (A*Lx + B*Ly + C*Lz)
//   Sz = P*(-A*Lz, -B*Lz, (A*Lx + B*Ly), -D*Lz) / (A*Lx + B*Ly + C*Lz)
//   Sw = P*(0, 0, 0, (A*Lx + B*Ly + C*Lz)) / (A*Lx + B*Ly + C*Lz)
//
//   所以可得到矩阵M:
//          |  B*Ly + C*Lz  -B*Lx         -C*Lx         -D*Lx               |
//      M = | -A*Ly          A*Lx + C*Lz  -C*Ly         -D*Ly               |
//          | -A*Lz         -B*Lz          A*Lx + B*Ly  -D*Lz               |
//          |  0             0             0             A*Lx + B*Ly + C*Lz |
//
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos)
{
    // These just make the code below easier to read. They will be
    // removed by the optimizer.
    float a = planeEq[0];
    float b = planeEq[1];
    float c = planeEq[2];
    float d = planeEq[3];

    float dx = -vLightPos[0];
    float dy = -vLightPos[1];
    float dz = -vLightPos[2];

    // Now build the projection matrix
    proj[0] = b * dy + c * dz;
    proj[1] = -a * dy;
    proj[2] = -a * dz;
    proj[3] = 0.0;

    proj[4] = -b * dx;
    proj[5] = a * dx + c * dz;
    proj[6] = -b * dz;
    proj[7] = 0.0;

    proj[8] = -c * dx;
    proj[9] = -c * dy;
    proj[10] = a * dx + b * dy;
    proj[11] = 0.0;

    proj[12] = -d * dx;
    proj[13] = -d * dy;
    proj[14] = -d * dz;
    proj[15] = a * dx + b * dy + c * dz;
    // Shadow matrix ready
}


/////////////////////////////////////////////////////////////////////////////
// I want to know the point on a ray, closest to another given point in space.
// As a bonus, return the distance squared of the two points.
// In: vRayOrigin is the origin of the ray.
// In: vUnitRayDir is the unit vector of the ray
// In: vPointInSpace is the point in space
// Out: vPointOnRay is the poing on the ray closest to vPointInSpace
// Return: The square of the distance to the ray
// ditto above... but with floats
float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir,
        const M3DVector3f vPointInSpace)
{
    M3DVector3f v;
    m3dSubtractVectors3(v, vPointInSpace, vRayOrigin);

    float t = m3dDotProduct(vUnitRayDir, v);

    // This is the point on the ray
    vPointOnRay[0] = vRayOrigin[0] + (t * vUnitRayDir[0]);
    vPointOnRay[1] = vRayOrigin[1] + (t * vUnitRayDir[1]);
    vPointOnRay[2] = vRayOrigin[2] + (t * vUnitRayDir[2]);

    return m3dGetDistanceSquared(vPointOnRay, vPointInSpace);
}

unsigned int m3dIsPOW2(unsigned int iValue)
{
    unsigned int nPow2 = 1;

    while(iValue > nPow2)
        nPow2 = (nPow2 << 1);

    return nPow2;
}

///////////////////////////////////////////////////////////////////////////////
// Inline vector functions
// Load Vector with (x, y, z, w).
void m3dLoadVector2(M3DVector2f v, float x, float y)
{ v[0] = x; v[1] = y; }
void m3dLoadVector3(M3DVector3f v, float x, float y, float z)
{ v[0] = x; v[1] = y; v[2] = z; }
void m3dLoadVector4(M3DVector4f v, float x, float y, float z, float w)
{ v[0] = x; v[1] = y; v[2] = z; v[3] = w;}


////////////////////////////////////////////////////////////////////////////////
// Copy vector src into vector dst
void	m3dCopyVector2(M3DVector2f dst, const M3DVector2f src) { memcpy(dst, src, sizeof(M3DVector2f)); }
void	m3dCopyVector3(M3DVector3f dst, const M3DVector3f src) { memcpy(dst, src, sizeof(M3DVector3f)); }
void	m3dCopyVector4(M3DVector4f dst, const M3DVector4f src) { memcpy(dst, src, sizeof(M3DVector4f)); }


////////////////////////////////////////////////////////////////////////////////
// Add Vectors (r, a, b) r = a + b
void m3dAddVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
{ r[0] = a[0] + b[0];	r[1] = a[1] + b[1];  }
void m3dAddVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
{ r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2]; }
void m3dAddVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
{ r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2]; r[3] = a[3] + b[3]; }

////////////////////////////////////////////////////////////////////////////////
// Subtract Vectors (r, a, b) r = a - b
void m3dSubtractVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
{ r[0] = a[0] - b[0]; r[1] = a[1] - b[1];  }
void m3dSubtractVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
{ r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2]; }
void m3dSubtractVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
{ r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2]; r[3] = a[3] - b[3]; }


///////////////////////////////////////////////////////////////////////////////////////
// Scale Vectors (in place)
void m3dScaleVector2(M3DVector2f v, float scale)
{ v[0] *= scale; v[1] *= scale; }

void m3dScaleVector3(M3DVector3f v, float scale)
{ v[0] *= scale; v[1] *= scale; v[2] *= scale; }

void m3dScaleVector4(M3DVector4f v, float scale)
{ v[0] *= scale; v[1] *= scale; v[2] *= scale; v[3] *= scale; }


//////////////////////////////////////////////////////////////////////////////////////
// Cross Product
// u x v = result
// We only need one version for floats, and one version for doubles. A 3 component
// vector fits in a 4 component vector. If  M3DVector4d or M3DVector4f are passed
// we will be OK because 4th component is not used.
void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
{
    result[0] = u[1]*v[2] - v[1]*u[2];
    result[1] = -u[0]*v[2] + v[0]*u[2];
    result[2] = u[0]*v[1] - v[0]*u[1];
}

//////////////////////////////////////////////////////////////////////////////////////
// Dot Product, only for three component vectors
// return u dot v
float m3dDotProduct(const M3DVector3f u, const M3DVector3f v)
{ return u[0]*v[0] + u[1]*v[1] + u[2]*v[2]; }


//////////////////////////////////////////////////////////////////////////////////////
// Angle between vectors, only for three component vectors. Angle is in radians...
float m3dGetAngleBetweenVectors(const M3DVector3f u, const M3DVector3f v)
{
    float dTemp = m3dDotProduct(u, v);
    return acosf(dTemp);
}

//////////////////////////////////////////////////////////////////////////////////////
// Get Square of a vectors length
// Only for three component vectors
float m3dGetVectorLengthSquared(const M3DVector3f u)
{ return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]); }


//////////////////////////////////////////////////////////////////////////////////////
// Get lenght of vector
// Only for three component vectors.
float m3dGetVectorLength(const M3DVector3f u)
{ return sqrt(m3dGetVectorLengthSquared(u)); }

//////////////////////////////////////////////////////////////////////////////////////
// Normalize a vector
// Scale a vector to unit length. Easy, just scale the vector by it's length
void m3dNormalizeVector(M3DVector3f u)
{ m3dScaleVector3(u, 1.0f / m3dGetVectorLength(u)); }


float m3dGetDistance(const M3DVector3f u, const M3DVector3f v)
{ return (sqrt(m3dGetDistanceSquared(u, v))); }

float m3dGetMagnitudeSquared(const M3DVector3f u) { return u[0]*u[0] + u[1]*u[1] + u[2]*u[2]; }

float m3dGetMagnitude(const M3DVector3f u) { return (sqrt(m3dGetMagnitudeSquared(u))); }



// Matrix functions
// Both floating point and double precision 3x3 and 4x4 matricies are supported.
// No support is included for arbitrarily dimensioned matricies on purpose, since
// the 3x3 and 4x4 matrix routines are the most common for the purposes of this
// library. Matrices are column major, like OpenGL matrices.
// Unlike the vector functions, some of these are going to have to not be inlined,
// although many will be.


// Copy Matrix
// Brain-dead memcpy
void m3dCopyMatrix33(M3DMatrix33f dst, const M3DMatrix33f src)
{ memcpy(dst, src, sizeof(M3DMatrix33f)); }

void m3dCopyMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
{ memcpy(dst, src, sizeof(M3DMatrix44f)); }


/////////////////////////////////////////////////////////////////////////////
// Get/Set Column.
void m3dGetMatrixColumn33(M3DVector3f dst, const M3DMatrix33f src, int column)
{ memcpy(dst, src + (3 * column), sizeof(float) * 3); }

void m3dSetMatrixColumn33(M3DMatrix33f dst, const M3DVector3f src, int column)
{ memcpy(dst + (3 * column), src, sizeof(float) * 3); }

void m3dGetMatrixColumn44(M3DVector4f dst, const M3DMatrix44f src, int column)
{ memcpy(dst, src + (4 * column), sizeof(float) * 4); }

void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, int column)
{ memcpy(dst + (4 * column), src, sizeof(float) * 4); }


// Get/Set row purposely omitted... use the functions below.
// I don't think row vectors are useful for column major ordering...
// If I'm wrong, add them later.


//////////////////////////////////////////////////////////////////////////////
// Get/Set RowCol - Remember column major ordering...
// Provides for element addressing
void m3dSetMatrixRowCol33(M3DMatrix33f m, int row, int col, float value)
{ m[(col * 3) + row] = value; }

float m3dGetMatrixRowCol33(const M3DMatrix33f m, int row, int col)
{ return m[(col * 3) + row]; }

void m3dSetMatrixRowCol44(M3DMatrix44f m, int row, int col, float value)
{ m[(col * 4) + row] = value; }

float m3dGetMatrixRowCol44(const M3DMatrix44f m, int row, int col)
{ return m[(col * 4) + row]; }


///////////////////////////////////////////////////////////////////////////////
// Extract a rotation matrix from a 4x4 matrix
// Extracts the rotation matrix (3x3) from a 4x4 matrix
void m3dExtractRotation(M3DMatrix33f dst, const M3DMatrix44f src)
{
    memcpy(dst, src, sizeof(float) * 3); // X column
    memcpy(dst + 3, src + 4, sizeof(float) * 3); // Y column
    memcpy(dst + 6, src + 8, sizeof(float) * 3); // Z column
}

// Inject Rotation (3x3) into a full 4x4 matrix...
void m3dInjectRotation(M3DMatrix44f dst, const M3DMatrix33f src)
{
    memcpy(dst, src, sizeof(float) * 4);
    memcpy(dst + 4, src + 4, sizeof(float) * 4);
    memcpy(dst + 8, src + 8, sizeof(float) * 4);
}


// Transform - Does rotation and translation via a 4x4 matrix. Transforms
// a point or vector.
// By-the-way __means I'm asking the compiler to do a cost/benefit analysis. If
// these are used frequently, they may not be inlined to save memory. I'm experimenting
// with this....
void m3dTransformVector3(M3DVector3f vOut, const M3DVector3f v, const M3DMatrix44f m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] *  v[2] + m[12];// * v[3];
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] *  v[2] + m[13];// * v[3];
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];// * v[3];
    //vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

void m3dTransformVector4(M3DVector4f vOut, const M3DVector4f v, const M3DMatrix44f m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] *  v[2] + m[12] * v[3];
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] *  v[2] + m[13] * v[3];
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
    vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}



// Just do the rotation, not the translation... this is usually done with a 3x3
// Matrix.
void m3dRotateVector(M3DVector3f vOut, const M3DVector3f p, const M3DMatrix33f m)
{
    vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];
    vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];
    vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];
}


// Scale a matrix (I don't beleive in Scaling matricies ;-)
// Yes, it's faster to loop backwards... These could be
// unrolled... but eh... if you find this is a bottleneck,
// then you should unroll it yourself
void m3dScaleMatrix33(M3DMatrix33f m, float scale)
{ int i; for(i = 8; i >=0; i--) m[i] *= scale; }

void m3dScaleAllMatrix44(M3DMatrix44f m, float scale)
{ int i; for(i = 15; i >=0; i--) m[i] *= scale; }

// Create a Translation matrix. Only 4x4 matrices have translation components
void m3dTranslationMatrix44(M3DMatrix44f m, float x, float y, float z)
{ m3dLoadIdentity44(m); m[12] = x; m[13] = y; m[14] = z; }


// Translate matrix. Only 4x4 matrices supported
void m3dTranslateMatrix44(M3DMatrix44f m, float x, float y, float z)
{ m[12] += x; m[13] += y; m[14] += z; }


// Scale matrix. Only 4x4 matrices supported
void m3dScaleMatrix44(M3DMatrix44f m, float x, float y, float z)
{ m[0] *= x; m[5] *= y; m[10] *= z; }


// Transpose/Invert - Only 4x4 matricies supported
#define TRANSPOSE44(dst, src)            \
{                                        \
    int i, j;                            \
    for (j = 0; j < 4; j++)              \
    {                                    \
        for (i = 0; i < 4; i++)          \
        {                                \
            dst[(j*4)+i] = src[(i*4)+j]; \
        }                                \
    }                                    \
}
void m3dTransposeMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
{ TRANSPOSE44(dst, src); }

// Calculates the signed distance of a point to a plane
float m3dGetDistanceToPlane(const M3DVector3f point, const M3DVector4f plane)
{ return point[0]*plane[0] + point[1]*plane[1] + point[2]*plane[2] + plane[3]; }

//////////////////////////////////////////////////////////////////////////////////////////////////
// Compare floats and doubles...
int m3dCloseEnough(float fCandidate, float fCompare, float fEpsilon)
{
    return (fabs(fCandidate - fCompare) < fEpsilon);
}

