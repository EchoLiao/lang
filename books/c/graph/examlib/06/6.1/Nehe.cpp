/*
 * ===========================================================================
 *
 *       Filename:  Nehe.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月23日 21时00分27秒
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



#include <string.h>
#include <unistd.h>
#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <GL/glut.h>         // The GL Utility Toolkit (GLUT) Header

#include "Texture.h"
#include "Vector3D.h"


//在此处定义全局变量:
// GL_Window*	OGL_window;
// Keys*		OGL_keys;

//在此处定义用户变量:
GLuint base;

GLuint axisTex,pyramidTex;
GLUquadricObj *quadric;

// 17个锥体的位置
GLfloat pos[17][3] = {{0.0f, 0.0f, 0.0f},
					{ 20.0f, 0.0f, 0.0f},{ 15.0f, 0.0f, 0.0f},{ 10.0f, 0.0f, 0.0f},{ 5.0f, 0.0f, 0.0f},
					{-20.0f, 0.0f, 0.0f},{-15.0f, 0.0f, 0.0f},{-10.0f, 0.0f, 0.0f},{-5.0f, 0.0f, 0.0f},
					{ 0.0f, 0.0f, 20.0f},{ 0.0f, 0.0f, 15.0f},{ 0.0f, 0.0f, 10.0f},{ 0.0f, 0.0f, 5.0f},
					{ 0.0f, 0.0f,-20.0f},{ 0.0f, 0.0f,-15.0f},{ 0.0f, 0.0f,-10.0f},{ 0.0f, 0.0f,-5.0f}};

CVector3 specalPoint[8];											// 8个视点运动路径的节点
GLfloat speed = 0.2f;												// 视点运动的速度
GLfloat tex_y;														// 纹理的Y方向位置
BOOL	Pause = FALSE;												// 视点运动是否暂停
BOOL	spaceDown;													// 空格键是否已经按下




// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
	// 在此处初始化绘制场景
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);							// 清屏为黑色
	glClearDepth(1.0f);												// 设置深度缓存
	glDepthFunc(GL_LEQUAL);											// 选择深度测试方式
	glEnable(GL_DEPTH_TEST);										// 开启深度测试
	glShadeModel(GL_SMOOTH);										// 阴暗处理采用平滑方式
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// 最精细的透视计算	

	// 建立字体
	// BuildFontGL();

	// 载入纹理
	BuildTexture((char*)"data/Axis.jpg", axisTex);
	BuildTexture((char*)"data/Reflection.JPG", pyramidTex);
	
	// 建立二次曲面对象
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	// 建立8个gluLookAt()视点要经过的节点
	specalPoint[0].Set(  0.0f,  0.0f, 15.0f);
	specalPoint[1].Set( 30.0f, 30.0f, 30.0f);
	specalPoint[2].Set( 15.0f,  0.0f, 0.0f);
	specalPoint[3].Set( 30.0f, 30.0f,-30.0f);
	specalPoint[4].Set(  0.0f,  0.0f,-15.0f);
	specalPoint[5].Set(-30.0f, 30.0f,-30.0f);
	specalPoint[6].Set(-15.0f,  0.0f,  0.0f);
	specalPoint[7].Set(-30.0f, 30.0f, 30.0f);

	return TRUE;
    
    return true;
}

void MoveCameraPassSpecalPoint(void)
{
	CVector3 direction;												// 保存视点的运动方向
	static CVector3 currentPos	= specalPoint[0];					// 保存当前视点的位置
	static CVector3 targetPoint	= specalPoint[1];					// 保存当前视点的移动目标点
	static int TargetPointId = 1;									// 保存当前视点的移动目标点序号

	// 更新视点位置
	if (!Pause)														// 如果没有暂停视点的移动
	{
		direction = targetPoint - currentPos;						// 得到视点的运动方向
		if ( direction.GetLength() > speed)							// 如果视点不在目标点的周围的范围内
		{
			currentPos = currentPos + direction.GetNormalized()*speed;// 沿视点的运动方向更新视点的位置
		}
		else														// 否则
		{
			currentPos = targetPoint;								// 将视点放置在目标点上
			TargetPointId = (TargetPointId + 1) % 8;				// 更新目标点序号
			targetPoint = specalPoint[TargetPointId];				// 更新目标点值
		}
	}

	// // 在屏幕上显示文字和一些变量
	// glDisable(GL_TEXTURE_2D);
	// glRasterPos3f(-0.54f, 0.4f, -1.0f);
	// glPrint("Move The Camera Pass The Every Point!");
	// glRasterPos3f(-0.54f, 0.37f, -1.0f);
	// glPrint("gluLookAt(");
	// glPrint("%.3f,%.3f,%.3f,", currentPos.x, currentPos.y, currentPos.z);
	// glPrint(" 0,0,0, 0,1,0);");
	// glRasterPos3f(-0.54f, 0.34f, -1.0f);
	// glPrint("Target Point:%.3f,%.3f,%.3f", targetPoint.x, targetPoint.y, targetPoint.z);
	// glPrint("(The %dth Point)", TargetPointId);
	// glRasterPos3f(-0.54f, 0.31f, -1.0f);
	// if (Pause)
	// {
	//     glPrint("Press Space Bar TO Continue Move Camera.");
	// }
	// else
	// {
	//     glPrint("Press Space Bar TO Stop Move Camera.");
	// }
	// glColor3f(1.0f, 0.0f, 0.0f);
	// glRasterPos3f(-0.54f, 0.28f, -1.0f);
	// glPrint("Axis X:Red");
	// glColor3f(0.0f, 1.0f, 0.0f);
	// glRasterPos3f(-0.34f, 0.28f, -1.0f);
	// glPrint("Axis Y:Green");
	// glColor3f(0.0f, 0.0f, 1.0f);
	// glRasterPos3f(-0.14f, 0.28f, -1.0f);
	// glPrint("Axis Z:Blue");

	// 使用gluLookAt建立观察矩阵
	gluLookAt(currentPos.x, currentPos.y, currentPos.z, currentPos.y, 0.0f, 0.0f, 1.0f, currentPos.y, 0.0f);
}

void RenderWorld(void)
{
	// 绘制平面网格
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.5f, 0.0f);
	glBegin(GL_LINES);
	for(int i=-30; i<=30; i++)
	{
		glVertex3i( 30, 0, i);
		glVertex3i(-30, 0, i);

		glVertex3i( i, 0, 30);
		glVertex3i( i, 0,-30);
	}
	glEnd();

	// 开启混合、关闭深度测试
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE );
	glEnable(GL_TEXTURE_2D);

	// 绘制三维坐标系轴，调整每个圆柱体的位置，使纹理运动的方向正好是轴的正方向
	glBindTexture(GL_TEXTURE_2D, axisTex);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.0f, tex_y/4.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	// 绘制X轴
	glPushMatrix();
		glTranslatef(-30.0f, 0.0f, 0.0f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.1, 0.1, 60, 8, 32);
	glPopMatrix();
	// 绘制Y轴
	glPushMatrix();
		glTranslatef(0.0f, -30.0f, 0.0f);
		glRotatef(90, -1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.1, 0.1, 60, 8, 32);
	glPopMatrix();
	// 绘制Z轴
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -30.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		gluCylinder(quadric, 0.1, 0.1, 60.0, 8, 32);
	glPopMatrix();
	// 重置纹理矩阵
	glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	// 绘制5个锥体
	glBindTexture(GL_TEXTURE_2D, pyramidTex);

	// 在纹理矩阵中移动纹理
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
		glTranslatef(0.0f, tex_y, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	for (int i=0; i<17; i++)
	{
		glPushMatrix();
		glTranslatef(pos[i][0], pos[i][1], pos[i][2]);
		// 中间锥体、X,Y方向锥体设置不同颜色
		if (i == 0)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glScalef(1.0f, 2.0f, 1.0f);
		}
		else if (i <= 8)
			glColor3f(1.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 1.0f);
		// 绘制锥体（顶点向上的锥体）
		glBegin(GL_TRIANGLE_FAN);
        // glBegin(GL_TRIANGLES);
			glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.0f, 2.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, 0.0f, 0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, 0.0f,-0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.0f,-0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.0f, 0.5f);
		glEnd();
		// 绘制锥体（顶点向下的锥体）
		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, 1.0f, 0.25f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.25f, 1.0f, 0.25f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.25f, 1.0f,-0.25f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.25f, 1.0f,-0.25f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.25f, 1.0f, 0.25f);
		glEnd();
		glPopMatrix();
	}
	// 重置纹理矩阵
	glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	// 开启深度测试、关闭混合
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	tex_y -= 0.01f;
}


// Our Rendering Is Done Here
void render(void)   
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// 清除场景和深度缓存
	glLoadIdentity();													// 重置当前矩阵
	//在此处添加代码进行绘制:

	MoveCameraPassSpecalPoint();										// 移动并设定视点
	RenderWorld();														// 绘制场景

	glFlush ();															// 刷新GL命令队列
    

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
            // // We Can Switch Between Windowed Mode And Fullscreen Mode Only
            // if (!g_gamemode) {
            //     g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
            //     if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
            //     else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
            // }
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
    // g_fullscreen = false; 
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    ask_gamemode();                                  // Ask For Fullscreen Mode
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)
    // if (g_gamemode) {
    //     glutGameModeString("640x480:16");            // Select The 640x480 In 16bpp Mode
    //     if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
    //         glutEnterGameMode();                     // Enter Full Screen
    //     else g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
    // }
    // if (!g_gamemode) {
        glutInitWindowSize(342*2, 256*2);                // Window Size If We Start In Windowed Mode
        glutCreateWindow("NeHe's OpenGL Framework"); // Window Title 
    // }
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

