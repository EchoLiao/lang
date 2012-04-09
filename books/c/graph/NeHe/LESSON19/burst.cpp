#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <stdlib.h>          // Standard C/C++ Library
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header

#include "bmprw.h"
#include "nal_fps.h"


#define WCX           640    // Window Width
#define WCY           480    // Window Height
#define TEXTURES_NUM  1      // We Have 1 Texture
#define	MAX_DIV_X     30
#define	MAX_DIV_Y     30
#define	MAX_PARTICLES (MAX_DIV_X * MAX_DIV_Y)
#define NOT_BURST_FRAME_NUM     30



// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
    GLuint   w;    // Image's Width
    GLuint   h;    // Image's Height
    GLubyte* data; // Image's Data (Pixels)
} RGBIMG;

// Global Variables
bool    g_gamemode;            // GLUT GameMode ON/OFF
bool    g_fullscreen;
bool    g_keys[256];           // Keys Array
bool    g_rainbow=true;
float	g_slowdown = 2.0f;     // Slow Down Particles
float   g_xspeed = 0.0f;	   // X Rotation Speed
float   g_yspeed = 0.0f;	   // Y Rotation Speed
float	g_zoom =  -2.0f;       // Used To Zoom Out
GLuint	g_col = 0;             // Current Color Selection
GLuint	g_delay = 0;           // Rainbow Effect Delay
GLuint	g_texid[TEXTURES_NUM]; // Our Textures' Id List

int     g_isTran    = 0;
int     g_tranStep  = 0;
int     g_notBurstNum = NOT_BURST_FRAME_NUM;
float   g_lastResetTime;
float   g_cenX[MAX_PARTICLES] = { 0.0, };
float   g_cenY[MAX_PARTICLES] = { 0.0, };
float   g_cenZ[MAX_PARTICLES] = { 0.0, };
const int   g_tranSteps  = 100;
const float g_leftBtoX = 0.0;
const float g_leftBtoY = 0.0;
const float g_leftBtoZ = 0.0;
const float g_leftBtoS = 0.0;
const float g_leftBtoT = 0.0;
const float g_ObjW     = 1.0;
const float g_ObjH     = 1.0;
const float g_ObjL     = 0.0;
const float g_TexW     = 1.0;
const float g_TexH     = 1.0;
const float g_divObjW  = g_ObjW / MAX_DIV_X;
const float g_divObjH  = g_ObjH / MAX_DIV_Y;
const float g_divObjL  = 0.0;
const float g_divTexW  = g_TexW / MAX_DIV_X;
const float g_divTexH  = g_TexH / MAX_DIV_Y;



typedef struct {    // Create A Structure For Particle
    bool  active;   // Active (Yes/No)
    float life;     // Particle Life
    float fade;     // Fade Speed
    float r;        // Red Value
    float g;        // Green Value
    float b;        // Blue Value
    float x;        // X Position
    float y;        // Y Position
    float z;        // Z Position
    float s;
    float t;
    float xi;       // X Direction
    float yi;       // Y Direction
    float zi;       // Z Direction
    float xg;       // X Gravity
    float yg;       // Y Gravity
    float zg;       // Z Gravity
} particles;        // Particles Structure

particles g_particle[MAX_PARTICLES];
particles g_OriPtile[MAX_PARTICLES];

static GLfloat COLORS[12][3] = {     // Rainbow Of Colors
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

bool load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
{
    GLuint   sz;    // Our Image's Data Field Length In Bytes
    FILE*    file;  // The Image's File On Disk
    long     fsize; // File Size In Bytes
    GLubyte* p;     // Helper Pointer

    // Update The Image's Fields
    refimg->w = (GLuint) w;
    refimg->h = (GLuint) h;
    sz = (((3*refimg->w+3)>>2)<<2)*refimg->h;
    refimg->data = new GLubyte [sz];
    if (refimg->data == NULL) return false;

    // Open The File And Read The Pixels
    file = fopen(file_name , "rb");
    if (!file) return false;
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    if (fsize != (long)sz) {
        fclose(file);
        return false;
    }
    fseek(file, 0L, SEEK_SET);
    p = refimg->data;
    while (fsize > 0) {
        fread(p, 1, 1, file);
        p++;
        fsize--;
    }
    fclose(file);
    return true;
}

bool setup_textures()
{
    int         ret;
    sbitData    *bmpr;

    bmpr = bmpCreateObjForRead(EBMP_RGBA, 0);
    ret = bmpRead("ActorBall6.bmp", bmpr);
    assert(ret);

    glGenTextures(TEXTURES_NUM, g_texid);
    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            bmpr->w, bmpr->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmpr->pdata);

    bmpDestroyObjForRead(&bmpr);

    return true;
}

