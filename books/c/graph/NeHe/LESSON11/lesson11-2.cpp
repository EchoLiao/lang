/*
   NeHe (nehe.gamedev.net) OpenGL tutorial series
   GLUT port.in 2001 by milix (milix_gr@hotmail.com)
   Most comments are from the original tutorials found in NeHe.
   For VC++ users, create a Win32 Console project and link 
   the program with glut32.lib, glu32.lib, opengl32.lib
   */

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header
#include "bmprw.h"


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

float   g_points[NUM_VERTEXES][NUM_PER_VERTEXE];   // The Array For The Points On The Grid Of Our "Wave"

int g_xnumFramePerCircle;

bool    g_gamemode;                  // GLUT GameMode ON/OFF
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

// Loads A RGB Raw Image From A Disk File And Updates Our Image Reference
// Returns true On Success, False On Fail.
bool load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
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
    refimg->data = new GLubyte [256*256*3];
    if (refimg->data == NULL) 
        return false;

    memset(refimg->data, 0, w*h*3*sizeof(GLubyte));
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
    }
    return true;
#endif
}

void line_equation(float x1, float y1, float x2, float y2, float *k, float *b)
{
    *k = (y2 - y1) / (x2 - x1);
    *b = (x2*y1 - x1*y2) / (x2 - x1);
}

void n2d_transf(float *xout, float *yout, float x, float y, 
        float a, float tx, float ty)
{
    float sinA = sinf(a);
    float cosA = cosf(a);
    *xout = x*cosA - y*sinA + tx*cosA - ty*sinA;
    *yout = x*sinA + y*cosA + tx*sinA + ty*cosA;
}

bool create_trian_rgb_image(int win, int hin, RGBIMG *imgin,
        int *wout, int *hout, RGBIMG *imgout)
{
    float w, h;
    float Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
    float ABk, ABb, BCk, BCb, CDk, CDb, ADk, ADb;
    float sinTheta, cosTheta, tanTheta;
    float i, j;

    w = sqrtf((float)win*win + (float)hin*hin);
    h = (float) 2.0*win*hin / (float)w;

    Ax = 0.0;
    Ay = h / 2.0;
    Bx = sqrtf(hin*hin - (h/2.0)*(h/2.0));
    By = h;
    Cx = w;
    Cy = h / 2.0;
    Dx = sqrtf(win*win - (h/2.0)*(h/2.0));
    Dy = 0.0;
    line_equation(Ax, Ay, Bx, By, &ABk, &ABb);
    line_equation(Bx, By, Cx, Cy, &BCk, &BCb);
    line_equation(Cx, Cy, Dx, Dy, &CDk, &CDb);
    line_equation(Ax, Ay, Dx, Dy, &ADk, &ADb);
    sinTheta = - hin / w;
    cosTheta = win / w;
    tanTheta = hin / win;

    *wout = (int)w + 1;
    // OpenGL 要求的图像的数据的行必须是以4字节对齐的!! MMMMM
    int linebs = (((*wout * 3 + 3)>>2)<<2);
    *hout = (int)h + 1;
    imgout->data = new GLubyte [linebs * *hout];
    if (imgout->data == NULL) 
        return false;

    imgout->w = *wout;
    imgout->h = *hout;
    memset(imgout->data, 0, linebs * *hout * sizeof(GLubyte));

#if 1
    // 
    // 变换公式: 	
    // 
    // 数学坐标系统 =》当前坐标系统:   
    // X  =   MX * A11 + MY * A12 ＋ DX ; 
    // Y  =   MX * A21 ＋ MY * A22 + DY
    // 当前坐标系统 =》数学坐标系统:   
    // MX  =   (X - DX) * A11  -  (Y -DY) * A12  ; 
    // MY  =   (X - DX) * A21 -  (Y -DY) * A22 
    //
    // 其中: A11 = COSA  A12 = SINA  A21 = -SINA   A22 = COSA 
    // DX,DY =  -数学坐标原点在当前坐标系统的XY
    //
    for ( int y = 0; y < hin; y++ )
    {
        for ( int x = 0; x < win; x++ )
        {
            GLubyte *pbout = imgout->data;
            GLubyte *pbin = imgin->data;
            // if ( (float) y - ABk*x - ABb <= 0.0
            //         && (float) y - BCk*x - BCb <= 0.0
            //         && (float) y - CDk*x - CDb >= 0.0
            //         && (float) y - ADk*x - ADb >= 0.0 )
            {
                n2d_transf(&i, &j, x, y, -45.0*(3.141592654/180.0), -128.0, 128.0);
                assert(i >= -10.0);
                assert(j >= 0.0);
                // i = (x+0.0)*cosTheta - (y+(win/2.0))*sinTheta;
                // j = (x+0.0)*sinTheta + (y+(win/2.0))*cosTheta;

                // i = (y-h/2.0)*sinTheta + x*cosTheta;
                // j = (y-h/2.0)*cosTheta - x*sinTheta;

                // j = x*cosTheta - (y-h/2.0)*sinTheta;
                // i = x*sinTheta + (y-h/2.0)*cosTheta;

                // i = (x-0.0)*cosTheta - (y-h/2.0)*sinTheta;
                // j = (x-0.0)*(-sinTheta) - (y-h/2.0)*cosTheta;
                // i = x*cosTheta + y*sinTheta + 0.0;
                // j = x*(-sinTheta) + y*cosTheta - h/2.0;
                //
                // j = ((x*tanTheta+(y-h/2.0))/cosTheta) / (1+tanTheta*tanTheta);
                // i = x/cosTheta - j*tanTheta;
                // j = ((y-h/2.0)+x*tanTheta/cosTheta) / (1+tanTheta*tanTheta);
                // i = x/cosTheta - j*tanTheta;
//                 pbin += ((int)j) * win * 3 + ((int)i) * 3;
// #if 1
//                 int ipTmp = (int) ((imgin->data + win*hin*3) - pbin);
//                 assert(ipTmp >= 0);
// #endif
                pbin += y * win * 3 + x * 3;
                pbout += ((int)j) * linebs + ((int)i) * 3;
                memcpy(pbout, pbin, 3*sizeof(GLubyte));
            }
            // else
            // {
            //     memset(pbout, 0, 3*sizeof(GLubyte));
            // }
        }
    }
#else
    GLubyte *pbin = imgin->data;
    GLubyte *pbout = imgout->data;
    for ( int j = 0; j < hin; j++ )
    {
        for ( int i = 0; i < win; i++ )
        {

        }
    }
#endif

    return true;

}

