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



#define SCREEN_W        480
#define SCREEN_H        340
#define TEXTURES_NUM    1



int     g_gamemode;
int     g_fullscreen;


int  init(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

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

    glPushMatrix(); {
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4f(1.0, 0.0, 1.0, 1.0);
        glScalef(0.99, 0.99, 0.99);
        glRectf(-0.01, -0.01, 0.01, 0.01);
        glRectf(-1.0, -1.0, 1.0, 1.0);
    } glPopMatrix();

    glutSwapBuffers();

    usleep(1000 * 200);
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    float ratio;

    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if ( w >= h )
    {
        ratio = (float)w / (float)h;
        glFrustum(-ratio/2.0, ratio/2.0, -0.5, 0.5, 1.0, 100);
    }
    else
    {
        ratio = (float)h / (float)w;
        glFrustum(-0.5, 0.5, -ratio/2.0, ratio/2.0, 1.0, 100);
    }

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
                else glutReshapeWindow(SCREEN_W, SCREEN_H);
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
        glutInitWindowSize(SCREEN_W, SCREEN_H);
        glutCreateWindow("NeHe's OpenGL Framework");
    }
    if (!init()) {
        return -1;
    }
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutIdleFunc(render);
    glutMainLoop();
    return 0;
}

