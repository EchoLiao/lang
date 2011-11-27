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


#define TEXTURES_NUM 1       // We Have 1 Texture 

// 被细分为90个小矩形
#define X_NUMDIV    90
#define Y_NUMDIV    1


// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
    GLuint   w;    // Image's Width
    GLuint   h;    // Image's Height
    GLubyte* data; // Image's Data (Pixels)
} RGBIMG;


// Global Variables

bool g_stop = 0;
bool g_isopenning = true;

float   g_points[X_NUMDIV * 4][5];   // The Array For The Points On The Grid Of Our "Wave"

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
    delete img.data;
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
    int xnumFramePerCircle = X_NUMDIV / ncircle;

    memset(g_points, 0, X_NUMDIV*4*5*sizeof(float));
    for(int y = 0; y < Y_NUMDIV; y++)
    {
        for (int x = 0; x < X_NUMDIV; x++) 
        {
            if ( g_x_curframe > x ) // 平面部分
            {
                // ([-1.x, 1.x], [-1.0, 1.0])

                g_points[4*x][0] = x * xvstep - ow_r_half;
                g_points[4*x][1] = y * yvstep - oh_r_half;
                g_points[4*x][2] = -r;
                g_points[4*x][3] = x * xtstep;
                g_points[4*x][4] = y * ytstep;

                g_points[4*x+1][0] = x * xvstep - ow_r_half;
                g_points[4*x+1][1] = (y+1) * yvstep - oh_r_half;
                g_points[4*x+1][2] = -r;
                g_points[4*x+1][3] = x * xtstep;
                g_points[4*x+1][4] = (y+1) * ytstep;

                g_points[4*x+2][0] = (x+1) * xvstep - ow_r_half;
                g_points[4*x+2][1] = (y+1) * yvstep - oh_r_half;
                g_points[4*x+2][2] = -r;
                g_points[4*x+2][3] = (x+1) * xtstep;
                g_points[4*x+2][4] = (y+1) * ytstep;

                g_points[4*x+3][0] = (x+1) * xvstep - ow_r_half;
                g_points[4*x+3][1] = y * yvstep - oh_r_half;
                g_points[4*x+3][2] = -r;
                g_points[4*x+3][3] = (x+1) * xtstep;
                g_points[4*x+3][4] = y * ytstep;
            }                    
            else // 卷轴部分
            {
                // 超过一圈的不再贴到柱面上
                if ( x - g_x_curframe >= xnumFramePerCircle )
                    break;
                float deta = (float)(x - g_x_curframe);
                float detab = (float)(x + 1 - g_x_curframe);
                g_points[4*x][0] = (float)sin(deta*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[4*x][1] = y*yvstep-oh_r_half;
                g_points[4*x][2] = (float)cos(deta*theta*ang2rad+rad180)*r;
                g_points[4*x][3] = x * xtstep;
                g_points[4*x][4] = y * ytstep;

                g_points[4*x+1][0] = (float)sin(deta*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[4*x+1][1] = (y+1)*yvstep-oh_r_half;
                g_points[4*x+1][2] = (float)cos(deta*theta*ang2rad+rad180)*r;
                g_points[4*x+1][3] = x * xtstep;
                g_points[4*x+1][4] = (y+1) * ytstep;

                g_points[4*x+2][0] = (float)sin(detab*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[4*x+2][1] = (y+1)*yvstep-oh_r_half;
                g_points[4*x+2][2] = (float)cos(detab*theta*ang2rad+rad180)*r;
                g_points[4*x+2][3] = (x+1) * xtstep;
                g_points[4*x+2][4] = (y+1) * ytstep;

                g_points[4*x+3][0] = (float)sin(detab*theta*ang2rad+rad180)*r - ow_r_half + g_x_curframe*xvstep;
                g_points[4*x+3][1] = y*yvstep-oh_r_half;
                g_points[4*x+3][2] = (float)cos(detab*theta*ang2rad+rad180)*r;
                g_points[4*x+3][3] = (x+1) * xtstep;
                g_points[4*x+3][4] = y * ytstep;
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

    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);

    update_ver_and_tex();

    // [(<<G:2>> P107)]
    glBegin(GL_QUADS);
    for (y = 0; y < Y_NUMDIV; y++) {
        for (x = 0; x < X_NUMDIV; x++) {

            glTexCoord2f(g_points[4*x][3], g_points[4*x][4]);
            glVertex3f(g_points[4*x][0], g_points[4*x][1], g_points[4*x][2]);

            glTexCoord2f(g_points[4*x+1][3], g_points[4*x+1][4]);
            glVertex3f(g_points[4*x+1][0], g_points[4*x+1][1], g_points[4*x+1][2]);

            glTexCoord2f(g_points[4*x+2][3], g_points[4*x+2][4]);
            glVertex3f(g_points[4*x+2][0], g_points[4*x+2][1], g_points[4*x+2][2]);

            glTexCoord2f(g_points[4*x+3][3], g_points[4*x+3][4]);
            glVertex3f(g_points[4*x+3][0], g_points[4*x+3][1], g_points[4*x+3][2]);

        }
    }
    glEnd();
    glPopMatrix();

    printf("NAL XX g_x_curframe=%d ...\n", g_x_curframe);
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
    usleep(1000 * 200);

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
