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


typedef	struct _tagTDRECT{
	int	left;
	int	top;
	int	right;
	int	bottom;
} TDRECT,*TDPRECT;

typedef struct
{
	float fX;                   /* X coordinate */
	float fY;                   /* Y coordinate */
	float fZ;                   /* Z coordinate */
	float fS;                   /* Texture S coordinate */
	float fT;                   /* Texture T coordinate */

	float fR;
	float fG;
	float fB;
	float fA;                   /* Alpha value */

	float fNX;                  /* Normal X coordinate */
	float fNY;                  /* Normal Y coordinate */
	float fNZ;                  /* Normal Z coordinate */
} GFX3D_Vertex;

typedef struct tmGodLightPosition
{
    float m_CurX; 
    float m_CurY;
    float m_CurZ;
    float m_CurWidth;
    float m_CurHeight;

    float m_TarX; //relative the current picutre the midpoint.left is -, right is +;
    float m_TarY;
    float m_TarZ;
    float m_TarWidth;
    float m_TarHeight;
    int m_FrameNum;//the speed from left to right
    int DEFAULT_X_SUBDIV;
    int DEFAULT_Y_SUBDIV;
    int time;//how many FrameNum in 1 secs.

    int	  gh3Dimage_width;
    int	  gh3Dimage_height;
}GodLightPosition;


// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
    GLuint   w;    // Image's Width
    GLuint   h;    // Image's Height
    GLubyte* data; // Image's Data (Pixels)
} RGBIMG;




#define TEXTURES_NUM 1       


bool    g_gamemode;                  // GLUT GameMode ON/OFF
bool    g_fullscreen;                // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
GLuint	g_texid[TEXTURES_NUM];       // Our Textures' Id List
GodLightPosition godlightposition={
    0.0, 
    0.0,
    0.0, 
    4, 
    4,

    0.0,//-0.7,//-1.5,
    -1.0,
    0.0,

    0.2, 
    0.3, 

    5,
    1,
    10,
    10,

    1024,
    512
};


float Lerp(float first,float second,float coef)
{
	return first+(second-first)*coef;
}

float ComputeLeftData(float y, int m_CurFrame, GodLightPosition position)
{
	float x, a, b, c;
	float x1,y1,x2,y2;
	float m_LeftData = 0.0;
	float coef;
	float curLeft;

	coef = m_CurFrame/(float)position.m_FrameNum; 
	if (coef >= 1.0)
		coef = 1.0;

	curLeft =  Lerp(position.m_CurX - position.m_CurWidth/2.0, position.m_TarX-position.m_TarWidth/2.0, coef);

	x1 = position.m_CurX - position.m_CurWidth/2.0;
	y1 = position.m_CurY + position.m_CurHeight/2.0;
	x2 = curLeft;
	y2 = position.m_CurY - position.m_CurHeight/2.0;

	if(fabs(x2-x1)<0.001)
	{
		m_LeftData  = x1; 
		return m_LeftData;			
	}

	a = (y2-y1)/((x2-x1)*(x2-x1));
	b = -2*x1*a;
	c = y1+x1*x1*a;
	x = (-b-(sqrt(b*b-4*a*(c-(float)y))))/(2*a);
	m_LeftData = x; 
			
    return m_LeftData; 
}

float ComputeRightData(float y, int  m_CurFrame, GodLightPosition position)
{
	float x, a, b, c;
	float x1,y1,x2,y2;

	float m_RightData = 0.0;
	float coef;
	float curRight;

	coef =  m_CurFrame/(float)position.m_FrameNum; 
	if (coef >= 1.0)
		coef = 1.0;

    curRight = Lerp(position.m_CurX + position.m_CurWidth/2.0, position.m_TarX+position.m_TarWidth/2.0, coef);

	x1 = position.m_CurX + position.m_CurWidth/2.0;
	y1 = position.m_CurY + position.m_CurHeight/2.0;
	x2 = curRight;
	y2 = position.m_CurY - position.m_CurHeight/2.0;

	if(fabs(x2-x1)<0.001)
	{
		m_RightData = x1;	
		return m_RightData;			
	}

	a = (y2-y1)/((x2-x1)*(x2-x1));
	b = -2*x1*a;
	c = y1+x1*x1*a;
	
	x = (-b+(sqrt(b*b-4*a*(c-(float)y))))/(2*a);

	m_RightData = x;	

	return m_RightData;
}

