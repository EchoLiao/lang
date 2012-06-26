/*
 * ===========================================================================
 *
 *       Filename:  3drace.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年06月26日 22时47分00秒
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

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>


typedef struct N3D_Vertex
{
    float fX;
    float fY;
    float fZ;

    float fS;
    float fT;

    float fR;
    float fG;
    float fB;
    float fA;

    float fNX;
    float fNY;
    float fNZ;
} N3D_Vertex;

typedef struct _RGBIMG {
    GLuint   w;
    GLuint   h;
    GLubyte* data;
} RGBIMG;



#define TEXTURES_NUM 1

int     g_gamemode;
int     g_fullscreen;
GLuint	g_texid[TEXTURES_NUM];



int  load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
{
#if 0
    GLuint   sz;    // Our Image's Data Field Length In Bytes
    FILE*    file;  // The Image's File On Disk
    long     fsize; // File Size In Bytes
    GLubyte* p;     // Helper Pointer

    // Update The Image's Fields
    refimg->w = (GLuint) w;
    refimg->h = (GLuint) h;
    // 所加载的是24位的位图; 使每一行都以4字节对齐, 以加载读取速度.
    sz = (((3*refimg->w+3)>>2)<<2)*refimg->h;
    refimg->data = new GLubyte [sz];
    if (refimg->data == NULL) return 0;

    // Open The File And Read The Pixels
    file = fopen(file_name , "rb");
    if (!file) return 0;
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    if (fsize != (long)sz) {
        fclose(file);
        return 0;
    }
    fseek(file, 0L, SEEK_SET);
    p = refimg->data;
    while (fsize > 0) {
        fread(p, 1, 1, file);
        p++;
        fsize--;
    }
    fclose(file);
    return 1;
#else
    /* refimg->w = (GLuint) w;
    refimg->h = (GLuint) h;
    refimg->data = new GLubyte [256*256*3];
    if (refimg->data == NULL)
        return 0;

    GLubyte *pb = refimg->data;
    for ( int y = 0; y < 256; y++ )
    {
        for ( int x = 0; x < 256; x++ )
        {
            if ( x == y )
            {
                *pb = 255;
                *(pb+1) = 0;
                *(pb+2) = 0;
            }
            else
            {
                *pb = 0;
                *(pb+1) = 255;
                *(pb+2) = 0;
            }
            pb += 3;
        }
    } */
    return 1;
#endif
}

int  setup_textures()
{
    // RGBIMG img;

    glGenTextures(TEXTURES_NUM, g_texid);
    // if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return 0;

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // delete img.data;

    return 1;
}

int  init(void)
{
    if (!setup_textures()) return 0;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    return 1;
}

static void drawRect()
{
    glRectf(-0.01, -0.01, 0.01, 0.01);
    glRectf(-1.0, -1.0, 1.0, 1.0);
}

// Our Rendering Is Done Here
void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-2.0f);

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);

#if 1
    glPushMatrix(); {
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4f(1.0, 0.0, 1.0, 1.0);
        glScalef(0.5, 0.5, 0.5);
        glRectf(-0.01, -0.01, 0.01, 0.01);
        glRectf(-1.0, -1.0, 1.0, 1.0);
    } glPopMatrix();
#else
#endif

    usleep(1000 * 200);

    glutSwapBuffers();
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (h == 0) h = 1;

    glFrustum(-(float)w/h/2.0, (float)w/h/2.0, -0.5, 0.5, 1.0, 100);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case ' ':
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void special_keys(int a_keys, int x, int y)
{
    switch (a_keys) {
        case GLUT_KEY_F1:
            if (!g_gamemode) {
                g_fullscreen = !g_fullscreen;
                if (g_fullscreen) glutFullScreen();
                else glutReshapeWindow(500, 500);
            }
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    if (g_gamemode) {
        glutGameModeString("640x480:16");
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();
        else g_gamemode = 0;
    }
    if (!g_gamemode) {
        glutInitWindowSize(342*2, 256*2);
        glutCreateWindow("NeHe's OpenGL Framework");
    }
    if (!init()) {
        return -1;
    }
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    // glutIdleFunc(render);
    glutMainLoop();
    return 0;
}