bool particlesInit()
{
    int i;
    float bsX = g_leftBtoX - g_ObjW / 2.0;
    float bsY = g_leftBtoY - g_ObjH / 2.0;
    float bsZ = g_leftBtoZ - g_ObjL / 2.0;
    float bsS = g_leftBtoS;
    float bsT = g_leftBtoT;

    for ( i=0; i<MAX_PARTICLES; i++ ) {
        int ix = i % MAX_DIV_X;
        int iy = i / MAX_DIV_Y;
        g_OriPtile[i].active = g_particle[i].active = true;
        g_OriPtile[i].life   = g_particle[i].life   = 1.0f;
        g_OriPtile[i].fade   = g_particle[i].fade   = float(rand()%100)/1000.0f+0.003f;
        g_OriPtile[i].r      = g_particle[i].r      = COLORS[(i+1)/(MAX_PARTICLES/12)][0];
        g_OriPtile[i].g      = g_particle[i].g      = COLORS[(i+1)/(MAX_PARTICLES/12)][1];
        g_OriPtile[i].b      = g_particle[i].b      = COLORS[(i+1)/(MAX_PARTICLES/12)][2];
        g_OriPtile[i].x      = g_particle[i].x      = bsX + (float)ix * g_divObjW;
        g_OriPtile[i].y      = g_particle[i].y      = bsY + (float)iy * g_divObjH;
        g_OriPtile[i].z      = g_particle[i].z      = bsZ;
        g_OriPtile[i].s      = g_particle[i].s      = bsS + (float)ix * g_divTexW;
        g_OriPtile[i].t      = g_particle[i].t      = bsT + (float)iy * g_divTexH;
        g_OriPtile[i].xi     = g_particle[i].xi     = float((rand()%50)-26.0f)*10.0f;
        g_OriPtile[i].yi     = g_particle[i].yi     = float((rand()%50)-25.0f)*10.0f;
        g_OriPtile[i].zi     = g_particle[i].zi     = float((rand()%50)-25.0f)*10.0f;
        g_OriPtile[i].xg     = g_particle[i].xg     = 0.0f;
        g_OriPtile[i].yg     = g_particle[i].yg     = -0.8f;
        g_OriPtile[i].zg     = g_particle[i].zg     = 0.0f;
    }

    return true;
}

void particlesReset()
{
    int i;

    g_lastResetTime = Fps_getProgTime();
    g_notBurstNum = NOT_BURST_FRAME_NUM;

    for ( i = 0; i < MAX_PARTICLES; i++ ) {
        g_particle[i].active = g_OriPtile[i].active;
        g_particle[i].life   = g_OriPtile[i].life;
        g_particle[i].fade   = g_OriPtile[i].fade;
        g_particle[i].r      = g_OriPtile[i].r;
        g_particle[i].g      = g_OriPtile[i].g;
        g_particle[i].b      = g_OriPtile[i].b;
        g_particle[i].x      = g_OriPtile[i].x;
        g_particle[i].y      = g_OriPtile[i].y;
        g_particle[i].z      = g_OriPtile[i].z;
        g_particle[i].s      = g_OriPtile[i].s;
        g_particle[i].t      = g_OriPtile[i].t;
        g_particle[i].xi     = g_OriPtile[i].xi;
        g_particle[i].yi     = g_OriPtile[i].yi;
        g_particle[i].zi     = g_OriPtile[i].zi;
        g_particle[i].xg     = g_OriPtile[i].xg;
        g_particle[i].yg     = g_OriPtile[i].yg;
        g_particle[i].zg     = g_OriPtile[i].zg;
    }
}

bool init(void)
{
    Fps_initProgTime();

    if (!setup_textures())
        return false;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,g_texid[0]);

    particlesInit();
    memset(g_keys,0,sizeof(g_keys));

    return true;
}