void init_indices(GLushort *iptr, GodLightPosition position)
{
	int x, y;
	
	for (y = 0; y < position.DEFAULT_Y_SUBDIV; y++)
	{
		int v = y * (position.DEFAULT_X_SUBDIV + 1);

		for (x = 0; x < position.DEFAULT_X_SUBDIV; x++)
		{
			*iptr++ = v;
			*iptr++ = v + position.DEFAULT_X_SUBDIV + 1;
			*iptr++ = v + position.DEFAULT_X_SUBDIV + 2;

			*iptr++ = v + position.DEFAULT_X_SUBDIV + 2;
			*iptr++ = v + 1;
			*iptr++ = v;
			v++;
		}
	}
}

void update_vertices_downtoup
(	
		GFX3D_Vertex *v, 
		GFX3D_Vertex *tmp_v, 
		int Cur_line, 
		TDRECT *rect, 
		GodLightPosition position
)
{
	int x, y;
	float width = (float)(rect->right - rect->left);
	float height = (float)(rect->bottom - rect->top);
	float x_mul = width/position.gh3Dimage_width/ position.DEFAULT_X_SUBDIV;
	float y_mul = height/position.gh3Dimage_height/ position.DEFAULT_Y_SUBDIV;

	for(y = 0; y <= position.DEFAULT_Y_SUBDIV; y++)
	{
		for(x = 0; x <= position.DEFAULT_X_SUBDIV; x++)
		{
			if( y <= Cur_line)
			{
				v->fX = (tmp_v + (position.DEFAULT_X_SUBDIV+1)*(position.DEFAULT_Y_SUBDIV-Cur_line))->fX; 
				v->fY = (tmp_v + (position.DEFAULT_X_SUBDIV+1)*(position.DEFAULT_Y_SUBDIV-Cur_line))->fY;
				v->fZ = 0.0;
				v->fS = x * x_mul;
				v->fT = y * y_mul;
				v++;
				tmp_v++;
			}
			else
			{
				v->fX = (v - (position.DEFAULT_X_SUBDIV+1)) -> fX;
				v->fY = (v - (position.DEFAULT_X_SUBDIV+1)) -> fY;// - (height/position.DEFAULT_Y_SUBDIV);
				v->fZ = 0.0;
				v->fS = x * x_mul;
				v->fT = y * y_mul;
				v++;
			}
		}
	}
}
void init_vertices(GFX3D_Vertex *v, int  m_CurFrame, TDRECT *rect, GodLightPosition position)
{
	int x, y;
	float curLeft;
	float coef;
	float left, right;
	float width = (float)(rect->right - rect->left);
	float height = (float)(rect->bottom - rect->top);

	float x_mul = width/position.gh3Dimage_width/ position.DEFAULT_X_SUBDIV;
	float y_mul = height/position.gh3Dimage_height/ position.DEFAULT_Y_SUBDIV;

	for (y = 0; y <= position.DEFAULT_Y_SUBDIV; y++)
	{
		coef = (float)y/position.DEFAULT_Y_SUBDIV;
		curLeft  =  Lerp(position.m_CurY + position.m_CurHeight/2.0, position.m_CurY - position.m_CurHeight/2.0, coef);

		left = ComputeLeftData(curLeft, m_CurFrame, position)*(float)width/720.0;
		right = ComputeRightData(curLeft, m_CurFrame, position)*(float)width/720.0;
//		printf("~~~~~~~~~~~``\nright = %f\n", right);

		for(x = 0; x <= position.DEFAULT_X_SUBDIV; x++)
		{
			v->fX = Lerp(left,right,(float)x/position.DEFAULT_X_SUBDIV)/2; 
			v->fY = curLeft * (float)height/720.0/2.0;
			v->fZ = 0.0;
			v->fS =  x * x_mul;
			v->fT = y * y_mul;
			v++;
		}
	}
}

