// Header file for the Math3d library. The C-Runtime has math.h, this file and the
// accompanying math.c are meant to suppliment math.h by adding geometry/math routines
// useful for graphics, simulation, and physics applications (3D stuff).

#ifndef _MATH3D_LIBRARY__
#define _MATH3D_LIBRARY__

#include <math.h>
#include <memory.h>

///////////////////////////////////////////////////////////////////////////////
// Data structures and containers
// Much thought went into how these are declared. Many libraries declare these
// as structures with x, y, z data members. However structure alignment issues
// could limit the portability of code based on such structures, or the binary
// compatibility of data files (more likely) that contain such structures across
// compilers/platforms. Arrays are always tightly packed, and are more efficient
// for moving blocks of data around (usually).
typedef float M3DVector3f[3];		// Vector of three floats (x, y, z)
typedef float M3DVector4f[4];		// Lesser used... Do we really need these?
typedef float M3DVector2f[2];		// 3D points = 3D Vectors, but we need a

// 3x3 matrix - column major. X vector is 0, 1, 2, etc.
//		0	3	6
//		1	4	7
//		2	5	8
typedef float M3DMatrix33f[9];		// A 3 x 3 matrix, column major (floats) - OpenGL Style

// 4x4 matrix - column major. X vector is 0, 1, 2, etc.
//	0	4	8	12
//	1	5	9	13
//	2	6	10	14
//	3	7	11	15
typedef float M3DMatrix44f[16];		// A 4 X 4 matrix, column major (floats) - OpenGL style


///////////////////////////////////////////////////////////////////////////////
// Useful constants
#define M3D_PI (3.14159265358979323846)
#define M3D_2PI (2.0 * M3D_PI)
#define M3D_PI_DIV_180 (0.017453292519943296)
#define M3D_INV_PI_DIV_180 (57.2957795130823229)


///////////////////////////////////////////////////////////////////////////////
// Useful shortcuts and macros
// Radians are king... but we need a way to swap back and forth
#define m3dDegToRad(x)	((x)*M3D_PI_DIV_180)
#define m3dRadToDeg(x)	((x)*M3D_INV_PI_DIV_180)

// Hour angles
#define m3dHrToDeg(x)	((x) * (1.0 / 15.0))
#define m3dHrToRad(x)	m3dDegToRad(m3dHrToDeg(x))

#define m3dDegToHr(x)	((x) * 15.0)
#define m3dRadToHr(x)	m3dDegToHr(m3dRadToDeg(x))


// Returns the same number if it is a power of
// two. Returns a larger integer if it is not a
// power of two. The larger integer is the next
// highest power of two.
unsigned int m3dIsPOW2(unsigned int iValue);

///////////////////////////////////////////////////////////////////////////////
// Inline accessor functions for people who just can't count to 3 - Vectors
#define	m3dGetVectorX(v) (v[0])
#define m3dGetVectorY(v) (v[1])
#define m3dGetVectorZ(v) (v[2])
#define m3dGetVectorW(v) (v[3])

#define m3dSetVectorX(v, x)	((v)[0] = (x))
#define m3dSetVectorY(v, y)	((v)[1] = (y))
#define m3dSetVectorZ(v, z)	((v)[2] = (z))
#define m3dSetVectorW(v, w)	((v)[3] = (w))

///////////////////////////////////////////////////////////////////////////////
// Inline vector functions
// Load Vector with (x, y, z, w).
void m3dLoadVector2(M3DVector2f v, float x, float y);
void m3dLoadVector3(M3DVector3f v, float x, float y, float z);
void m3dLoadVector4(M3DVector4f v, float x, float y, float z, float w);


////////////////////////////////////////////////////////////////////////////////
// Copy vector src into vector dst
void	m3dCopyVector2(M3DVector2f dst, const M3DVector2f src);
void	m3dCopyVector3(M3DVector3f dst, const M3DVector3f src);
void	m3dCopyVector4(M3DVector4f dst, const M3DVector4f src);


////////////////////////////////////////////////////////////////////////////////
// Add Vectors (r, a, b) r = a + b
void m3dAddVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b);
void m3dAddVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b);
void m3dAddVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b);