void ParticlesDraw(particles *par, int n)
{
    int i;

    for ( i=0; i < n; i++ ) 
    {
        if ( !par[i].active ) 
            continue;
        if ( g_isTran ) 
        {
            par[i].x += g_cenX[i];
            par[i].y += g_cenY[i];
            par[i].z += g_cenZ[i];
        }
        float x  = par[i].x;
        float y  = par[i].y;
        float z  = par[i].z + g_zoom;
        float cx = g_divObjW;
        float cy = g_divObjH;
        float s  = par[i].s;
        float t  = par[i].t;
        float cs = g_divTexW;
        float ct = g_divTexH;

#if 0 // NALD
        if ( i==0 || i==3 || i==23 )
            printf("NAL &&& i=%d, (x,y,z)=(%f,%f,%f)\n", i, x, y, z);
#endif

        // glColor4f(par[i].r, par[i].g, par[i].b, par[i].life);

        //glDisable(GL_TEXTURE_2D); // NALD
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(s   , t+ct); glVertex3f(x,    y+cy, z);
        glTexCoord2d(s   , t   ); glVertex3f(x,    y,    z);
        glTexCoord2d(s+cs, t+ct); glVertex3f(x+cx, y+cy, z);
        glTexCoord2d(s+cs, t   ); glVertex3f(x+cx, y,    z);
        glEnd();
#if 0
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z);
        glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z);
        glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z);
        glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z);
        glEnd();

        par[i].x += par[i].xi/(g_slowdown*1000);
        par[i].y += par[i].yi/(g_slowdown*1000);
        par[i].z += par[i].zi/(g_slowdown*1000);

        par[i].xi += par[i].xg;
        par[i].yi += par[i].yg;
        par[i].zi += par[i].zg;
        par[i].life -= par[i].fade;

        if (par[i].life < 0.0f) {
            par[i].life = 1.0f;
            par[i].fade = float(rand()%100)/1000.0f+0.003f;
            par[i].x = 0.0f;
            par[i].y = 0.0f;
            par[i].z = 0.0f;
            par[i].xi = g_xspeed+float((rand()%60)-32.0f);
            par[i].yi = g_yspeed+float((rand()%60)-30.0f);
            par[i].zi = float((rand()%60)-30.0f);
            par[i].r = COLORS[g_col][0];
            par[i].g = COLORS[g_col][1];
            par[i].b = COLORS[g_col][2];
        }

        // If Number Pad 8 And Y Gravity Is Less Than 1.5 Increase Pull Upwards
        if (g_keys['8'] && (par[i].yg<1.5f)) par[i].yg += 1.01f;

        // If Number Pad 2 And Y Gravity Is Greater Than -1.5 Increase Pull Downwards
        if (g_keys['2'] && (par[i].yg>-1.5f)) par[i].yg -= 1.01f;

        // If Number Pad 6 And X Gravity Is Less Than 1.5 Increase Pull Right
        if (g_keys['6'] && (par[i].xg<1.5f)) par[i].xg += 1.01f;

        // If Number Pad 4 And X Gravity Is Greater Than -1.5 Increase Pull Left
        if (g_keys['4'] && (par[i].xg>-1.5f)) par[i].xg -= 1.01f;

        if (g_keys['\t']) {
            par[i].x = 0.0f;
            par[i].y = 0.0f;
            par[i].z = 0.0f;
            par[i].xi = float((rand()%50)-26.0f)*10.0f;
            par[i].yi = float((rand()%50)-25.0f)*10.0f;
            par[i].zi = float((rand()%50)-25.0f)*10.0f;
        }
#endif
    }
}

void ParticlesUpdate(particles *par, int n)
{
    int i;

    if ( --g_notBurstNum > 0 )
        return;

    for ( i=0; i < n; i++ ) {
        if ( par[i].active ) {
            par[i].x += par[i].xi/(g_slowdown*1000);
            par[i].y += par[i].yi/(g_slowdown*1000);
            par[i].z += par[i].zi/(g_slowdown*1000);

            par[i].xi += par[i].xg;
            par[i].yi += par[i].yg;
            par[i].zi += par[i].zg;
            par[i].life -= par[i].fade;

            if ( par[i].life < 0.0f ) 
            {
                par[i].life = 1.0f;
                par[i].fade = float(rand()%100)/1000.0f+0.003f;
                par[i].x = 0.0f;
                par[i].y = 0.0f;
                par[i].z = 0.0f;
                par[i].xi = g_xspeed+float((rand()%60)-32.0f);
                par[i].yi = g_yspeed+float((rand()%60)-30.0f);
                par[i].zi = float((rand()%60)-30.0f);
                par[i].r = COLORS[g_col][0];
                par[i].g = COLORS[g_col][1];
                par[i].b = COLORS[g_col][2];
            }

            if (g_keys['\t']) {
                par[i].x = 0.0f;
                par[i].y = 0.0f;
                par[i].z = 0.0f;
                par[i].xi = float((rand()%50)-26.0f)*10.0f;
                par[i].yi = float((rand()%50)-25.0f)*10.0f;
                par[i].zi = float((rand()%50)-25.0f)*10.0f;
            }
        }
    }
}

