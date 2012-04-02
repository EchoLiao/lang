/*
 * ===========================================================================
 *
 *       Filename:  Nehe.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年02月23日 21时00分27秒
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

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/glut.h>

#include "Texture.h"
#include "Vector3D.h"

typedef struct N3D_Vertex
{
	GLfloat fX;
	GLfloat fY;
	GLfloat fZ;
	GLfloat fS;
	GLfloat fT;
	GLfloat fNX;
	GLfloat fNY;
	GLfloat fNZ;
} N3D_Vertex;

typedef struct N3D_Cone
{
    N3D_Vertex  *pVx;
    GLfloat     fHeight;
    GLfloat     fRadius;
    GLint       iStacks;
} N3D_Cone;



GLint  ConeCreate(N3D_Cone *pCone);
GLvoid ConeDraw(N3D_Cone *pCone);


#define LNUM    10

GLuint  pyramidTex;
GLfloat tex_y;
BOOL	Pause = FALSE;
static float gRotateY = 0.0;
static N3D_Cone sCone = { NULL, 0.8, 0.13, 9 };


// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
    // 在此处初始化绘制场景
    glClearColor(0.2f, 0.2f, 0.2f, 0.5f);							// 清屏为黑色
    glClearDepth(1.0f);												// 设置深度缓存
    glDepthFunc(GL_LEQUAL);											// 选择深度测试方式
    glEnable(GL_DEPTH_TEST);										// 开启深度测试
    glShadeModel(GL_SMOOTH);										// 阴暗处理采用平滑方式
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// 最精细的透视计算

    BuildTexture((char*)"Data/Reflection.bmp", pyramidTex);


    int ret;
    ret = ConeCreate(&sCone);
    assert(ret != 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    return TRUE;
}

#if 0
void DrawCone(float h, float r, int s)
{
    assert(s > 0);

    int i;
    float texs, text;
    float vx, vy, vz;
    float theta = 3.141592653 * 2 / s;

    glBegin(GL_TRIANGLE_FAN); {
        for ( i = 0; i < s + 1 + 1; i++ )
        {
            if ( i == 0 )
            {
                texs = 0.0;
                text = 1.0;
                vx = vy = vz = 0.0;
            }
            else
            {
                if ( i % 2 == 0 )
                {
                    texs = 0.0;
                    text = 0.0;
                }
                else
                {
                    texs = 1.0;
                    text = 0.0;
                }
                vx = r * cosf(theta * i);
                vy = h;
                vz = r * sinf(theta * i);
            }
            glTexCoord2f(texs, text);
            glVertex3f(vx, vy, vz);
        }
    } glEnd();
}
#else
GLint ConeCreate(N3D_Cone *pCone)
{
    assert(pCone != NULL && pCone->iStacks > 0 && pCone->pVx == NULL);

    GLint           i = 0;
    GLfloat         texs, text;
    GLfloat         vx, vy, vz;
    GLfloat         theta = 3.141592653 * 2 / pCone->iStacks;
    GLfloat         tCen = 1.0 / pCone->iStacks;

    pCone->pVx = (N3D_Vertex *) calloc(pCone->iStacks + 1 + 1,
            sizeof(pCone->pVx[0]));
    if ( pCone->pVx == NULL )
        return 0;

    pCone->pVx[0].fX = 0.0;
    pCone->pVx[0].fY = 0.0;
    pCone->pVx[0].fZ = 0.0;
    pCone->pVx[0].fS = 0.5;
    pCone->pVx[0].fT = 0.0;
    for ( i = 0; i < pCone->iStacks; i++ )
    {
            vx = pCone->fRadius * cosf(theta * i);
            vy = pCone->fHeight;
            vz = pCone->fRadius * sinf(theta * i);
            texs = tCen * i;
            text = 1.0;
#if 0 // NALD
        printf("NALL &&& i=%d, xyz(%f, %f, %f); st(%f,%f); [%s:%d]\n", i,
                pCone->pVx[i].fX, pCone->pVx[i].fY, pCone->pVx[i].fZ,
                pCone->pVx[i].fS, pCone->pVx[i].fT,
                __FILE__, __LINE__);
        getchar();
#endif
        // glTexCoord2f(texs, text);
        // glVertex3f(vx, vy, vz);
        pCone->pVx[i + 1].fX = vx;
        pCone->pVx[i + 1].fY = vy;
        pCone->pVx[i + 1].fZ = vz;
        pCone->pVx[i + 1].fS = texs;
        pCone->pVx[i + 1].fT = text;
    }
    pCone->pVx[i + 1].fX = pCone->pVx[1].fX;
    pCone->pVx[i + 1].fY = pCone->pVx[1].fY;
    pCone->pVx[i + 1].fZ = pCone->pVx[1].fZ;
    pCone->pVx[i + 1].fS = 1.0;
    pCone->pVx[i + 1].fT = pCone->pVx[1].fT;

    assert(i + 1 + 1 == pCone->iStacks + 1 + 1);

    return 1;
}

GLvoid ConeDraw(N3D_Cone *pCone)
{
    assert(pCone != NULL && pCone->pVx != NULL);

    glVertexPointer(3, GL_FLOAT, sizeof(N3D_Vertex), &(pCone->pVx[0].fX));
    glTexCoordPointer(2, GL_FLOAT, sizeof(N3D_Vertex), &(pCone->pVx[0].fS));
    // glNormalPointer(GL_FLOAT, sizeof(N3D_Vertex), &(pCone->pVx[0].fNX));

    glDrawArrays(GL_TRIANGLE_FAN, 0, pCone->iStacks + 2);
}

#endif

void RenderRect(void)
{
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.15,  0.0,  0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f( 0.15,  0.0,  0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f( 0.15,  1.0,  0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.15,  1.0,  0.0);
    glEnd();
}

void RenderWorld(void)
{
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE );
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, pyramidTex);

    // 在纹理矩阵中移动纹理
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    // glTranslatef(0.0f, -tex_y, 0.0f);
    // glScalef(0.0f, tex_y, 0.0f);

    glMatrixMode(GL_MODELVIEW);
#if 0
    for (int i=0; i<1; i++)
    {
        glPushMatrix();
#if 0
        glColor3f(0.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN); {
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 1.1f, 0.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 1.1f, 0.1f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, 1.1f,-0.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, 1.1f,-0.1f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 1.1f, 0.1f);
        } glEnd();

        glScalef(-1.0, -1.0, -1.0);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN); {
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 1.1f, 0.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 1.1f, 0.1f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, 1.1f,-0.1f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, 1.1f,-0.1f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 1.1f, 0.1f);
        } glEnd();
#else
        glColor3f(0.0f, 1.0f, 1.0f);
        // DrawCone(1.1, 0.13, 4);

        glScalef(-1.0, -1.0, -1.0);
        glColor3f(1.0f, 1.0f, 0.0f);
        // DrawCone(1.1, 0.13, 4);
#endif
        glPopMatrix();
    }
#else
#if 0
    //glDisable(GL_TEXTURE_2D);
    // glColor3f(0.0f, 1.0f, 1.0f);
    // ConeDraw(&sCone);

    // glScalef(-1.0, -1.0, -1.0);
    // // glColor3f(1.0f, 1.0f, 0.0f);
    // glColor3f(1.0f, 1.0f, 1.0f);
    ConeDraw(&sCone);
#else
    // glDisable(GL_TEXTURE_2D);
    // glDisable(GL_BLEND);
    RenderRect();
#endif
#endif

    // 重置纹理矩阵
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    tex_y += 0.001f;
}


// Our Rendering Is Done Here
void render(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if ( ! Pause )
    {
        gRotateY += 1.5;
        if ( gRotateY > 360.0 )
            gRotateY = 0.0;
    }

    glLoadIdentity();

    // 画出最开始的坐标系的原点
    glPushMatrix(); {
        glTranslatef(0.0, -0.0, -2.0);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glRectf(-0.01, 0.01, 0.01, -0.01);
    } glPopMatrix();


    float fRCen = 360.0 / (LNUM / 2);
    float fR;
    float fcColor[][4] = {
        {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
        {1.0, 1.0, 0.0, 1.0}, {1.0, 0.0, 1.0, 1.0},
        {0.0, 1.0, 1.0, 1.0}, {1.0, 0.5, 0.0, 1.0},
        {1.0, 0.0, 0.5, 1.0}, {0.0, 1.0, 0.5, 1.0},
        // {0.0, 0.0, 0.5, 1.0}, {0.5, 0.5, 0.0, 1.0},
    };
    extern int dummy[ LNUM % 2 == 0 ? 1 : -1 ];
    extern int dummy[ sizeof(fcColor) / sizeof(fcColor[0]) == LNUM ? 1 : -1 ];

    glTranslatef(0.0, -0.4, -2.0);
    glPushMatrix(); {
        for ( int k = 0; k < 1; k++ )
        {
            for ( int i = 0; i < LNUM; i++ )
            {
                glPushMatrix(); {
                    fR = gRotateY + (i / 2) * fRCen;
                    glRotatef(-fR, 0.0, 1.0, 0.0);
                    glRotatef(70.0, 1.0, 0, 0);
                    glRotatef(fR, 0.0, 1.0, 0.0);

                    if ( i % 2 == 1 )
                        glScalef(-1.0, -1.0, -1.0);
                    glColor3fv(fcColor[i]);
                    RenderWorld();
                } glPopMatrix();
            }
        }
    } glPopMatrix();

    glFlush ();
    glutSwapBuffers();
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    if (h == 0) h = 1;

    glFrustum(-(float)w/h/2.0, (float)w/h/2.0, -0.5, 0.5, 1.0, 100);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix

    glLoadIdentity(); // Reset The Modelview Matrix
    // glTranslatef(0.0, -10, -10.0);
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:        // When Escape Is Pressed...
            exit(0);    // Exit The Program
            break;          // Ready For Next Case
        case ' ':
            Pause = ! Pause;
            break;
        default:
            break;
    }
}

// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void special_keys(int a_keys, int x, int y)
{
    switch (a_keys) {
        case GLUT_KEY_F1:
            // // We Can Switch Between Windowed Mode And Fullscreen Mode Only
            // if (!g_gamemode) {
            //     g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
            //     if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
            //     else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
            // }
            break;
        default:
            break;
    }
}


// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)
    glutInitWindowSize(342*2, 256*2);                // Window Size If We Start In Windowed Mode
    glutCreateWindow("NeHe's OpenGL Framework"); // Window Title
    if (!init()) {                                   // Our Initialization
        return -1;
    }
    glutDisplayFunc(render);                     // Register The Display Function
    glutReshapeFunc(reshape);                    // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
    glutSpecialFunc(special_keys);               // Register Special Keys Handler
    glutIdleFunc(render);                        // We Do Rendering In Idle Time
    glutMainLoop();                              // Go To GLUT Main Loop
    return 0;
}