////////////////////////////////////////////////////////////////////////////////
// Subtract Vectors (r, a, b) r = a - b
void m3dSubtractVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b);
void m3dSubtractVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b);
void m3dSubtractVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b);


///////////////////////////////////////////////////////////////////////////////////////
// Scale Vectors (in place)
void m3dScaleVector2(M3DVector2f v, float scale);
void m3dScaleVector3(M3DVector3f v, float scale);
void m3dScaleVector4(M3DVector4f v, float scale);

//////////////////////////////////////////////////////////////////////////////////////
// Cross Product
// u x v = result
// We only need one version for floats, and one version for doubles. A 3 component
// vector fits in a 4 component vector. If  M3DVector4d or M3DVector4f are passed
// we will be OK because 4th component is not used.
void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v);

//////////////////////////////////////////////////////////////////////////////////////
// Dot Product, only for three component vectors
// return u dot v
float m3dDotProduct(const M3DVector3f u, const M3DVector3f v);


//////////////////////////////////////////////////////////////////////////////////////
// Angle between vectors, only for three component vectors. Angle is in radians...
float m3dGetAngleBetweenVectors(const M3DVector3f u, const M3DVector3f v);

//////////////////////////////////////////////////////////////////////////////////////
// Get Square of a vectors length
// Only for three component vectors
float m3dGetVectorLengthSquared(const M3DVector3f u);


//////////////////////////////////////////////////////////////////////////////////////
// Get lenght of vector
// Only for three component vectors.
float m3dGetVectorLength(const M3DVector3f u);

//////////////////////////////////////////////////////////////////////////////////////
// Normalize a vector
// Scale a vector to unit length. Easy, just scale the vector by it's length
void m3dNormalizeVector(M3DVector3f u);

//////////////////////////////////////////////////////////////////////////////////////
// Get the distance between two points. The distance between two points is just
// the magnitude of the difference between two vectors
// Located in math.cpp
float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v);


float m3dGetDistance(const M3DVector3f u, const M3DVector3f v);

float m3dGetMagnitudeSquared(const M3DVector3f u);

float m3dGetMagnitude(const M3DVector3f u);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix functions
// Both floating point and double precision 3x3 and 4x4 matricies are supported.
// No support is included for arbitrarily dimensioned matricies on purpose, since
// the 3x3 and 4x4 matrix routines are the most common for the purposes of this
// library. Matrices are column major, like OpenGL matrices.
// Unlike the vector functions, some of these are going to have to not be inlined,
// although many will be.


// Copy Matrix
// Brain-dead memcpy
void m3dCopyMatrix33(M3DMatrix33f dst, const M3DMatrix33f src);
void m3dCopyMatrix44(M3DMatrix44f dst, const M3DMatrix44f src);


// LoadIdentity
// Implemented in Math3d.cpp
void m3dLoadIdentity33(M3DMatrix33f m);
void m3dLoadIdentity44(M3DMatrix44f m);

/////////////////////////////////////////////////////////////////////////////
// Get/Set Column.
void m3dGetMatrixColumn33(M3DVector3f dst, const M3DMatrix33f src, int column);
void m3dSetMatrixColumn33(M3DMatrix33f dst, const M3DVector3f src, int column);
void m3dGetMatrixColumn44(M3DVector4f dst, const M3DMatrix44f src, int column);
void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, int column);

// Get/Set row purposely omitted... use the functions below.
// I don't think row vectors are useful for column major ordering...
// If I'm wrong, add them later.


//////////////////////////////////////////////////////////////////////////////
// Get/Set RowCol - Remember column major ordering...
// Provides for element addressing
void m3dSetMatrixRowCol33(M3DMatrix33f m, int row, int col, float value);
float m3dGetMatrixRowCol33(const M3DMatrix33f m, int row, int col);
void m3dSetMatrixRowCol44(M3DMatrix44f m, int row, int col, float value);
float m3dGetMatrixRowCol44(const M3DMatrix44f m, int row, int col);

///////////////////////////////////////////////////////////////////////////////
// Extract a rotation matrix from a 4x4 matrix
// Extracts the rotation matrix (3x3) from a 4x4 matrix
void m3dExtractRotation(M3DMatrix33f dst, const M3DMatrix44f src);

