#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header
#include "M3D_math3d.h"
#include "M3D_plane.h"

void update_ver_and_tex();


typedef int bool;
#define true    1
#define false   0

#define TEXTURES_NUM 1       // We Have 1 Texture 

// (0,1,2) is (x,y,z), (3,4) is (s,t).
#define NUM_PER_VERTEXE     5



// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
    GLuint   w;    // Image's Width
    GLuint   h;    // Image's Height
    GLubyte* data; // Image's Data (Pixels)
} RGBIMG;


// Global Variables

bool    g_stop = 0;
float   g_points[(14+1)*2][NUM_PER_VERTEXE];   // The Array For The Points On The Grid Of Our "Wave"
bool    g_fullscreen;                // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
GLuint	g_texid[TEXTURES_NUM];       // Our Textures' Id List 

int     windowW;
int     windowH;
int     viewW;
int     viewH;

float g_Ztran = -3.0f;
float worldx, worldy, worldz;

typedef struct tagFoldUpObj
{

    float ow; // 对象大小
    float oh;
    float vw; // 视口大小
    float vh;
    float iw; // 图像大小
    float ih;
    float tw; // 所申请的纹理大小
    float th;
    int   numDiv;
    int   curAngPosID;
    int   lastAngPosID;
    int   numFrame;
    int   curFrame;
    int   isNewClick;
} sFoldUpObj;

sFoldUpObj g_foldup;



