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


typedef struct N3D_GodPos
{
    float mfTarX;
    float mfTarY;
    float mfTarW;
    float mfTarH;

    int   mnFramExpend;
    int   mnDivY;

    N3D_Vertex  *mvVex;
} N3D_GodPos;

typedef struct _RGBIMG {
    GLuint   w;
    GLuint   h;
    GLubyte* data;
} RGBIMG;



#define TEXTURES_NUM 1

bool    g_gamemode;
bool    g_fullscreen;
GLuint	g_texid[TEXTURES_NUM];

N3D_GodPos      g_godPos = {
    0.0,
    -1.0,
    0.3,
    1.0,

    10,
    20,

    NULL
};



/*******************************************************************/
// N3D_GodPos begin //

/* 
 * y = k*x + b
 *
 * y1 = k*x1 + b \       / k = (y2 - y1) / (x2 - x1)
 *                | ==> |
 * y2 = k*x2 + b /       \ b = y1 - k*x1  
 *
 * */
int  N3D_lineConstruct(float x1, float y1, float x2, float y2,
        float *k, float *b)
{
    assert( !(x1 == x2 && y1 == y2) );

    if ( x1 == x2 )
    {
        return 0;
    }

    *k = (y2 - y1) / (x2 - x1);
    *b = y1 - (*k) * x1;

    return 1;
}

/* 
 * y = k*x + b
 *
 * y = k1*x + b1 \      / x = (b2 -b1) / (k1 - k2)
 *                | ==> |
 * y = k2*x + b2 /      \ y = k1*x + b1
 *
 * */
int  N3D_lineInsertPos(float k1, float b1, float k2, float b2,
        float *x, float *y)
{
    if ( k1 == k2 )
    {
        return 0;
    }

    *x = (b2 - b1) / (k1 - k2);
    *y = k1 * (*x) + b1;

    return 1;
}
    
int N3D_godCreate(N3D_GodPos *god)
{
    assert(god != NULL);
    assert(god->mnDivY > 0);
    assert(god->mvVex == NULL);

    god->mvVex = (N3D_Vertex *)calloc(2*(god->mnDivY+1), sizeof(N3D_Vertex));
    if ( god->mvVex == NULL )
        return 0;

    return 1;
}

void N3D_godDestroy(N3D_GodPos *god)
{
    assert(god != NULL && god->mvVex != NULL);

    free(god->mvVex);
    god->mvVex = NULL;
}

void N3D_godInitPos(N3D_GodPos *god)
{
    assert(god != NULL && god->mvVex != NULL && god->mnDivY > 0);

    // Normal Rect: [(-1.0, 1.0), (1.0, -1.0)], size is 2.0
    const float     rectLTX = -1.0;
    const float     rectLTY =  1.0;
    const float     rectBRX =  1.0;
    const float     rectBRY = -1.0;
    int     i;
    float   fCcen = god->mfTarH / (float)god->mnDivY;
    float   fEcen = 2.0 / (float)god->mnDivY;
    float   fTexCen = 1.0 / (float)god->mnDivY;
    N3D_Vertex *pV = god->mvVex;

    for ( i = 0; i <= god->mnDivY; i++ )
    {
        float Cx1 = god->mfTarX;
        float Cy1 = god->mfTarY + fCcen * i;
        float CxL = rectLTX;
        float CyL = rectLTY;
        float CxR = rectBRX;
        float CyR = rectLTY;
        float Ex1 = -1.0;
        float Ey1 = rectBRY + fEcen * i;
        float Ex2 =  0.0;
        float Ey2 = Ey1;
        float Ck, Cb, Ek, Eb, insPosXL, insPosYL, insPosXR, insPosYR;

        if ( Cx1 == CxL ) CxL += 0.00001;
        N3D_lineConstruct(Cx1, Cy1, CxL, CyL, &Ck, &Cb);
        N3D_lineConstruct(Ex1, Ey1, Ex2, Ey2, &Ek, &Eb);
        if ( Ck == Ek ) Ek += 0.00001;
        N3D_lineInsertPos(Ck, Cb, Ek, Eb, &insPosXL, &insPosYL);

        Cx1 += god->mfTarW;
        if ( Cx1 == CxR ) CxR += 0.00001;
        N3D_lineConstruct(Cx1, Cy1, CxR, CyR, &Ck, &Cb);
        if ( Ck == Ek ) Ek += 0.00001;
        N3D_lineInsertPos(Ck, Cb, Ek, Eb, &insPosXR, &insPosYR);

        pV[2*i].fX = insPosXL;
        pV[2*i].fY = insPosYL;
        pV[2*i].fZ = 0.0;
        pV[2*i].fS = 0.0;
        pV[2*i].fT = fTexCen * i;

        pV[2*i+1].fX = insPosXR;
        pV[2*i+1].fY = insPosYR;
        pV[2*i+1].fZ = 0.0;
        pV[2*i+1].fS = 1.0;
        pV[2*i+1].fT = fTexCen * i;
    }
}

void N3D_godDraw(N3D_GodPos *god)
{
    assert(god != NULL && god->mvVex != NULL && god->mnDivY > 0);

    int i;
    N3D_Vertex *pV = god->mvVex;

    glBegin(GL_TRIANGLE_STRIP); {
        for ( i = 0; i <= god->mnDivY; i++ )
        {
            glTexCoord2f(pV[2*i].fS, pV[2*i].fT);
            glVertex3f(pV[2*i].fX, pV[2*i].fY, pV[2*i].fZ);
            glTexCoord2f(pV[2*i+1].fS, pV[2*i+1].fT);
            glVertex3f(pV[2*i+1].fX, pV[2*i+1].fY, pV[2*i+1].fZ);
            // printf("NAL && (%f,%f,%f)\n", pV[i].fX, pV[i].fY, pV[i].fZ);
        }
    } glEnd();
}
// N3D_GodPos end //
/*******************************************************************/




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

    glGenTextures(TEXTURES_NUM, g_texid);
    if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return false;

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    delete img.data;

    return true;
}

// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
    int ret;

    if (!setup_textures()) return false;

    ret = N3D_godCreate(&g_godPos);
    assert( ret != 0 );
    N3D_godInitPos(&g_godPos);

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

    return true;
}

// Our Rendering Is Done Here
void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-2.0f);

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix(); {
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor4f(1.0, 0.0, 1.0, 1.0);
        glScalef(0.5, 0.5, 0.5);
        glRectf(-0.01, -0.01, 0.01, 0.01);
        glRectf(-1.0, -1.0, 1.0, 1.0);
    } glPopMatrix();

    glPushMatrix(); {
        glEnable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPointSize(3);
        glColor4f(1.0, 1.0, 0.0, 1.0);
        glScalef(0.5, 0.5, 0.5);
        N3D_godDraw(&g_godPos);
    } glPopMatrix();

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
        else g_gamemode = false;
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