void ParTranInit()
{
    int i;

    for ( i = 0; i < MAX_PARTICLES; i++ )
    {
        g_cenX[i] = (g_OriPtile[i].x - g_particle[i].x) / g_tranSteps;
        g_cenY[i] = (g_OriPtile[i].y - g_particle[i].y) / g_tranSteps;
        g_cenZ[i] = (g_OriPtile[i].z - g_particle[i].z) / g_tranSteps;
#if 1 // NALD
        printf("NAL &^^^& i=%d, (%f,%f,%f)\n", i,
                g_cenX[i], g_cenY[i], g_cenZ[i]);
#endif
    }
}

void PTS_draw()
{
    ParticlesDraw(g_particle, MAX_PARTICLES);
}

void PTS_update()
{
    if ( Fps_getProgTime() - g_lastResetTime > 3.00 )
    {
        g_isTran = 1;
        if ( g_tranStep == 0 )
            ParTranInit();
        // particlesReset();
    }
    else
    {
        ParticlesUpdate(g_particle, MAX_PARTICLES);
    }

    if ( g_tranStep == g_tranSteps )
    {
        g_lastResetTime = Fps_getProgTime();
        g_isTran = 0;
        g_tranStep = 0;
        g_notBurstNum = NOT_BURST_FRAME_NUM;
    }
    if ( g_isTran && g_tranStep <= g_tranSteps)
    {
        g_tranStep++;
    }
}

// Our Rendering Is Done Here
void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    PTS_update();
    PTS_draw();


    // ParticlesDraw(g_particle, MAX_PARTICLES);

    // if ( Fps_getProgTime() - g_lastResetTime > 3.00 )
    // {
    //     g_isTran = 1;
    //     if ( g_tranStep == 0 )
    //         ParTranInit();
    //     // particlesReset();
    // }
    // else
    // {
    //     ParticlesUpdate(g_particle, MAX_PARTICLES);
    // }

    glFlush();
    glutSwapBuffers ( );

    Fps_updateFPS();
    //usleep(30 * 1000);
}

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

void game_function(void)
{
    if (g_keys[GLUT_KEY_PAGE_UP])	g_zoom += 0.1f;	// Zoom In
    if (g_keys[GLUT_KEY_PAGE_DOWN])	g_zoom -= 0.1f;	// Zoom Out
    if (g_keys[GLUT_KEY_UP] && (g_yspeed < 200)) g_yspeed += 9.0f;
    if (g_keys[GLUT_KEY_DOWN] && (g_yspeed > -200)) g_yspeed -= 9.0f;
    if (g_keys[GLUT_KEY_RIGHT] && (g_xspeed < 200)) g_xspeed += 9.0f;
    if (g_keys[GLUT_KEY_LEFT] && (g_xspeed > -200)) g_xspeed -= 9.0f;
    if (g_keys[' '] || (g_rainbow && (g_delay>25))) {
        if (g_keys[' ']) g_rainbow = false;
        g_delay = 0;
        g_col++;
        if (g_col > 11) g_col = 0;
    }
    g_delay++;

    render();
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case '+':
            if (g_slowdown > 1.0f) g_slowdown -= 0.01f;
            break;
        case '-':
            if (g_slowdown < 4.0f) g_slowdown += 0.01f;
            break;
        case '\n':
            g_rainbow = !g_rainbow;
            break;
        default:
            g_keys[key] = true;
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
        return -1;
    }
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutKeyboardUpFunc(normal_keys_up);
    glutSpecialUpFunc(special_keys_up);
    glutIdleFunc(game_function);

    g_lastResetTime = Fps_getProgTime();
    glutMainLoop();

    return 0;
}
