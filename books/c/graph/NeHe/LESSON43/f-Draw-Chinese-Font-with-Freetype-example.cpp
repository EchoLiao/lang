
// opengl学习从头开始(笔记12 freetype中文显示)

//   2011-06-26 23:27:24|  分类:  Opengl |  标签:  |字号大中小 订阅
//
/**
  这里使用到的是opengl 和 glut 进行编写学习的笔记, 一个简单的出口
  使用到的开发库有 linux (gl glu glut) windows(opengl32.lib glu32.lib glut32.lib)

  实现:使用freetype实现中文的显示

  这里我们使用黑体(simhei.ttf),来作为我们使用的字体
  如果是在ubuntu上使用新立得软件管理器下载的freetype
  不知道怎么找.h .a 文件在那个文件夹的话, 可以点击
  安装的文件如libfreetype6-dev 看它属性,看已安装的文件就
  可以看到你要的文件究竟在哪里多了.

  目前这个成功在ubuntu上显示中文window上的没有测试, 需要注意的地方
  1.中文必须是utf8的, (内容是把utf8的字符转换成unicode,还有一个文字的utf8长度要小于5个字节)
  2.字符串长度要在256个字符内, 也就是说如果是utf8的中文的话就算3个字符了


// 实现思路:
// 这个是根据nehe 的freetype 那一课的基础上进行修改实现的
// 这里有几个提醒的是
// freetype 的 FT_Get_Char_Index(face,charcode);
// 这里的charcode 是unicode 编码, 在程序里面我对字符从utf8转换成unicode 才传进去
// 这样才获得了字符数据
//
// 我是先把所有的单字符的英文字符, 数字, 等单字符创建显示列表128(这个是nehe已经实现的 ; ))
// 到输入字符串后, 如果有单字符那么就直接从128个显示列表中获取,如果是中文或多字符
// 那么就从ttf中查找, 获取数据后创建显示列表, 并保存, 已备如果下次这个字符调用的时候
// 不用在创建,而直接在列表里面查询出来使用
//
// (这样就有可能带来, 2万多个字, 如果都调用到的话就会有2万多个的显示列表, 不知道会
// 带来多大的影响,目前也只有暂时这样了)

例子:

问题:
优化部分, 这里没有进行太多的优化



*/

#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "ft.h"
#include "mft.h"

#ifdef _MSC_VER

#endif

#ifdef __GNUC__
#include <unistd.h>
#endif

using namespace std;
using namespace freetype;

float g_angle  = 0.0f;

//freetype::font_data our_font;
MFont_Data our_font;
/////////////////////////////////////////////////////////////////////////////
//===========================OpenGL 部分=========================
// 初始化opengl的一些参数
void init()
{
    // 设置灯光
    GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f}; // 环境光参数
    GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f}; // 漫反射光参数
    GLfloat LightPosition[] = { 2.0f, 8.0f, 1.0f, 1.0f}; // 光源位置

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // 设置环境光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // 设置漫反射光
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // 设置位置

    glEnable(GL_LIGHT1);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 清理背景颜色为蓝色
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // 使用圆点而不使用方点
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);    // Antialias the lines
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
    //glEnable(GL_DEPTH_TEST);
    our_font.init("simhei.ttf",16);
}

//------------------------------------
// 设置刷新时间
void timer(int p)
{
    glutPostRedisplay();
    glutTimerFunc(20, timer, 0);
}

//-----------------------------------
// 显示我们要绘制的模型
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清理颜色缓存和深度缓存
    glLoadIdentity(); // 重置矩阵

    glColor3ub(0xff,0, 0);

    glPushMatrix();
    glLoadIdentity();
    glRotatef(g_angle,0, 0, 1);
    glScalef(1, 0.8 + 0.3 * cos(g_angle/5), 1);
    glTranslatef(-180, 0, 0);
    // 这个是只能显示英文的
    //our_font.drawonetext(320,320,"Active 中文 FreeType Text - %7.2f", g_angle);
    our_font.drawtext(320,320,"Active 中文 FreeType \n Text - %7.2f", g_angle);
    glPopMatrix();

    g_angle += 0.051f;



    glutSwapBuffers(); //双缓冲
}