void test_img(RGBIMG *img)
{
#if 0
    for ( int y = img->h - 1; y >= 0; y-- )
    {
        GLubyte *p = img->data + y * img->w * 3;
        for ( int x = 0; x < (int)img->h; x++ )
        {
            if ( *p == 255 )
                printf("r");
            else if ( *(p+1) == 255 )
                printf("g");
            else
                printf("0");

            p += 3;
        }
        printf("\n");
    }
#else
    // NALWriteBMPFile("nal-1.bmp", img->data, img->w, img->h, 24, NALTRUE);
#endif
}

// Setup Our Textures. Returns true On Success, false On Fail
bool setup_textures()
{
    RGBIMG img, img2;
    int w2, h2;

    // Create The Textures' Id List
    glGenTextures(TEXTURES_NUM, g_texid);          
    // Load The Image From A Disk File
    if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return false;

    if ( ! create_trian_rgb_image(256, 256, &img, &w2, &h2, &img2) )
        return false;

    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
#if 0
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
    tw = iw = img.w;
    th = ih = img.h;
#else
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img2.w, img2.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img2.data);
    tw = iw = img2.w;
    th = ih = img2.h;
#endif

    test_img(&img2);

    // Finished With Our Image, Free The Allocated Data
    delete img.data;
    delete img2.data;
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

    return true;
}

void update_ver_and_tex()
{
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
    for(int y = 0; y < Y_NUMDIV; y++)
    {
        for (int x = 0; x <= X_NUMDIV; x++) 
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
}

// Our Rendering Is Done Here
void render(void)   
{
    int x, y;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();									// Reset The View
    glTranslatef(0.0f,0.0f,-2.02f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);

#if 1
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

#else
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0,  1.0, 0.0);

    glTexCoord2f(1.0, 1.0);
    glVertex3f( 1.0,  1.0, 0.0);

    glTexCoord2f(1.0, 0.0);
    glVertex3f( 1.0, -1.0, 0.0);
    glEnd();
#endif

    glPopMatrix();

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
    usleep(1000 * 50);

    // Swap The Buffers To Become Our Rendering Visible
    glutSwapBuffers ( );
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    glLoadIdentity();                // Reset The Projection Matrix
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (h == 0) h = 1;

    glFrustum(-(float)w/h/2.0, (float)w/h/2.0, -0.5, 0.5, 1.0, 100);
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
            if (!g_gamemode) {
                g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
                if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
                else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
            }
            break;
        default:
            break;
    }
}

// Ask The User If He Wish To Enter GameMode Or Not
void ask_gamemode()
{
    // int answer;
    // // Use Windows MessageBox To Ask The User For Game Or Windowed Mode
    // answer = MessageBox(NULL, "Do you want to enter game mode?", "Question",
    //                     MB_ICONQUESTION | MB_YESNO);
    // g_gamemode = (answer == IDYES);
    // If Not Game Mode Selected, Use Windowed Mode (User Can Change That With F1)
    g_fullscreen = false; 
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    ask_gamemode();                                  // Ask For Fullscreen Mode
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)
    if (g_gamemode) {
        glutGameModeString("640x480:16");            // Select The 640x480 In 16bpp Mode
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();                     // Enter Full Screen
        else g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
    }
    if (!g_gamemode) {
        glutInitWindowSize(342*2, 256*2);                // Window Size If We Start In Windowed Mode
        glutCreateWindow("NeHe's OpenGL Framework"); // Window Title 
    }
    if (!init()) {                                   // Our Initialization
        // MessageBox(NULL,"Cannot initialize textures.","Error",MB_ICONSTOP);
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
