#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

/*  Set initial display-window size. */
GLsizei winWidth = 600, winHeight = 600;

/*  Set range for world coordinates.  */
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;

class wcPt2D {
    public:
        GLfloat x, y;
};

typedef GLfloat Matrix3x3 [3][3];

Matrix3x3 matComposite;

const GLdouble pi = 3.14159;

void init (void)
{
    /*  Set color of display window to white.  */
    glClearColor (1.0, 1.0, 1.0, 0.0);
}

/* Construct the 3 by 3 identity matrix. */
void matrix3x3SetIdentity (Matrix3x3 matIdent3x3)
{
    GLint row, col;

    for (row = 0; row < 3; row++)
        for (col = 0; col < 3; col++)
            matIdent3x3 [row][col] = (row == col);
}

/* Premultiply matrix m1 times matrix m2, store result in m2. */
/* m2 = m1 * m2 */
void matrix3x3PreMultiply (Matrix3x3 m1, Matrix3x3 m2)
{
    GLint row, col;
    Matrix3x3 matTemp;

    for (row = 0; row < 3; row++)
        for (col = 0; col < 3 ; col++)
            matTemp [row][col] = m1 [row][0] * m2 [0][col] + m1 [row][1] *
                m2 [1][col] + m1 [row][2] * m2 [2][col];

    for (row = 0; row < 3; row++)
        for (col = 0; col < 3; col++)
            m2 [row][col] = matTemp [row][col];
}

void translate2D (GLfloat tx, GLfloat ty)
{
    Matrix3x3 matTransl;

    /*  Initialize translation matrix to identity.  */
    matrix3x3SetIdentity (matTransl);

    matTransl [0][2] = tx;
    matTransl [1][2] = ty;

    /*  Concatenate matTransl with the composite matrix.  */
    matrix3x3PreMultiply (matTransl, matComposite);
}

void rotate2D (wcPt2D pivotPt, GLfloat theta)
{
    Matrix3x3 matRot;

    /*  Initialize rotation matrix to identity.  */
    matrix3x3SetIdentity (matRot);

    matRot [0][0] = cos (theta);
    matRot [0][1] = -sin (theta);
    matRot [0][2] = pivotPt.x * (1 - cos (theta)) +
        pivotPt.y  * sin (theta);
    matRot [1][0] = sin (theta);
    matRot [1][1] = cos (theta);
    matRot [1][2] = pivotPt.y * (1 - cos (theta)) -
        pivotPt.x  * sin (theta);

    /*  Concatenate matRot with the composite matrix.  */
    matrix3x3PreMultiply (matRot, matComposite);
}

void scale2D (GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
    Matrix3x3 matScale;

    /*  Initialize scaling matrix to identity.  */
    matrix3x3SetIdentity (matScale);

    matScale [0][0] = sx;
    matScale [0][2] = (1 - sx) * fixedPt.x;
    matScale [1][1] = sy;
    matScale [1][2] = (1 - sy) * fixedPt.y;

    /*  Concatenate matScale with the composite matrix.  */
    matrix3x3PreMultiply (matScale, matComposite);
}

/* Using the composite matrix, calculate transformed coordinates. */
//
//               |A00  A01  A02|   |x|   |A00 * x + A01 * y + A02|
// V' = M * V =  |A10  A11  A12| * |y| = |A10 * x + A11 * y + A12|
//               |A20  A21  A22|   |1|   |          1            |
//
void transformVerts2D (GLint nVerts, wcPt2D * verts)
{
    GLint k;
    GLfloat temp;

    for (k = 0; k < nVerts; k++) {
        temp = matComposite [0][0] * verts [k].x + matComposite [0][1] *
            verts [k].y + matComposite [0][2];
        verts [k].y = matComposite [1][0] * verts [k].x + matComposite [1][1] *
            verts [k].y + matComposite [1][2];
        verts [k].x = temp;
    }
}

