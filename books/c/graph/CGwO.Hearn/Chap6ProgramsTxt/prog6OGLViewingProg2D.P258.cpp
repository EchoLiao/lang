#include <GL/glut.h>

class wcPt2D {
    public:
        GLfloat x, y;
};

void init (void)
{
    /*  Set color of display window to white.  */
    glClearColor (1.0, 1.0, 1.0, 0.0);

    /*  Set parameters for world-coordinate clipping window.  */
    // [(P253)]
    // 在OpenGL中, 不能建立独立的二维观察坐标系(即: 没有提供有函数用于设置二维
    // 坐标观察坐标系, 所以我们只能在世界坐标系中设置裁剪窗口), 必须将裁剪窗口
    // 的参数作为投影变换的一部分来设置. 因些必须先选择投影模式.
    glMatrixMode (GL_PROJECTION);
    /*
     * [(P253)]
     * 设定将场景映射到屏幕的正交投影(裁剪窗口).
     * 对于二维场景, 正交投影除了将对象位置转换到规范化坐标系之外没有其他作用.
     * OpenGL裁剪函数使用 -1 到 1 的规范化坐标范围.
     * gluOrtho2D 函数的默认参数为: (-1, 1, -1, 1).
     * */
    gluOrtho2D (-50.0, 100.0, -100.0, 100.0);

    /*  Set mode for constructing geometric transformation matrix.  */
    glMatrixMode (GL_MODELVIEW);
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
    wcPt2D verts [3] = { {-50.0, -25.0}, {50.0, -25.0}, {0.0, 50.0} };

    glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.
    glPushMatrix(); {

        glColor3f (0.0, 0.0, 1.0);       //  Set fill color to blue.
        /* [(P251)]
         * 只有在裁剪窗口和视口有相同的纵横比时才能保持对象的相对比例不变.
         * */
        glViewport (0, 0, 300, 300);     //  Set left viewport.
        triangle (verts);                //  Display triangle.

        /*  Rotate triangle and display in right half of display window.  */
        glColor3f (1.0, 0.0, 0.0);         //  Set fill color to red.
        glViewport (300, 0, 300, 300);     //  Set right viewport.
        glRotatef (90.0, 0.0, 0.0, 1.0);   //  Rotate about z axis.
        triangle (verts);           //  Display red rotated triangle.
    } glPopMatrix();

    glFlush ( );
}

int main (int argc, char ** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (50, 50);
    glutInitWindowSize (600, 300);
    glutCreateWindow ("Split-Screen Example");

    init ( );
    glutDisplayFunc (displayFcn);

    glutMainLoop ( );

    return 0;
}