//-----------------------------------
// 当窗口变化的时候会调用这里
void reshape(int w, int h)
{
    /* 视口设置
     * 这个是当窗口发生大小变化的时候会调用这里, 大家可以自己
     * 拉伸下窗口就可以看到不一样的地方了
     */
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); // 设置视口
    glMatrixMode(GL_PROJECTION); // 设置当前矩阵为投影矩阵
    glLoadIdentity();

    if (h == 0) h = 1; // 防止除0情况
    gluPerspective(45.0f,(GLfloat) w / (GLfloat)h , 0.5f, -1000.0f);

    glMatrixMode(GL_MODELVIEW); // 设置当前矩阵为模型矩阵
    glLoadIdentity();
}

//-----------------------------------
//键盘事件
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // 当按下键盘的esc键的时候退出
            exit(0);
            break;
        case 's':
        case 'S':
            break;
        case 'h':
        case 'H':
            break;
        case 'j':
        case 'J':
            break;
        case 'k':
        case 'K':
            break;
        case 'l':
        case 'L':
            break;
    }
}

//------------------------------------
// 鼠标鼠标事件.
void MouseEvent(int button, int state, int x, int y)
{
}

//-------------------------------------
// 鼠标移动事件
void MotionMove(int x,int y)
{
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGLDemo");
    init();
    glutTimerFunc(20,timer,0); // 设置更新时间
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MouseEvent);
    glutMotionFunc(MotionMove);
    glutMainLoop();

    our_font.clean();
    return 0;
}

// mft.h
#ifndef __mtf_h__
#define __mtf_h__

#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <GL/glut.h>


#ifdef _MSC_VER
#endif

#ifdef __GNUC__
#include <unistd.h>
#endif


namespace freetype
{
    using namespace std;

    int next_p2(int a);
    int splite(const std::string &line,const std::string & s, std::vector<std::string> &vlist);

    template <typename T>
        void strtonumber(const std::string& value,T &data);
    /**
     *
     * 思路是:
     * 1.读取文件
     * 2.把英文字母, 字符, 符号这些单字符转换成纹理
     * 3.显示字符串,
     *    A 获取字符串中的数据, 进行一个一个字符或文字进行获取,
     *    B 如果是文字, 获取文字freetype索引, 生成纹理,
     *        保存gl的texture,显示列表和文字字符到std::map 已准相同文字的时候查询
     *    C 拼接要使用的纹理索引列表, 记录纹理数量
     *    D 真正的绘制了(这里还是使用显示列表来进行)
     */
    typedef struct STF_TEXTURE_DATA
    {
        GLuint m_list;
        GLuint m_texture;
        STF_TEXTURE_DATA():m_list(0),m_texture(0){}
    }STF_TEXTURE_DATA;

    // 这个是使用到的显示列表
    typedef struct STF_LIST
    {
        int size;    // 现实列表的长度
        GLuint *m_list;    // 这个是一个显示列表集合
    }STF_LIST;

    class MFont_Data
    {
        public:
            MFont_Data();
            ~MFont_Data();

            void init(const char *filename, int h);
            void clean();

            void drawtext(float x, float y, const char *fmt, ...);
            void drawonetext(float x, float y, const char *fmt, ...);

        protected:
            void drawlinetext(vector<FT_ULong> &list);
            // 创建字符和英文等单字符的文字生成静态纹理
            // 生成单个
            void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint* tex_base);
            void make_wlist(FT_Face face, FT_ULong ch, GLuint list, GLuint texture);

            //void maie_one(FT_Face face, FT_ULong ch, GLuint list, GLuint texture);

