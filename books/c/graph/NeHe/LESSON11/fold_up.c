/*
   NeHe (nehe.gamedev.net) OpenGL tutorial series
   GLUT port.in 2001 by milix (milix_gr@hotmail.com)
   Most comments are from the original tutorials found in NeHe.
   For VC++ users, create a Win32 Console project and link 
   the program with glut32.lib, glu32.lib, opengl32.lib
   */

#include <string.h>
#include <unistd.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header
#include "M3D_math3d.h"
#include "M3D_plane.h"

typedef int bool;
#define true    1
#define false   0

#define TEXTURES_NUM 1       // We Have 1 Texture 

// 被细分为90个小矩形
#define X_NUMDIV    90
// Y_NUMDIV MUST is equal one!!
#define Y_NUMDIV    1

// 
// Use `GL_TRIANGLE_STRIP' draw it!
//    
//    1       3       5       6       9
//    ---------------------------------
//    |x      |x      |x      |x      |
//    | x     | x     | x     | x     |
//    |  x    |  x    |  x    |  x    |
//    |   0   |   1   |   2   |   3   |
//    |    x  |    x  |    x  |    x  |
//    |     x |     x |     x |     x |
//    |      x|      x|      x|      x|
//    ---------------------------------
//    0       2       4       6       8 
//
//    0,1,2...3 对应 X_NUMDIV.
//    0,1,2...9 对应 g_points 的下标.
//
#define NUM_VERTEXES        ((X_NUMDIV+1)*2)
// (0,1,2) is (x,y,z), (3,4) is (s,t).
#define NUM_PER_VERTEXE     5


// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
    GLuint   w;    // Image's Width
    GLuint   h;    // Image's Height
    GLubyte* data; // Image's Data (Pixels)
} RGBIMG;


// Global Variables

bool g_stop = 0;
bool g_isopenning = true;

float   g_points[(14+1)*2][NUM_PER_VERTEXE];   // The Array For The Points On The Grid Of Our "Wave"

int g_xnumFramePerCircle;

bool    g_fullscreen;                // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
int     g_wiggle = 0;                // Counter Used To Control How Fast Flag Waves
GLuint	g_texid[TEXTURES_NUM];       // Our Textures' Id List 
int     g_x_curframe = 0;

const float ang2rad = 2.0*3.141592654/360.0;

// 所申请的纹理大小
int tw = 256;
int th = 256;

// 图像大小
int iw = 256;
int ih = 256;

// 对象大小
int ow = 342;
int oh = 256;

float worldx, worldy, worldz;

typedef struct tagFoldUpObj
{
    float ow;
    float oh;
    float sw;
    float sh;
    float iw;
    float ih;
    float tw;
    float th;
    int   numDiv;
    int   curAngPosID;
} sFoldUpObj;

sFoldUpObj g_foldup;


// Loads A RGB Raw Image From A Disk File And Updates Our Image Reference
// Returns true On Success, False On Fail.
bool load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
{

#if 0
#else
    refimg->w = (GLuint) w;
    refimg->h = (GLuint) h;
    refimg->data = (GLubyte*)malloc(256*256*3);
    if (refimg->data == NULL) 
        return false;

    int x, y;
    GLubyte *pb = refimg->data;
    for ( y = 0; y < 256; y++ )
    {
        for ( x = 0; x < 256; x++ )
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
    }
    return true;
#endif
}

// Setup Our Textures. Returns true On Success, false On Fail
bool setup_textures()
{
    RGBIMG img;

    // Create The Textures' Id List
    glGenTextures(TEXTURES_NUM, g_texid);          
    // Load The Image From A Disk File
    if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return false;
    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
    // Finished With Our Image, Free The Allocated Data
    free(img.data);
    return true;
}

// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
    if (!setup_textures()) return false;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
    glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                 // Set The Blending Function (NEW)
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			   // Black Background (CHANGED)
    glClearDepth(1.0f);								   // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPolygonMode(GL_BACK, GL_FILL);                   // Back Face Is Solid (NEW)
    glPolygonMode(GL_FRONT, GL_FILL);                  // Front Face Is Made Of Lines (NEW)
    glPointSize(6);

    g_foldup.ow = 342 * 2;
    g_foldup.oh = 256;
    g_foldup.sw = 342;
    g_foldup.sh = 256;
    g_foldup.iw = 256;
    g_foldup.ih = 256;
    g_foldup.tw = 256;
    g_foldup.th = 256;
    g_foldup.numDiv = 14;
    g_foldup.curAngPosID = 6;

    return true;
}

