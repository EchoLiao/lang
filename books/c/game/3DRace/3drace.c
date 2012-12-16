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
#include <stdlib.h>
#include <sys/time.h>

#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SCREEN_W        480
#define SCREEN_H        320
#define TEXTURES_NUM    1
#define NUM_PENTS       50



int     g_gamemode;
int     g_fullscreen;
float   g_rand[NUM_PENTS][3];
float   g_deltaZ;
float   g_cenZ = 5.18;
float   g_handX;
float   g_cenX = 0.05;
float   g_accX = 1.0;
int64_t g_startTime;


static inline int64_t getTimeInUsec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void resetRandPos()
{
    int i;

    for ( i = 0; i < NUM_PENTS; i++ )
    {
        g_rand[i][0] = (-100.0 + rand() % 200) / 100.0;
        g_rand[i][1] = -rand() % 140;
        g_rand[i][2] = 1.0;
        // printf("NAL %f, %f\n", g_rand[i][0], g_rand[i][1]);
    }
}


int init(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(0.5);

#if 1
    GLfloat values[2];
    glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values); // 线条的精确度
    printf ("GL_LINE_WIDTH_GRANULARITY %3.1f\n", values[0]);
    glGetFloatv (GL_LINE_WIDTH_RANGE, values);
    printf ("GL_LINE_WIDTH_RANGE %3.1f - %3.1f\n", values[0], values[1]);
#endif

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    resetRandPos();
    g_startTime = getTimeInUsec();

    return 1;
}

static void drawCube()
{
    glutWireCube(1.0);
}

static void drawPentahedral()
{
    glBegin(GL_TRIANGLE_FAN); {
        glVertex3f(0.0, 0.0, -1.0);
        glVertex3f(-1.0,  1.0, 1.0);
        glVertex3f( 1.0,  1.0, 1.0);
        glVertex3f( 1.0, -1.0, 1.0);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f(-1.0,  1.0, 1.0);
    } glEnd();
}

static void drawRandPentahedrals()
{
    const float scalex = 0.05;
    const float scaley = 0.05;
    const float scalez = 0.20;
    int i;

    for ( i = 0; i < NUM_PENTS; i++ )
    {
        glPushMatrix(); {
            glTranslatef(g_rand[i][0], -0.25, g_rand[i][1]);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(scalex, scaley, scalez);
            drawPentahedral();
        } glPopMatrix();
    }
}

static void drawRoute()
{
    float sx1 = -1.0;
    float sx2 =  1.0;
    float sy1 = -0.5;
    float sy2 = sy1;
    float sz1 =  3.0;
    float sz2 = sz1;

    // float ex1 = sx1;
    // float ex2 = ex1;
    float ey1 = sy1;
    float ey2 = ey1;
    float ez1 = -140.0;
    float ez2 = ez1;

    float cenX = 0.2;
    float curX;

    for ( curX = sx1; curX <= sx2; curX += cenX )
    {
        glBegin(GL_LINES); {
            glVertex3f(curX, sy2, sz2);
            glVertex3f(curX, ey2, ez2);
        } glEnd();
    }
}

// Our Rendering Is Done Here
void render(void)
{
    int64_t nowTime = getTimeInUsec();
    g_deltaZ += g_cenZ * (nowTime - g_startTime) / (float)1000000;
    // printf("NAL FPS=%.1f\n",  1.0/((nowTime-g_startTime)/1000000.0));
    g_startTime = nowTime;
    if ( g_deltaZ > 90.0 ) {
        g_deltaZ = 0.0;
        resetRandPos();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
            0.0, 0.0, -100.0,
            0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0f, -0.0f);

    glPushMatrix(); {
        glTranslatef(0.0, 0.0, g_deltaZ);
        glColor4f(0.0, 1.0, 0.0, 1.0);
        drawRoute();
    } glPopMatrix();

    glPushMatrix(); {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glTranslatef(0.0, 0.0, -2.0);
        glScalef(0.99, 0.99, 0.99);
        glRectf(-0.02, -0.02, 0.02, 0.02);
        glRectf(-1.0, -1.0, 1.0, 1.0);
    } glPopMatrix();

    glPushMatrix(); {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK,  GL_LINE);
        glTranslatef(0.0, 0.0, g_deltaZ);
        glColor4f(1.0, 0.0, 1.0, 1.0);
        drawRandPentahedrals();
    } glPopMatrix();

    const float sCubeZ = -1.3;
    glPushMatrix(); {
        glTranslatef(g_handX, -0.3, sCubeZ);
        // glTranslatef(0.0, 0.0, g_deltaZ);
        glScalef(0.065, 0.065, 0.065);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        drawCube();
    } glPopMatrix();

    glutSwapBuffers();

    int i;
    const float sAccuracy = 0.1100;
    for ( i = 0; i < NUM_PENTS; i++ ) {
        if ( g_rand[i][2] > 0.0 &&
                fabs(g_rand[i][1] + g_deltaZ - sCubeZ) < sAccuracy &&
                fabs(g_rand[i][0] - g_handX) < sAccuracy
           ) {
            g_rand[i][2] = -1.0;
            printf("NAL rand[%d]=(%f,%f), handX=%f, deltaZ=%f\n", i,
                    g_rand[i][0], g_rand[i][1], g_handX, g_deltaZ);
            break;
        }
    }
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
        case 'f':
            g_accX *= 1.125;
            printf("NAL g_accX=%f\n", g_accX);
            break;
        default:
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key) {
        case 'f':
            //g_accX = 1.0;
            printf("NAL UP g_accX=%f\n", g_accX);
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
        case GLUT_KEY_UP:
            g_cenZ *= 1.1;
            printf("g_cenZ = %f\n", g_cenZ);
            break;
        case GLUT_KEY_DOWN:
            g_cenZ *= 0.9;
            printf("g_cenZ = %f\n", g_cenZ);
            break;
        case GLUT_KEY_LEFT:
            g_handX -= g_cenX * g_accX;
            // printf("g_handX = %f\n", g_handX);
            break;
        case GLUT_KEY_RIGHT:
            g_handX += g_cenX * g_accX;
            // printf("g_handX = %f\n", g_handX);
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
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(special_keys);
    glutIdleFunc(render);
    glutMainLoop();
    return 0;
}