void triangle (wcPt2D *verts)
{
    GLint k;

    glBegin (GL_TRIANGLES);
    for (k = 0; k < 3; k++)
        glVertex2f (verts [k].x, verts [k].y);
    glEnd ( );
}

void displayFcn (void)
{
    /*  Define initial position for triangle.  */
    GLint nVerts = 3;
    wcPt2D verts [3] = { {50.0, 25.0}, {150.0, 25.0}, {100.0, 100.0} };

    /*  Calculate position of triangle centroid.  */
    wcPt2D centroidPt;

    GLint k, xSum = 0, ySum = 0;
    for (k = 0; k < nVerts;  k++) {
        xSum += verts [k].x;
        ySum += verts [k].y;
    }
    centroidPt.x = GLfloat (xSum) / GLfloat (nVerts);
    centroidPt.y = GLfloat (ySum) / GLfloat (nVerts);

    /*  Set geometric transformation parameters.  */
    wcPt2D pivPt, fixedPt;
    pivPt = centroidPt;
    fixedPt = centroidPt;

    GLfloat tx = 0.0, ty = 100.0;
    GLfloat sx = 0.5, sy = 0.5;
    GLdouble theta = pi/2.0;

    glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

    glColor3f (0.0, 0.0, 1.0);      //  Set initial fill color to blue.
    triangle (verts);               //  Display blue triangle.

    /*  Initialize composite matrix to identity.  */
    matrix3x3SetIdentity (matComposite);

    /*  Construct composite matrix for transformation sequence.  */
    //
    // M' = (Mt * (Mr * (Ms * M)))  // (B)
    //    = (((Mt * Mr) * Ms) * M)  // (A)
    //
    // 按 (A) 或 (B) 的方式去理解都可以:
    //     A. 按局部坐标系去理解. 每一次变换(矩阵相乘)都会改变该局部坐标系, 因
    //        此无论之前发生了什么变换, 到画图渲染时, 都相对于该局部坐标系, 且
    //        物体永远在该局部坐标系的原点. 比如: 不论之前发生了什么变换, 若当
    //        前变换是旋转, 则该旋转的"基准点"是该局部坐标系相对点, 所以旋转的
    //        结果是绕该局部坐标系相对点进行旋转.
    //     B. 按全局坐标系去理解. 每一次变换都会改变物体在该全局坐标系中坐标,
    //        因此无论之前发生了什么变换, 到画图渲染时, 都相对于该全局坐标系,
    //        且物体的坐标是不断地改变的. 比如: 如果之前发生了移动变换, 而当前
    //        的变换是旋转, 但该旋转的"基准点"的坐标在该全局坐标系中并没有改变
    //        , 所以旋转的结果是以该基准点为圆心, 以物体的当前位置到该基准点的
    //        距离为半径画弧. 数学上, 一般是以该种方式进行推导的.
    //
    // 这与OpenGL的是不一样的. OpenGL的旋转和缩放变换的基准点是只能是原点, 但
    // 这里的可以把任意点作为基准点; 且这里的变换操作是把变换矩阵左乘当前矩阵,
    // 而OpenGL是把变换矩阵右乘当前矩阵.
    //
    scale2D (sx, sy, fixedPt);   //  First transformation: Scale.
    rotate2D (pivPt, theta);     //  Second transformation: Rotate
    translate2D (tx, ty);        //  Final transformation: Translate.

    /*  Apply composite matrix to triangle vertices.  */
    //
    // P' = M' * P
    //
    transformVerts2D (nVerts, verts);

    glColor3f (1.0, 0.0, 0.0);  // Set color for transformed triangle.
    triangle (verts);           // Display red transformed triangle.

    glFlush ( );
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

    glClear (GL_COLOR_BUFFER_BIT);
}


int main (int argc, char ** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (50, 50);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("Geometric Transformation Sequence");

    init ( );
    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);

    glutMainLoop ( );

    return 0;
}