// Inject Rotation (3x3) into a full 4x4 matrix...
void m3dInjectRotation(M3DMatrix44f dst, const M3DMatrix33f src);


////////////////////////////////////////////////////////////////////////////////
// MultMatrix
// Implemented in Math.cpp
void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b);
void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b);


// Transform - Does rotation and translation via a 4x4 matrix. Transforms
// a point or vector.
// By-the-way __means I'm asking the compiler to do a cost/benefit analysis. If
// these are used frequently, they may not be inlined to save memory. I'm experimenting
// with this....
void m3dTransformVector3(M3DVector3f vOut, const M3DVector3f v, const M3DMatrix44f m);
void m3dTransformVector4(M3DVector4f vOut, const M3DVector4f v, const M3DMatrix44f m);


// Just do the rotation, not the translation... this is usually done with a 3x3
// Matrix.
void m3dRotateVector(M3DVector3f vOut, const M3DVector3f p, const M3DMatrix33f m);


// Scale a matrix (I don't beleive in Scaling matricies ;-)
// Yes, it's faster to loop backwards... These could be
// unrolled... but eh... if you find this is a bottleneck,
// then you should unroll it yourself
void m3dScaleMatrix33(M3DMatrix33f m, float scale);
void m3dScaleAllMatrix44(M3DMatrix44f m, float scale);

// Create a Rotation matrix
// Implemented in math.cpp
void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z);
void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z);

// Create a Translation matrix. Only 4x4 matrices have translation components
void m3dTranslationMatrix44(M3DMatrix44f m, float x, float y, float z);

// Translate matrix. Only 4x4 matrices supported
void m3dTranslateMatrix44(M3DMatrix44f m, float x, float y, float z);

// Scale matrix. Only 4x4 matrices supported
void m3dScaleMatrix44(M3DMatrix44f m, float x, float y, float z);

void m3dTransposeMatrix44(M3DMatrix44f dst, const M3DMatrix44f src);
int m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Other Miscellaneous functions

// Find a normal from three points
// Implemented in math3d.cpp
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
        const M3DVector3f point3);

// Calculates the signed distance of a point to a plane
float m3dGetDistanceToPlane(const M3DVector3f point, const M3DVector4f plane);

// Get plane equation from three points and a normal
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3);

// Determine if a ray intersects a sphere
float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius);

// Etc. etc.

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Faster (and more robust) replacements for gluProject
/* void m3dProjectXY( M3DVector2f vPointOut, const M3DMatrix44f mModelView, const
        M3DMatrix44f mProjection, const int iViewPort[4],
        const M3DVector2f vPointIn);
void m3dProjectXYZ(M3DVector3f vPointOut, const M3DMatrix44f mModelView, const
        M3DMatrix44f mProjection, const int iViewPort[4],
        const M3DVector3f vPointIn); */
void m3dProjectXY(const M3DMatrix44f mModelView, const M3DMatrix44f
        mProjection, const int iViewPort[4], const M3DVector3f vPointIn,
        M3DVector3f vPointOut);
void m3dProjectXYZ(const M3DMatrix44f mModelView, const M3DMatrix44f
        mProjection, const int iViewPort[4], const M3DVector3f vPointIn,
        M3DVector3f vPointOut);


//////////////////////////////////////////////////////////////////////////////////////////////////
// This function does a three dimensional Catmull-Rom "spline" interpolation between p1 and p2
void m3dCatmullRom3(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Compare floats and doubles...
int m3dCloseEnough(float fCandidate, float fCompare, float fEpsilon);

////////////////////////////////////////////////////////////////////////////
// Used for normal mapping. Finds the tangent bases for a triangle...
// Only a floating point implementation is provided.
void m3dCalculateTangentBasis(const M3DVector3f pvTriangle[3], const M3DVector2f pvTexCoords[3], const M3DVector3f N, M3DVector3f vTangent);

////////////////////////////////////////////////////////////////////////////
// Smoothly step between 0 and 1 between edge1 and edge 2
float m3dSmoothStep(float edge1, float edge2, float x);

/////////////////////////////////////////////////////////////////////////////
// Planar shadow Matrix
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos);

float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir,
        const M3DVector3f vPointInSpace);

#endif