// Loads A RGB Raw Image From A Disk File And Updates Our Image Reference
// Returns true On Success, False On Fail.
bool load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
{

#if 1
    GLuint   sz;    // Our Image's Data Field Length In Bytes
    FILE*    file;  // The Image's File On Disk
    long     fsize; // File Size In Bytes
    GLubyte* p;     // Helper Pointer

    // Update The Image's Fields
    refimg->w = (GLuint) w;
    refimg->h = (GLuint) h;
    // 所加载的是24位的位图; 使每一行都以4字节对齐, 以加载读取速度.
    sz = (((3*refimg->w+3)>>2)<<2)*refimg->h;
    refimg->data = (GLubyte*)malloc(256*256*3);
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

void st_foldup_init()
{
    g_foldup.ow = 256 * 14;
    g_foldup.oh = 256;
    g_foldup.vw = viewW;
    g_foldup.vh = viewH;
    g_foldup.iw = 256;
    g_foldup.ih = 256;
    g_foldup.tw = 256;
    g_foldup.th = 256;
    g_foldup.numDiv = 14;
    g_foldup.curAngPosID = 13;
    g_foldup.lastAngPosID = g_foldup.curAngPosID;
    g_foldup.numFrame = 32;
    g_foldup.curFrame = 0;
    g_foldup.isNewClick = 0;

    assert(g_foldup.ow > g_foldup.vw);
    assert(g_foldup.numDiv % 2 == 0);
    assert(g_foldup.numFrame > 1);

    update_ver_and_tex();
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

   glFrontFace(GL_CW); // 顶点顺序是顺时针方向的表面是正面
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glPointSize(6);

    st_foldup_init();

    return true;
}

/* 
   [(<<G:2>> P111)]
*/
void st_foldup_update_frame()
{
    if ( g_foldup.curAngPosID == g_foldup.lastAngPosID )
        return;

    float ow = 2.0 * (float) g_foldup.ow / g_foldup.oh;
    float sw = 2.0 * (float) g_foldup.vw / g_foldup.vh;
    float b = ow / g_foldup.numDiv;
    float a = (sw - 2.0*b) / (g_foldup.numDiv - 2);
    float c = sqrtf(b*b - a*a);

    float e = (g_foldup.curFrame + 1) * (c / g_foldup.numFrame);
    float s1 = sqrtf(b*b - (c-e)*(c-e)) - a;
    float s2 = 2.0 * s1;
    float g = sqrtf(b*b - (b-s1)*(b-s1));

    int curid = g_foldup.curAngPosID;
    int lastid = g_foldup.lastAngPosID;
    int x, y;
    for ( y = 0; y < 1; y++ )
    {
        for ( x = 0; x <= g_foldup.numDiv; x++ )
        {
            if ( curid < lastid )
            {
                if ( x <= curid - 1 )
                {
                }
                else if ( x == curid )
                {
                    g_points[2*x][0] += s1;
                    g_points[2*x][2] += e;
                    g_points[2*x+1][0] += s1;
                    g_points[2*x+1][2] += e;
                }
                else if ( x >= curid + 1 && x <= lastid - 1 )
                {
                    g_points[2*x][0] += s2;
                    g_points[2*x+1][0] += s2;
                }
                else if ( x == lastid )
                {
                    g_points[2*x][0] += s1;
                    g_points[2*x][2] -= g;
                    g_points[2*x+1][0] += s1;
                    g_points[2*x+1][2] -= g;
                }
                else
                {
                }
            }
            else
            {
                if ( x <= lastid - 1 )
                {
                }
                else if ( x == lastid )
                {
                    g_points[2*x][0] -= s1;
                    g_points[2*x][2] -= e;
                    g_points[2*x+1][0] -= s1;
                    g_points[2*x+1][2] -= e;
                }
                else if ( x >= lastid + 1 && x <= curid - 1 )
                {
                    g_points[2*x][0] -= s2;
                    g_points[2*x+1][0] -= s2;
                }
                else if ( x == curid )
                {
                    g_points[2*x][0] -= s1;
                    g_points[2*x][2] += g;
                    g_points[2*x+1][0] -= s1;
                    g_points[2*x+1][2] += g;
                }
                else
                {
                }
            }
        }
    }
}

/* 
   [(<<G:2>> P111)]
*/
void update_ver_and_tex()
{
    // ([0.0, 2*ow/oh], [0.0, 2.0])
    // ([0.0, 2.2x], [0.0, 2.0])
    float ow = 2.0 * (float) g_foldup.ow / g_foldup.oh;
    float oh = 2.0 * (float) g_foldup.oh / g_foldup.oh;
    float sw = 2.0 * (float) g_foldup.vw / g_foldup.vh;
    float sh = 2.0 * (float) g_foldup.vh / g_foldup.vh;

    float sw_half = sw / 2.0;
    float sh_half = sh / 2.0;

    float b = ow / g_foldup.numDiv;
    float a = (sw - 2.0*b) / (g_foldup.numDiv - 2);
    float c = sqrtf(b*b - a*a);
    float d = oh / 1; // (oh / numDivInYdir)
    int   curid = g_foldup.lastAngPosID;

    int x, y;
    float detaX;
    for ( y = 0; y < 1; y++ )
    {
        for ( x = 0; x <= g_foldup.numDiv; x++ )
        {
            if ( x <= curid - 1 )
                detaX = a * x;
            else if ( x >= curid + 2 )
                detaX = a * (x - 2) + 2 * b;
            else
                detaX = a*(curid-1) + b*(x-curid+1);

            // BOTTOM
            g_points[2*x][0] = detaX - sw_half;
            g_points[2*x][1] = y * d - sh_half;
            if ( x % 2 == 0 ) 
                g_points[2*x][2] = 0.0;
            else
                g_points[2*x][2] = -c;
            if ( x >= curid - 1 && x <= curid + 1 )
                g_points[2*x][2] = 0.0;
            g_points[2*x][3] = 1.0 * x;
            g_points[2*x][4] = 0.0;


            // TOP
            g_points[2*x+1][0] = detaX - sw_half;
            g_points[2*x+1][1] = (y+1) * d - sh_half;
            if ( x % 2 == 0 ) 
                g_points[2*x+1][2] = 0.0;
            else
                g_points[2*x+1][2] = -c;
            if ( x >= curid - 1 && x <= curid + 1 )
                g_points[2*x+1][2] = 0.0;
            g_points[2*x+1][3] = 1.0 * x;
            g_points[2*x+1][4] = 1.0;
        }
    }
}

// Our Rendering Is Done Here
void render(void)   
{
    int x, y;
    printf("^^^^^ render ^^^^^^^^^\n");

    if ( g_foldup.isNewClick && g_foldup.curAngPosID != g_foldup.lastAngPosID )
    {
        update_ver_and_tex();
        st_foldup_update_frame();
        printf("^^^^^ update ^^^^^^^^^\n");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();									// Reset The View

    glPushMatrix(); {
        glTranslatef(0.0f,0.0f,g_Ztran);

        glEnable(GL_TEXTURE_2D);

        // 纹理单元与当前的片段颜色混合的算法由 glTexEnvi() 指定!
        // glTexEnvi() 默认使用的是 GL_MODULATE .
        // 即混合算法是: R(dst) = R(src) * R(tex) ...
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g_texid[0]);

        glBegin(GL_TRIANGLE_STRIP);
        // printf("NAL 1X curAngPosID=%d\n", g_foldup.curAngPosID);
        for ( y = 0; y < 1; y++ )
        {
            for ( x = 0; x <= g_foldup.numDiv; x++ )
            {
                glTexCoord2f(g_points[2*x][3], g_points[2*x][4]);
                glVertex3f(g_points[2*x][0], g_points[2*x][1], g_points[2*x][2]);

                glTexCoord2f(g_points[2*x+1][3], g_points[2*x+1][4]);
                glVertex3f(g_points[2*x+1][0], g_points[2*x+1][1], g_points[2*x+1][2]);
            }
        }
        glEnd();
    } glPopMatrix();

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

    if ( ++g_foldup.curFrame >= g_foldup.numFrame )
    {
        g_foldup.isNewClick = 0;
    }
    else
    {
        glutPostRedisplay();
    }

    if (glGetError() != GL_NO_ERROR)
        fprintf(stderr, "GL Error!\n");

    usleep(1000 * 1);

    glutSwapBuffers ( );
}

void resetup_matrix(int w, int h)
{
    if (h == 0) 
        h = 1;

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
    viewW = windowW = w;
    viewH = windowH = h;

    glViewport(0, 0, w, h);

    resetup_matrix(w, h);
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
    m3dLoadPlanev(p, N, -g_Ztran);
#if 0
    double pndotrd = p[0]*dir[0] + p[1]*dir[1] + p[2]*dir[2];
	double point_len = -(p[0]*nx + p[1]*ny + p[2]*nz + p[3]) / pndotrd;

	*worx =  nx + (point_len * dir[0]);
	*wory =  ny + (point_len * dir[1]);
	*worz =  nz + (point_len * dir[2]);
    printf("1w (%f, %f, %f)\n", *worx, *wory, *worz);
#else
    M3DVector3f wor, rayPos;
    m3dLoadVector3(rayPos, nx, ny, nz);
    m3dRayIntersection(p, wor, rayPos, dir);
	*worx =  wor[0];
	*wory =  wor[1];
	*worz =  wor[2];
    printf("2w (%f, %f, %f)\n", wor[0], wor[1], wor[2]);
#endif

    // restore to prev matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void st_foldup_process_button_down(float wx, float wy, float wz)
{
    int i, j;
    float x, y;

    for ( j = 0; j < 1; j++ )
    {
        for ( i = 1; i <= g_foldup.numDiv - 1; i += 2 )
        {
            x = g_points[2*i][0];
            y = g_points[2*i][1];
            if ( x >= wx - 0.09 && x <= wx + 0.09 )
            {
                g_foldup.isNewClick = 1;
                g_foldup.lastAngPosID = g_foldup.curAngPosID;
                g_foldup.curAngPosID = i;
                g_foldup.curFrame = 0;
                printf("******** HIT ******** i = %d\n", i);
                glutPostRedisplay();
                break;
            }
        }
    }
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
        st_foldup_process_button_down(wx, wy, wz);
        // glutPostRedisplay();
    }
}


// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    viewW = windowW = 256 * 5;
    viewH = windowH = 256 * 2;

    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)
    glutInitWindowSize(windowW, windowH);                // Window Size If We Start In Windowed Mode
    glutCreateWindow("NeHe's OpenGL Framework"); // Window Title 
    if (!init()) {                                   // Our Initialization
        return -1;
    }
    glutDisplayFunc(render);                     // Register The Display Function
    glutReshapeFunc(reshape);                    // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
    glutSpecialFunc(special_keys);               // Register Special Keys Handler
	glutMouseFunc(MouseCallback);
    // glutIdleFunc(render);                        // We Do Rendering In Idle Time
    glutMainLoop();                              // Go To GLUT Main Loop
    return 0;
}