            /////////////////////////////////////////////
            // 辅助函数部分
            // 判断是否已经创建过这个文字的纹理, 这里只针对多字符,
            // 单字符直接从显示列表里面拿就好了
            bool hasTexListData(FT_ULong ch, STF_TEXTURE_DATA& data);
            void createTexListData(FT_ULong ch, STF_TEXTURE_DATA& data);

            inline void pushScreenCoordinateMatrix();
            inline void pop_projection_matrix();

            // 单个字符串, 不要有\n的
            inline int getULongList(const std::string& line, vector<FT_ULong> &list);

            // 判断是字符还是多字节文字
            inline int getUtf8Size(char c);
            // 因为freetype 中使用的是unicode ,而不是 utf8 悲剧中
            inline FT_ULong utf8tounicode(char *s, int size);

            // 这个是我们当前现实使用的字符串, 如果有变化就需要从新生成
            bool m_isChange;    // 是否需要改变
            //string m_string;
            vector<GLuint> m_textureVect;

            //vector<FT_ULong> m_chVect;    // 保存需要的所有字符

            //vector<STF_LIST> m_drawVect;    // 这个就是用来绘制的东西了
            float m_h; // 字体的高度

            GLuint *m_textures;    // 字体使用的纹理 只针对英文字母数字, 单字符符号
            GLuint m_list_base;    // 显示列表的值

            FT_Library    m_library;
            FT_Face        m_face;
            ////////////////////////////////////////
            // 保存所有的文字的map
            map<FT_ULong,FT_UInt>            m_allcharactorMap;// 文字对应的glyph索引
            map<FT_ULong,STF_TEXTURE_DATA>    m_ch2texlistMap;// 文字对应的纹理,这里只针对文字部分,字符那128里面


    };
}
// 使用方法
//
// #include "mft.h"
// using namespace freetype;
// MFont_Data our_font;
//
// our_font.init("simhei.ttf",16);
// our_font.drawtext("Active 中文 FreeType Text - %7.2f", g_angle);
// our_font.clean();
//
#endif // __mtf_h__

// mft.cpp

#include "mft.h"
#include <iostream>

namespace freetype
{

    //----------------------------------------------------------------------------------------
    // 字符串分割函数
    int splite(const std::string &line,const std::string & s, std::vector<std::string> &vlist)
    {
        if (line == "")
            return -1;


        // 去掉两边的空格
        //line.erase(0, line.find_first_not_of("\t\n\r")).erase(line.find_last_not_of("\t\n\r") + 1);

        unsigned int len = s.size();
        unsigned int spos = 0;    // 分割的位置
        unsigned int rpos = 0;    // 读取数据的位置

        do{

            spos = line.find(s, rpos);
            vlist.push_back(line.substr(rpos, spos - rpos));
            rpos = spos + len;

        }while(spos != std::string::npos);

        return 0;
    }

    //----------------------------------
    //
    int next_p2(int a)
    {
        int rval = 1;
        while(rval<a) rval <<= 1;
        return rval;
    }

    //----------------------------------------------------------------------------------------
    // 字符串转数字
    template <typename T>
        void strtonumber(const std::string& value,T &data)
        {
            std::stringstream ss;
            ss << value;
            ss >> data;
        }

    //-------------------------------------------------------
    //
    MFont_Data::MFont_Data()
    {
    }

    //-------------------------------------------------------
    //
    MFont_Data::~MFont_Data(){}

    //--------------------------------------------------------
    //
    void MFont_Data::init(const char *filename, int h)
    {
        // 英文字母, 字符, 等单个字符的纹理
        this->m_textures = new GLuint[128];
        this->m_h = h; // 字的高度

        if (FT_Init_FreeType(&m_library))
            throw std::runtime_error("FT_Init_FreeType failed");

        if (FT_New_Face(m_library, filename, 0, &m_face))
            throw std::runtime_error("FT_New_Face failed ( \
                there is probably a problem with your \
                    font file)");

        FT_Set_Char_Size(m_face, h << 6, h << 6, 96, 96);

        m_list_base = glGenLists(128);
        glGenTextures(128, m_textures);

        for (unsigned char i = 0; i < 128; i++)
            make_dlist(m_face, i, m_list_base, m_textures);


        // 单个字符的 128 个已经完成
        // 下面是加载所有的字符了
        FT_ULong charcode;
        FT_UInt gindex;

        charcode = FT_Get_First_Char(m_face, &gindex);
        while(gindex != 0)
        {
            m_allcharactorMap[charcode] = gindex;
            charcode = FT_Get_Next_Char(m_face, charcode, &gindex);
        }

    }

