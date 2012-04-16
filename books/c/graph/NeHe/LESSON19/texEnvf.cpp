#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <stdlib.h>          // Standard C/C++ Library
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header

#include "bmprw.h"
#include "nal_fps.h"


#define WCX           1280/1.5    // Window Width
#define WCY           720/1.5    // Window Height
#define	MAX_DIV_X     30
#define	MAX_DIV_Y     30
#define	MAX_PARTICLES (MAX_DIV_X * MAX_DIV_Y)
#define NOT_BURST_FRAME_NUM     30

#define TEXTURES_C1   0
#define TEXTURES_C2   1
#define TEXTURES_A1   2
#define TEXTURES_NUM  3


// Global Variables
bool    g_gamemode;            // GLUT GameMode ON/OFF
bool    g_fullscreen;
bool    g_keys[256];           // Keys Array
GLuint	g_texid[TEXTURES_NUM]; // Our Textures' Id List

static float    g_alpha = 1.0;


bool setup_textures(const char *bmpFile, GLuint *texID)
{
    int         ret;
    sbitData    *bmpr;

    bmpr = bmpCreateObjForRead(EBMP_RGBA, 0);
    ret = bmpRead(bmpFile, bmpr);
    assert(ret);

    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, *texID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            bmpr->w, bmpr->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmpr->pdata);

    bmpDestroyObjForRead(&bmpr);

    return true;
}

bool init(void)
{
    Fps_initProgTime();

    if ( !setup_textures("circle1.bmp", &g_texid[TEXTURES_C1]) )
        return false;
    if ( !setup_textures("circle2.bmp", &g_texid[TEXTURES_C2]) )
        return false;
    if ( !setup_textures("ActorBall6.bmp", &g_texid[TEXTURES_A1]) )
        return false;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_TEXTURE_2D);

    memset(g_keys,0,sizeof(g_keys));

    // glEnableClientState(GL_VERTEX_ARRAY);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    return true;
}


static inline void PTS_lookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
        GLfloat centerx, GLfloat centery, GLfloat centerz,
        GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    /* Make rotation matrix */
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    mag = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    mag = sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
}

void drawRect()
{
    float   x0 = -0.5;
    float   y0 = -0.5;
    float   x1 =  0.5;
    float   y1 =  0.5;
    float   s0 =  0.0;
    float   t0 =  0.0;
    float   s1 =  1.0;
    float   t1 =  1.0;

    glBegin(GL_TRIANGLE_FAN); {
        glTexCoord2d(s0, t0); glVertex3f(x0, y0, 0.0);
        glTexCoord2d(s1, t0); glVertex3f(x1, y0, 0.0);
        glTexCoord2d(s1, t1); glVertex3f(x1, y1, 0.0);
        glTexCoord2d(s0, t1); glVertex3f(x0, y1, 0.0);
    } glEnd();
}

// Our Rendering Is Done Here
void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -2.0);


    glPushMatrix(); {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_texid[TEXTURES_C2]);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // glColor4f(0.0, 0.0, 0.0, 1.0);
        glColor4f(1.0, 1.0, 1.0, 1.0);

        glTranslatef(-0.8, 0.0, 0.0);
        glScalef(1.7, 1.7, 1.0);
        drawRect();
    } glPopMatrix();


    // 从物体从不透明到透明, 直至消失.
    glPushMatrix(); {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_texid[TEXTURES_C1]);
#if 1
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4f(1.0, 1.0, 1.0, g_alpha);
#else
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        glColor4f(0.0, 0.9, 0.0, g_alpha);
        // glAlphaFunc(GL_GREATER, 0.0f);
        // glEnable(GL_ALPHA_TEST);
        // glColor4f(0.0, 1.9, 0.0, g_alpha);
#endif
        drawRect();
        glDisable(GL_ALPHA_TEST);
    } glPopMatrix();



    glFlush();
    glutSwapBuffers ( );

    // Fps_updateFPS();
    //usleep(30 * 1000);
}

void reshape(int w, int h)
{
    // int sx = 0;
    // int sy = 180;
    // int ex = w;
    // int ey = 420;
    // w = ex - sx;
    // h = ey - sy;

    // glViewport(sx, sy, w, h);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (h == 0) h = 1;
    glFrustum(-(float)w/h/2.0, (float)w/h/2.0, -0.5, 0.5, 1.0, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void game_function(void)
{
    render();
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a': g_alpha += 0.01; break;
        case 'A': g_alpha -= 0.01; break;
        case 27:
                  exit(0);
                  break;
        default:
                  g_keys[key] = true;
                  break;
    }
    printf("NAL &&& Alpha=%f\n", g_alpha);
}

void special_keys(int a_keys, int x, int y)
{
    switch (a_keys) {
        case GLUT_KEY_F1:
            if (!g_gamemode) {
                g_fullscreen = !g_fullscreen;
                if (g_fullscreen) glutFullScreen();
                else glutReshapeWindow(WCX, WCY);
            }
            break;
        default:
            g_keys[a_keys] = true;
            break;
    }
}

// Our Keyboard Handler For Special Key Releases.
void special_keys_up(int key, int x, int y)
{
    g_keys[key] = false;
}

// Our Keyboard Handler For Normal Key Releases.
void normal_keys_up(unsigned char key, int x, int y)
{
    g_keys[key] = false;
}

// Ask The User If He Wish To Enter GameMode Or Not
void ask_gamemode()
{
    g_fullscreen = false;
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    ask_gamemode();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    if (g_gamemode) {
        glutGameModeString("640x480:16");
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();
        else g_gamemode = false;
    }
    if (!g_gamemode) {
        glutInitWindowSize(WCX, WCY);
        glutCreateWindow("NeHe's OpenGL Framework");
    }
    if (!init()) {
        printf("NAL &&& Initializtion Error!\n");
        return -1;
    }
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutKeyboardUpFunc(normal_keys_up);
    glutSpecialUpFunc(special_keys_up);
    glutIdleFunc(game_function);

    glutMainLoop();

    return 0;
}