void init_vertices_downtoup(GFX3D_Vertex *tmp_v, int  m_CurFrame, TDRECT *rect, GodLightPosition position)
{
	int x, y;
	float curLeft;
	float coef;
	float left, right;
	float width = (float)(rect->right - rect->left);
	float height = (float)(rect->bottom - rect->top);

	float x_mul = width/position.gh3Dimage_width/ position.DEFAULT_X_SUBDIV;
	float y_mul = height/position.gh3Dimage_height/ position.DEFAULT_Y_SUBDIV;

	for (y = 0; y <= position.DEFAULT_Y_SUBDIV; y++)
	{
		coef = (float)y/position.DEFAULT_Y_SUBDIV;
		curLeft  =  (float)Lerp(position.m_CurY + position.m_CurHeight/2.0, position.m_CurY - position.m_CurHeight/2.0, coef);
		//curLeft  =  Lerp(position.m_CurY + width/720.0/2.0, position.m_CurY - position.m_CurHeight/2.0, coef);
		left = ComputeLeftData(curLeft, m_CurFrame, position)*(float)width/720.0;
		right = ComputeRightData(curLeft, m_CurFrame, position)*(float)width/720.0;

		for(x = 0; x <= position.DEFAULT_X_SUBDIV; x++)
		{
			tmp_v->fX = Lerp(left,right,(float)x/position.DEFAULT_X_SUBDIV)/2; 
			tmp_v->fY = curLeft * (float)height/720.0/2.0;
			tmp_v->fZ = 0.0;
			tmp_v->fS =  x * x_mul;
			tmp_v->fT = y * y_mul;
			tmp_v++;
		}
	}
}

