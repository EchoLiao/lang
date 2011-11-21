#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

/*  Set initial display-window size. */
GLsizei winWidth = 600, winHeight = 600;

/*  Set range for world coordinates.  */
GLfloat xwcMin = -150.0, xwcMax = 150.0;
GLfloat ywcMin = -150.0, ywcMax = 150.0;


class wcPt3D {
    public:
        GLfloat x, y, z;
};
typedef float Matrix4x4 [4][4];

Matrix4x4 matRot;


void init (void)
{
    /*  Set color of display window to white.  */
    glClearColor (1.0, 1.0, 1.0, 0.0);
}

/* Construct the 4 by 4 identity matrix. */
void matrix4x4SetIdentity (Matrix4x4 matIdent4x4)
{
    GLint row, col;

    for (row = 0; row < 4; row++)
        for (col = 0; col < 4 ; col++)
            matIdent4x4 [row][col] = (row == col);
}

/* Premultiply matrix m1 times matrix m2, store result in m2. */
// m2 = m1 * m2
// 行主序.
void matrix4x4PreMultiply (Matrix4x4 m1, Matrix4x4 m2)
{
    GLint row, col;
    Matrix4x4 matTemp;

    for (row = 0; row < 4; row++)
        for (col = 0; col < 4 ; col++)
            matTemp [row][col] = m1 [row][0] * m2 [0][col] + m1 [row][1] *
                m2 [1][col] + m1 [row][2] * m2 [2][col] +
                m1 [row][3] * m2 [3][col];
    for (row = 0; row < 4; row++)
        for (col = 0; col < 4; col++)
            m2 [row][col] = matTemp [row][col];
}

void translate3D (GLfloat tx, GLfloat ty, GLfloat tz)
{
    Matrix4x4 matTransl3D;

    /*  Initialize translation matrix to identity.  */
    matrix4x4SetIdentity (matTransl3D);

    matTransl3D [0][3] = tx;
    matTransl3D [1][3] = ty;
    matTransl3D [2][3] = tz;

    /*  Concatenate translation matrix with matRot.  */
    matrix4x4PreMultiply (matTransl3D, matRot);
}

/* 
 * 3D, 绕任意轴旋转
 *
 *   Mr(theta) = T^-1 * R(theta) * T
 *
 * */
void rotate3D (wcPt3D p1, wcPt3D p2, GLfloat radianAngle)
{
    Matrix4x4 matQuaternionRot;

    GLfloat axisVectLength = sqrt ((p2.x - p1.x) * (p2.x - p1.x) +
            (p2.y - p1.y) * (p2.y - p1.y) +
            (p2.z - p1.z) * (p2.z - p1.z));
    GLfloat cosA = cos (radianAngle);
    GLfloat oneC = 1 - cosA;
    GLfloat sinA = sin (radianAngle);
    GLfloat ux = (p2.x - p1.x) / axisVectLength;
    GLfloat uy = (p2.y - p1.y) / axisVectLength;
    GLfloat uz = (p2.z - p1.z) / axisVectLength;

    /*  Set up translation matrix for moving p1 to origin.  */
    translate3D (-p1.x, -p1.y, -p1.z);

    /*  Initialize matQuaternionRot to identity matrix.  */
    matrix4x4SetIdentity (matQuaternionRot);

    matQuaternionRot [0][0] = ux*ux*oneC + cosA;
    matQuaternionRot [0][1] = ux*uy*oneC - uz*sinA;
    matQuaternionRot [0][2] = ux*uz*oneC + uy*sinA;
    matQuaternionRot [1][0] = uy*ux*oneC + uz*sinA;
    matQuaternionRot [1][1] = uy*uy*oneC + cosA;
    matQuaternionRot [1][2] = uy*uz*oneC - ux*sinA;
    matQuaternionRot [2][0] = uz*ux*oneC - uy*sinA;
    matQuaternionRot [2][1] = uz*uy*oneC + ux*sinA;
    matQuaternionRot [2][2] = uz*uz*oneC + cosA;

    /*  Combine matQuaternionRot with translation matrix.  */
    matrix4x4PreMultiply (matQuaternionRot, matRot);

    /*  Set up inverse matTransl3D and concatenate with
     *  product of previous two matrices.
     */
    translate3D (p1.x, p1.y, p1.z);
}

void transformVerts3D (GLint nVerts, wcPt3D * verts)
{
    GLint k;
    GLfloat tempX, tempY;

    for (k = 0; k < nVerts; k++) {
        tempX = matRot [0][0] * verts [k].x +
            matRot [0][1] * verts [k].y +
            matRot [0][2] * verts [k].z +
            matRot [0][3];
        tempY = matRot [1][0] * verts [k].x +
            matRot [1][1] * verts [k].y +
            matRot [1][2] * verts [k].z +
            matRot [1][3];
        verts [k].z = matRot [2][0] * verts [k].x +
            matRot [2][1] * verts [k].y +
            matRot [2][2] * verts [k].z +
            matRot [2][3];
        verts [k].x = tempX;
        verts [k].y = tempY;
    }
}

void triangle (wcPt3D *verts)
{
    GLint k;

    glBegin (GL_TRIANGLES);
    for (k = 0; k < 3; k++)
        glVertex3f (verts[k].x, verts[k].y, verts[k].z);
    glEnd ( );
}

void displayFcn (void)
{
    /*  Input rotation parameters.  */
    GLint nVerts = 3;
    wcPt3D verts[3] = {
        {-100.0, 0.0, 0.0},
        { 100.0, 0.0, 0.0},
        { 0.0, 100.0, 0.0},
    };

    glColor3f (0.0, 0.0, 1.0);      //  Set initial fill color to blue.
    triangle (verts);               //  Display blue triangle.

    /*  Initialize matRot to identity matrix:  */
    matrix4x4SetIdentity (matRot);

    /*  Pass rotation parameters to procedure rotate3D.  */
    // N = Np2 - Np1
    wcPt3D Np1 = {1.0, 0.0, 0.0};
    wcPt3D Np2 = {0.0, 0.0, 0.0};
    rotate3D(Np1, Np2, 60 * (3.1415926 / 180.0));

    /*  Display rotated object.  */
    transformVerts3D(nVerts, &verts[0]);
    glColor3f (1.0, 0.0, 0.0);      //  Set initial fill color to blue.
    triangle (verts);               //  Display blue triangle.

    glFlush();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    glOrtho(xwcMin, xwcMax, ywcMin, ywcMax, 10, 600);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ( );
    glTranslatef(0.0, 0.0, -10.0);

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