void update_ver_and_tex()
{
#if 0
    // ([0.0, 2*ow/oh], [0.0, 2.0])
    // ([0.0, 2.2x], [0.0, 2.0])
    float ow_r = 2.0 * (float) ow / oh;
    float oh_r = 2.0 * (float) oh / oh;

    float ow_r_half = ow_r / 2.0;
    float oh_r_half = oh_r / 2.0;

    float xvstep = ow_r / (float)X_NUMDIV;
    float yvstep = oh_r / (float)Y_NUMDIV;
    float xtstep = (1.0/((float)iw/tw)) / (float)X_NUMDIV;
    float ytstep = (1.0/((float)ih/th)) / (float)Y_NUMDIV;

    // [(<<G:2>> P45 P109)]
    int ncircle = 4;     // 共须转 ncircle 圈
    // ncircle * 2 * PI * r == ow_r
    float r = ow_r / (ncircle*2.0*3.141592654);
    // 把柱面部分的纹理按顺时针方向贴到柱面上
    float theta = -360.0f / (X_NUMDIV / ncircle);
    // 初始化指针初始位置, 使得平面部分与柱面部分能够平滑衔接
    float rad180 = 3.141592654;
    g_xnumFramePerCircle = X_NUMDIV / ncircle;

    memset(g_points, 0, NUM_VERTEXES*NUM_PER_VERTEXE*sizeof(float));
    int x, y;
    for(y = 0; y < Y_NUMDIV; y++)
    {
        for (x = 0; x <= X_NUMDIV; x++) 
        {
            if ( g_x_curframe > x ) // 平面部分
            {
                // ([-1.x, 1.x], [-1.0, 1.0])

                g_points[2*x][0] = x * xvstep - ow_r_half;
                g_points[2*x][1] = y * yvstep - oh_r_half;
                g_points[2*x][2] = -r;
                g_points[2*x][3] = x * xtstep;
                g_points[2*x][4] = y * ytstep;

                g_points[2*x+1][0] = x * xvstep - ow_r_half;
                g_points[2*x+1][1] = (y+1) * yvstep - oh_r_half;
                g_points[2*x+1][2] = -r;
                g_points[2*x+1][3] = x * xtstep;
                g_points[2*x+1][4] = (y+1) * ytstep;
            }                    
            else // 卷轴部分
            {
                // 超过一圈的不再贴到柱面上
                if ( x - g_x_curframe >= g_xnumFramePerCircle )
                    break;

                float deta = (float)(x - g_x_curframe);

                g_points[2*x][0] = (float)sin(deta*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[2*x][1] = y*yvstep-oh_r_half;
                g_points[2*x][2] = (float)cos(deta*theta*ang2rad+rad180)*r;
                g_points[2*x][3] = x * xtstep;
                g_points[2*x][4] = y * ytstep;

                g_points[2*x+1][0] = (float)sin(deta*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[2*x+1][1] = (y+1)*yvstep-oh_r_half;
                g_points[2*x+1][2] = (float)cos(deta*theta*ang2rad+rad180)*r;
                g_points[2*x+1][3] = x * xtstep;
                g_points[2*x+1][4] = (y+1) * ytstep;
            }
        }
    }
#else
    // ([0.0, 2*ow/oh], [0.0, 2.0])
    // ([0.0, 2.2x], [0.0, 2.0])
    float ow_r = 2.0 * (float) g_foldup.ow / g_foldup.oh;
    float oh_r = 2.0 * (float) g_foldup.oh / g_foldup.oh;
    float sw_r = 2.0 * (float) g_foldup.sw / g_foldup.sh;
    float sh_r = 2.0 * (float) g_foldup.sh / g_foldup.sh;

    float ow_r_half = ow_r / 2.0;
    float oh_r_half = oh_r / 2.0;

    float xvstep = ow_r / g_foldup.numDiv;
    float yvstep = oh_r / 1.0;
    float xtstep = (1.0/(g_foldup.iw/g_foldup.tw)) / (float)g_foldup.numDiv;
    float ytstep = (1.0/(g_foldup.iw/g_foldup.tw)) / 1.0;

    float b = ow_r / g_foldup.numDiv;
    float a = (sw_r - 2.0*ow_r/g_foldup.numDiv) / (g_foldup.numDiv - 2);
    float c = sqrtf(b*b - a*a);
    float d = yvstep;

    // if ( g_foldup.curAngPosID % 2 == 1 )
    //     zstart = 0.0;
    // else
    //     zstart = -c;

    int x, y;
    for ( y = 0; y < 1; y++ )
    {
        for ( x = 0; x <= g_foldup.numDiv; x++ )
        {
            g_points[2*x][0] = x * a - ow_r_half;
            g_points[2*x][1] = y * d - oh_r_half;
            if ( (g_foldup.curAngPosID % 2 == 1 && x % 2 == 0)
                    || (g_foldup.curAngPosID % 2 == 0 && x % 2 == 1) )
                g_points[2*x][2] = 0.0;
            else
                g_points[2*x][2] = -c;
            g_points[2*x][3] = x * xtstep;
            g_points[2*x][4] = y * ytstep;

            g_points[2*x+1][0] = x * a - ow_r_half;
            g_points[2*x+1][1] = (y+1) * d - oh_r_half;
            if ( (g_foldup.curAngPosID % 2 == 1 && x % 2 == 0)
                    || (g_foldup.curAngPosID % 2 == 0 && x % 2 == 1) )
                g_points[2*x][2] = 0.0;
            else
                g_points[2*x][2] = -c;
            g_points[2*x+1][3] = x * xtstep;
            g_points[2*x+1][4] = (y+1) * ytstep;
        }
    }
#endif
}

// Our Rendering Is Done Here
void render(void)   
{
    int x, y;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();									// Reset The View

    glPushMatrix();
    glTranslatef(0.0f,0.0f,-2.0f);
    glEnable(GL_TEXTURE_2D);
    // 纹理单元与当前的片段颜色混合的算法由 glTexEnvi() 指定!
    // glTexEnvi() 默认使用的是 GL_MODULATE .
    // 即混合算法是: R(dst) = R(src) * R(tex) ...
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    update_ver_and_tex();
    // [(<<G:2>> P107)]
    glBegin(GL_TRIANGLE_STRIP);
    for (y = 0; y < Y_NUMDIV; y++) {
        for (x = 0; x <= X_NUMDIV; x++) {
            // 从此之后的不再有顶点数据
            if ( x - g_x_curframe >= g_xnumFramePerCircle )
                break;

            glTexCoord2f(g_points[2*x][3], g_points[2*x][4]);
            glVertex3f(g_points[2*x][0], g_points[2*x][1], g_points[2*x][2]);
            glTexCoord2f(g_points[2*x+1][3], g_points[2*x+1][4]);
            glVertex3f(g_points[2*x+1][0], g_points[2*x+1][1], g_points[2*x+1][2]);
        }
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
        glVertex3f(worldx, worldy, worldz);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(worldx - 0.1, worldy - 0.1, worldz);
        glVertex3f(worldx + 0.1, worldy - 0.1, worldz);
        glVertex3f(worldx + 0.1, worldy + 0.1, worldz);
        glVertex3f(worldx - 0.1, worldy + 0.1, worldz);
    glEnd();
#if 1
    // printf("NAL XX g_x_curframe=%d ...\n", g_x_curframe);
    if ( ! g_stop )
    {
        if ( g_isopenning )
        {
            if ( ++g_x_curframe >= X_NUMDIV )
                g_isopenning = false;
        }
        else
        {
            if ( --g_x_curframe <= 0 )
                g_isopenning = true;
        }
    }
#endif
    // usleep(1000 * 200);

    // Swap The Buffers To Become Our Rendering Visible
    glutSwapBuffers ( );
}

void resetup_matrix(int w, int h)
{
    float asp = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix

    glFrustum(-asp/2.0, asp/2.0, -0.5, 0.5, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (h == 0) h = 1;

    glFrustum(-(float)w/h/2.0, (float)w/h/2.0, -0.5, 0.5, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix

    glLoadIdentity(); // Reset The Modelview Matrix
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:        // When Escape Is Pressed...
            exit(0);    // Exit The Program
            break;          // Ready For Next Case
        case ' ':
            g_stop = !g_stop;
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
            // We Can Switch Between Windowed Mode And Fullscreen Mode Only
            g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
            if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
            else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
            break;
        default:
            break;
    }
}

/* 
 * 实现 gluUnProject() 的功能!
 * [(man gluUnProject)]
 * http://pyopengl.sourceforge.net/documentation/manual-3.0/gluUnProject.xhtml
 *
 *   | objX |              | (2*(winX-view[0]))/view[2] - 1 |
 *   | objY | =  INV(PM) * | (2*(winY-view[1]))/view[3] - 1 |
 *   | objZ |              |    2*winZ - 1                  |
 *   |  W   |              |        1                       |
 *
 */
int MygluUnProjectf(float winX, float winY, float winZ,
        const float *model, const float *proj, const GLint *view,
        float* objX, float* objY, float* objZ)
{
    M3DMatrix44f PM, Inv;
    M3DVector4f  V, R;

    m3dLoadVector4(V, (2.0*(winX-view[0])/view[2])-1.0,
                      (2.0*(winY-view[1])/view[3])-1.0,
                      (2.0*winZ)-1.0,
                      1.0 );
    m3dMatrixMultiply44(PM, proj, model);
    if ( ! m3dInvertMatrix44(Inv, PM) )
        return 0;
    m3dMatrix44fMultiplyVector4f(R, Inv, V);

    *objX = R[0] / R[3];
    *objY = R[1] / R[3];
    *objZ = R[2] / R[3];

    return 1;
}

void ProcessSelection(int xPos, int yPos, float *worx, float *wory, float *worz)
{
	int viewport[4];
	double nx, ny, nz, fx, fy, fz;

    // save Current matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

	glGetIntegerv(GL_VIEWPORT, viewport);
    resetup_matrix(viewport[2], viewport[3]);

#if 0
	double modelMatrix[16], projMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix); // 获取视图矩阵
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix); // 获取投影矩阵
    // 求出两点, 用于确定一条向量
	gluUnProject((double)xPos, (double)(viewport[3] - yPos), 0.0,
            modelMatrix, projMatrix, viewport, &nx, &ny, &nz);
	gluUnProject((double)xPos, (double)(viewport[3] - yPos), 1.0,
            modelMatrix, projMatrix, viewport, &fx, &fy, &fz);
#else
	float nxf, nyf, nzf, fxf, fyf, fzf;
	float modelMatrixf[16], projMatrixf[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrixf); // 获取视图矩阵
	glGetFloatv(GL_PROJECTION_MATRIX, projMatrixf); // 获取投影矩阵
	MygluUnProjectf((float)xPos, (float)(viewport[3] - yPos), 0.0,
            modelMatrixf, projMatrixf, viewport, &nxf, &nyf, &nzf);
	MygluUnProjectf((float)xPos, (float)(viewport[3] - yPos), 1.0,
            modelMatrixf, projMatrixf, viewport, &fxf, &fyf, &fzf);
    nx = nxf; ny = nyf; nz = nzf;
    fx = fxf; fy = fyf; fz = fzf;
#endif
    printf("nx=%f ny=%f nz=%f,  fx=%f fy=%f fz=%f\n", nx, ny, nz, fx, fy, fz);

    M3DPlane4f p;
    M3DVector3f N, dir;
    m3dLoadVector3(dir, fx-nx, fy-ny, fz-nz);
    m3dLoadVector3(N, 0.0, 0.0, 1.0);
    m3dLoadPlanev(p, N, -(-2.0));
// #if 0
    double pndotrd = p[0]*dir[0] + p[1]*dir[1] + p[2]*dir[2];
	double point_len = -(p[0]*nx + p[1]*ny + p[2]*nz + p[3]) / pndotrd;

	*worx =  nx + (point_len * dir[0]);
	*wory =  ny + (point_len * dir[1]);
	*worz =  nz + (point_len * dir[2]);
    printf("1w (%f, %f, %f)\n", *worx, *wory, *worz);
// #else
    M3DVector3f wor, rayPos;
    m3dLoadVector3(rayPos, nx, ny, nz);
    m3dRayIntersection(p, wor, rayPos, dir);
	*worx =  wor[0];
	*wory =  wor[1];
	*worz =  wor[2];
    printf("2w (%f, %f, %f)\n", wor[0], wor[1], wor[2]);
// #endif

    // restore to prev matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

///////////////////////////////////////////////////////////
// Process the mouse click
void MouseCallback(int button, int state, int x, int y)
{
    float wx, wy, wz;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        ProcessSelection(x, y, &wx, &wy, &wz);
        printf("3w (%f, %f, %f)\n", wx, wy, wz);
        worldx = wx;
        worldy = wy;
        worldz = wz;
        glutPostRedisplay();
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
	glutMouseFunc(MouseCallback);
    glutIdleFunc(render);                        // We Do Rendering In Idle Time
    glutMainLoop();                              // Go To GLUT Main Loop
    return 0;
}