void GodLight_DOWNTOUP()
{
    int	i;
    TDRECT rect = {100, 100, 500, 500};
    int vertex_index_len = 6 * godlightposition.DEFAULT_X_SUBDIV * godlightposition.DEFAULT_Y_SUBDIV;
    int num_vertices = (godlightposition.DEFAULT_X_SUBDIV + 1) * (godlightposition.DEFAULT_Y_SUBDIV + 1);

    /* GFX3D_Rect ImageRect = {
        0,
        0,
        gpCurLightView->Rect.right  - gpCurLightView->Rect.left,
        gpCurLightView->Rect.bottom - gpCurLightView->Rect.top
    };

    TDRECT ShowRect = {
        gpCurLightView->Rect.left,
        gpCurLightView->Rect.top,
        gpCurLightView->Rect.right  - gpCurLightView->Rect.left,
        gpCurLightView->Rect.bottom - gpCurLightView->Rect.top
    }; */

    GLushort *indices = (GLushort *)calloc(vertex_index_len, sizeof(GLushort));
    GFX3D_Vertex *vertices = (GFX3D_Vertex *)calloc(num_vertices, sizeof(GFX3D_Vertex));
    GFX3D_Vertex *tmp_vertices = (GFX3D_Vertex *)calloc(num_vertices, sizeof(GFX3D_Vertex));

    /* gh3DImage[TextureID] = GFX3D_Image_Create(ghDisplay, 
            NULL, 
            godlightposition.gh3Dimage_width,
            godlightposition.gh3Dimage_height,
            TextureID,
            0);

    GFX3D_Image_Update( gh3DImage[TextureID],
            ghScrMemDC1, 
            gpCurLightView->Rect.left,
            gpCurLightView->Rect.top,
            gpCurLightView->Rect.right  - gpCurLightView->Rect.left,
            gpCurLightView->Rect.bottom - gpCurLightView->Rect.top
            );

    gh3DObject[TextureID] = GFX3D_Object_Create( ghDisplay, gh3DImage[TextureID],\
            &ImageRect, GFX3D_ObjectType_Mesh, vertices, 1 );

    if(!gh3DObject[TextureID])
    {
        TDDebug("\nCreate 3DObject Error...\n");
    } */

    // GFX3D_Object_AddPrimitivesToMesh( gh3DObject[TextureID], GFX3D_PrimitiveType_List, indices, vertex_index_len );
    // GFX3D_Object_Move( gh3DObject[TextureID], 0, gpCurLightView->Rect.left, gpCurLightView->Rect.top, 0 );
    // init_vertices_downtoup(tmp_vertices, godlightposition.m_FrameNum, &gpCurLightView->Rect, godlightposition);
    init_vertices_downtoup(tmp_vertices, godlightposition.m_FrameNum, &rect, godlightposition);

    init_indices(indices, godlightposition);

    for(i = 0; i <= 1+godlightposition.DEFAULT_Y_SUBDIV+godlightposition.m_FrameNum; i++)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,-2.0f);
        glBindTexture(GL_TEXTURE_2D, g_texid[0]);
        glDisable(GL_TEXTURE_2D);                           // Enable Texture Mapping
        glColor4f(1.0, 0.0, 1.0, 1.0);

        if(i <= godlightposition.DEFAULT_Y_SUBDIV)
        {
            // update_vertices_downtoup(vertices, tmp_vertices, i, &gpCurLightView->Rect, godlightposition);
            update_vertices_downtoup(vertices, tmp_vertices, i, &rect, godlightposition);
        }
        else
        {
            // init_vertices(vertices, godlightposition.m_FrameNum-i+godlightposition.DEFAULT_Y_SUBDIV+1, &gpCurLightView->Rect, godlightposition);
            init_vertices(vertices, godlightposition.m_FrameNum-i+godlightposition.DEFAULT_Y_SUBDIV+1, &rect, godlightposition);
        }

        glVertexPointer(3, GL_FLOAT, sizeof(GFX3D_Vertex), (float *)vertices);
        // glTexCoordPointer(2, GL_FLOAT, sizeof(GFX3D_Vertex), &(vertices[0].fS));
        glDrawElements(GL_TRIANGLES, vertex_index_len, GL_UNSIGNED_SHORT, indices);

        usleep( 100 * 1000);
        glFlush();
        glutSwapBuffers();
    }
    printf("NALL ... \n");

    free(indices);
    free(vertices);
    free(tmp_vertices);

    return;
}


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

    glGenTextures(TEXTURES_NUM, g_texid);
    if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return false;

    glBindTexture(GL_TEXTURE_2D, g_texid[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

    delete img.data;

    return true;
}

// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
    if (!setup_textures()) return false;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
    glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			   // Black Background (CHANGED)
    glClearDepth(1.0f);								   // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPolygonMode(GL_BACK, GL_FILL);                   // Back Face Is Solid (NEW)
    glPolygonMode(GL_FRONT, GL_FILL);                  // Front Face Is Made Of Lines (NEW)
    
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
    glDisable(GL_TEXTURE_2D);                           // Enable Texture Mapping
    glColor4f(1.0, 0.0, 1.0, 1.0);
    glPushMatrix(); {
        glRectf(-0.5, -0.5, 0.5, 0.5);
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

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
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
    glutDisplayFunc(GodLight_DOWNTOUP);                     // Register The Display Function
    glutReshapeFunc(reshape);                    // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
    glutSpecialFunc(special_keys);               // Register Special Keys Handler
    // glutIdleFunc(render);                        // We Do Rendering In Idle Time
    glutMainLoop();                              // Go To GLUT Main Loop
    return 0;
}