    //--------------------------------------------------------
    //
    void MFont_Data::clean()
    {
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_library);

        glDeleteLists(m_list_base, 128);
        glDeleteTextures(128, m_textures);
        delete [] m_textures;

        //
        map<FT_ULong,STF_TEXTURE_DATA>::iterator iter;
        for (iter = m_ch2texlistMap.begin(); iter != m_ch2texlistMap.end(); iter++)
        {
            STF_TEXTURE_DATA tmpdata = iter->second;
            glDeleteLists(tmpdata.m_list,1);
            glDeleteTextures(1,&tmpdata.m_texture);
        }
        m_ch2texlistMap.clear();
        m_allcharactorMap.clear();

    }

    //----------------------------------------------------------
    // 这里处理的是单个字符的情况
    void MFont_Data::make_dlist(FT_Face face, char ch,
            GLuint list_base, GLuint * tex_base)
    {
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face,ch),FT_LOAD_DEFAULT))
        {
            throw std::runtime_error("FT_Load_Glyph failed");
        }

        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
            throw std::runtime_error("FT_Get_Glyph failed");

        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal,0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        FT_Bitmap& bitmap = bitmap_glyph->bitmap;
        // 我们需要获取位图的信息
        int width = next_p2(bitmap.width);
        int height = next_p2(bitmap.rows);

        GLubyte* expanded_data = new GLubyte[2 * width * height];

        for(int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                expanded_data[2 *(i+j*width)] = expanded_data[2*(i+j*width)+1] =
                    (i >= bitmap.width || j >= bitmap.rows) ?
                    0 : bitmap.buffer[i + bitmap.width * j];
            }
        }

        // 设置纹理啦
        glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA,
                GL_UNSIGNED_BYTE, expanded_data);

        delete [] expanded_data;

        glNewList(list_base + ch, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
        glPushMatrix();

        glTranslatef(bitmap_glyph->left, 0, 0);
        glTranslatef(0, bitmap_glyph->top-bitmap.rows, 0);

        float x=(float)bitmap.width / (float)width;
        float y = (float)bitmap.rows / (float) height;

        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
        glTexCoord2d(0,y); glVertex2f(0,0);
        glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
        glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6 ,0,0);

        glEndList();
    }

    //-------------------------------------------------------------
    // 处理多字符的情况
    void MFont_Data::make_wlist(FT_Face face, FT_ULong ch,
            GLuint list, GLuint texture)
    {
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face,ch),FT_LOAD_DEFAULT))
        {
            throw std::runtime_error("FT_Load_Glyph failed");
        }

        FT_Glyph glyph;

        if (FT_Get_Glyph(face->glyph, &glyph))
            throw std::runtime_error("FT_Get_Glyph failed");

        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal,0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        FT_Bitmap& bitmap = bitmap_glyph->bitmap;
        // 我们需要获取位图的信息
        int width = next_p2(bitmap.width);
        int height = next_p2(bitmap.rows);

        GLubyte* expanded_data = new GLubyte[2 * width * height];

        for(int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                expanded_data[2 *(i+j*width)] = expanded_data[2*(i+j*width)+1] =
                    (i >= bitmap.width || j >= bitmap.rows) ?
                    0 : bitmap.buffer[i + bitmap.width * j];
            }
        }

        // 设置纹理啦
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA,
                GL_UNSIGNED_BYTE, expanded_data);

        delete [] expanded_data;

        glNewList(list, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPushMatrix();

        glTranslatef(bitmap_glyph->left, 0, 0);
        glTranslatef(0, bitmap_glyph->top-bitmap.rows, 0);

        float x = (float)bitmap.width / (float)width;
        float y = (float)bitmap.rows / (float) height;

        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
        glTexCoord2d(0,y); glVertex2f(0,0);
        glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
        glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6 ,0,0);

        glEndList();

        // 保存需要的信息
        STF_TEXTURE_DATA tmpdata;

        tmpdata.m_list = list;
        tmpdata.m_texture = texture;

        m_ch2texlistMap[ch] = tmpdata;
    }

    bool MFont_Data::hasTexListData(FT_ULong ch, STF_TEXTURE_DATA &data)
    {
        map<FT_ULong,STF_TEXTURE_DATA>::iterator iter = m_ch2texlistMap.find(ch);
        if (iter != m_ch2texlistMap.end())
        {
            data = iter->second;
            return true;
        }
        return false;
    }

    //-------------------------------------------------------------
    //
    void MFont_Data::drawtext(float x, float y, const char *fmt, ...)
    {
        pushScreenCoordinateMatrix();

        char text[256];
        char buff[256];

        memset(text, 0, 256);
        memset(buff, 0, 256);

        float h = m_h/ 0.63f;
        va_list ap;

        if (fmt == NULL)
            *text = 0;
        else
        {
            va_start(ap, fmt);
            vsprintf(text, fmt, ap);
            va_end(ap);
        }
        string s(text);

        /////////////
        vector<string> vlist;
        splite(s,"\n", vlist);

        glPushAttrib(GL_LIST_BASE | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glListBase(m_list_base);
        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        for (unsigned int i = 0; i < vlist.size(); i++)
        {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(x, y-h*i, 0);
            glMultMatrixf(modelview_matrix);

            vector<FT_ULong> ftlist;
            getULongList(vlist[i], ftlist);
            drawlinetext(ftlist);

            glPopMatrix();
        }

        glPopAttrib();
        pop_projection_matrix();
    }

    //------------------------------------------------------
    //
    void MFont_Data::drawonetext(float x, float y, const char *fmt, ...)
    {

        pushScreenCoordinateMatrix();

        //GLuint font = ft_font.list_base;
        float h = m_h/ 0.63f;
        char text[256];
        va_list ap;

        if (fmt == NULL)
            *text = 0;
        else
        {
            va_start(ap, fmt);
            vsprintf(text, fmt, ap);
            va_end(ap);
        }

        const char *start_line = text;
        const char *c = text;
        vector<string> lines;
        //int len = getUtf8Size(c[0]);
        for (; *c; c++)
        {
            if (*c == '\n')
            {
                string line;
                for (const char *n=start_line; n < c; n++)
                    line.append(1, *n);

                lines.push_back(line);
                start_line=c+1;
            }
        }

        if (start_line)
        {
            string line;
            for(const char *n = start_line; n<c; n++)
                line.append(1,*n);

            lines.push_back(line);
        }

        glPushAttrib(GL_LIST_BASE | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glListBase(m_list_base);
        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        for (unsigned int i = 0; i < lines.size(); i++)
        {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(x, y-h*i, 0);
            glMultMatrixf(modelview_matrix);

            glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
            glPopMatrix();
        }

        glPopAttrib();
        pop_projection_matrix();
    }
    //--------------------------------------------------------
    //
    void MFont_Data::drawlinetext(vector<FT_ULong> &list)
    {
        GLuint tmplist[list.size()];

        for (unsigned int i = 0; i < list.size(); i++)
        {
            if (list[i] > 128) // 判断下
            {
                STF_TEXTURE_DATA tmpdata;
                if (hasTexListData(list[i], tmpdata))
                {
                    tmplist[i] = tmpdata.m_list;
                    //glCallList(tmpdata.m_list);
                }
                else
                {
                    createTexListData(list[i], tmpdata);
                    tmplist[i] = tmpdata.m_list;
                    //glCallList(tmpdata.m_list);
                }
            }
            else
            {
                tmplist[i] = m_list_base + list[i]; // 直接使用字符和英文字符的纹理了
                //glCallList(m_list_base + list[i]);
                //glCallList(list[i]);
            }
        }

        // 获取了所有的显示列表 开始显示 数据
        // 这里可能要一个一个调用哦 如果下面的这个函数无法实现的话
        glCallLists(list.size(), GL_UNSIGNED_INT, tmplist);

    }

    //---------------------------------------------------------
    //
    void MFont_Data::createTexListData(FT_ULong ch, STF_TEXTURE_DATA& data)
    {
        data.m_list = glGenLists(1);
        glGenTextures(1, &data.m_texture);

        make_wlist(m_face, ch, data.m_list, data.m_texture);
    }

    //--------------------------------------------------
    //
    void MFont_Data::pushScreenCoordinateMatrix()
    {
        glPushAttrib(GL_TRANSFORM_BIT);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
        glPopAttrib();
    }

    //---------------------------------------------------
    //
    void MFont_Data::pop_projection_matrix()
    {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
    }

    //----------------------------------------------------
    //
    int MFont_Data::getULongList(const std::string& line, vector<FT_ULong> &list)
    {
        if (line == "")
            return 0;

        char buff[256];

        unsigned int len = line.size();

        unsigned int i = 0;
        FT_ULong tmpdata = 0;

        while(i < len)
        {
            char c = line[i];
            int size = getUtf8Size(c);

            if (size > 0)
            {
                memset(buff, 0, 256);
                unsigned int count = i + size;
                unsigned int k = 0;
                while (i < count)
                {
                    buff[k]     = line[i];
                    k++;
                    i++;
                }

                tmpdata = utf8tounicode(buff,size);
            }
            else
            {
                tmpdata = line[i];
                i++;
            }
            list.push_back(tmpdata);
        } // while
        return 0;
    }

    //--------------------------------------------------------
    //
    int MFont_Data::getUtf8Size(char c)
    {
        char t = 1 << 7;
        char r = c;
        int count = 0;
        while (r & t)
        {
            r = r << 1;
            count++;
        }
        return count;
    }

    //-----------------------------------------------------
    //
    FT_ULong MFont_Data::utf8tounicode(char *s, int size)
    {
        char uc[] = {0x7f,0x3f,0x1f,0x0f,0x07,0x03};

        FT_ULong data = 0;
        FT_ULong tmp = 0;
        int count = 0;

        char *p = s;
        while (count < size)
        {
            char c = (*p);
            tmp = c;

            if (count == 0)
            {
                tmp &= uc[size - 1];
            }
            else
            {
                tmp &= 0x3f;
            }

            tmp = tmp << (6 * (size - count - 1));
            data |= tmp;

            p++;
            count++;
        }

        return data;
    }

}

/*

// makefile
CXX=g++
FLAGS=-Wall -o2 -g
OBJS=main.o mft.o
TARGETS=main
#LIBS= $(shell pkg-config --libs glu  glew) -L/usr/local/lib/ -lglut

LIBS=$(shell pkg-config --libs gl) -L/usr/lib -lGLU -lglut -lfreetype
INCLUDES=$(shell pkg-config --cflags gl) -I/usr/include -I. -I/usr/include/freetype2

#LIBS= -L"/usr/lib" -lGL -lGLU -lglut #-lGLEW

#vpath %.h /usr/include
#$(TARGETS)


all:$(TARGETS)
$(TARGETS):$(OBJS)
$(CXX) $(FLAGS) $(OBJS) -o $@ $(LIBS)
$(OBJS):%.o:%.cpp
$(CXX) $(FLAGS) -c  $< -o $@ $(INCLUDES)
clean:
*/
