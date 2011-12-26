/*
	A quick and simple opengl font library that uses GNU freetype2, written
	and distributed as part of a tutorial for nehe.gamedev.net.
	Sven Olsen, 2003
*/


//Include our header file.
#include "FreeType.h"
#include "../LESSON11/bmprw.h"
#include <assert.h>
#include <stdio.h>
#include <map>

namespace freetype {

/*
 * Unicode 与 UTF-8 的关系:
 *
 *   Unicode 编码                     UTF-8 存储码
 * ========================  =====================================================
 * U-00000000 - U-0000007F:  0xxxxxxx
 * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
 * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
 * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 
 *  (前面几个1就代表后面几个字节是属于一起的)
 *  
 */

typedef struct STF_TEXTURE_DATA
{
    GLuint m_list;
    GLuint m_texture;
    STF_TEXTURE_DATA():m_list(0),m_texture(0){}
}STF_TEXTURE_DATA;

//
std::map<FT_ULong,STF_TEXTURE_DATA>    m_ch2texlistMap;

FT_Face face;




/*****************************************************************************
 * 根据字符的UTF-8编码的第一个字节求出该字符用UTF-8编码存储时所需要多少个
 * 字节空间. 特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 *
 * 参数:
 *    char c    字符的UTF-8编码的第一个字节的值
 *
 * 返回值:
 *   该字符用UTF-8编码存储时所需要多少个字节空间.
 *   特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 ****************************************************************************/
int enc_get_utf8_size(char c)
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


/*****************************************************************************
 * 将UTF8编码转换成Unicode(UCS-2)编码.
 *
 * 参数:
 *    char* pInput   指向输入字符串(以'\0'结尾)的指针
 *    char* pOutput  指向输出字符串指针的指针
 *    int   outsize  pOutput 缓冲的大小
 *
 * 返回值:
 *    返回转换后的Unicode字符串的字节数, 如果出错则返回 -1
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode字符有字节序,
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种,
 *        在Intel处理器中采用小端法表示, 本例中采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有足够的空间, 若空间不足, 则只进行部分转换.
 ****************************************************************************/
int enc_utf8_to_unicode(const char* pInput, char* pOutput, int outsize)
{
    assert(pInput != NULL && pOutput != NULL);
    assert(outsize >= 3);

    int  outputSize = 0;  //记录转换后的Unicode字符串的字节数
    char *pout = pOutput;

    while ( *pInput && outputSize + 2 + 1 <= outsize )
    {
        if (*pInput > 0x00 && *pInput <= 0x7F) // 处理单字节UTF8字符
        {
            *pout     = *pInput;
            *(pout+1) = 0;       // 小端法表示, 在高地址填补0
            pInput += 1;
        }
        else if (((*pInput) & 0xE0) == 0xC0) // 处理双字节UTF8字符
        {
            char high = *pInput;
            char low  = *(pInput + 1);

            if ((low & 0xC0) != 0x80) // 检查是否为合法的UTF8字符表示
                return -1;

            *pout     = (high << 6) + (low & 0x3F);
            *(pout+1) = (high >> 2) & 0x07;
            pInput += 2;
        }
        else if (((*pInput) & 0xF0) == 0xE0) // 处理三字节UTF8字符
        {
            char high   = *pInput;
            char middle = *(pInput + 1);
            char low    = *(pInput + 2);

            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
                return -1;

            *pout     = (middle << 6) + (low & 0x7F);
            *(pout+1) = (high << 4) + ((middle >> 2) & 0x0F);
            pInput += 3;
        }
        else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }

        pout += 2;
        outputSize += 2;
    }

    *pout = 0;

    return outputSize;
}

    
///This function gets the first power of 2 >= the
///int that we pass it.
// 这个函数返回比a大的, 并且是最接近a的2的次方的数
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

///Create a display list coresponding to the give character.
void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base ) 
{

	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
    // 载入给定字符的轮廓.
    // 对于 FT_Get_Char_Index(face, ch), ch 最好是unicode编码的, 因为大多数
    // 的*.ttf字体文件都包含有unicode编码索引表.
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
    // 保存轮廓对象
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
    // 把轮廓转化为位图
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
    // 保存位图
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
    // 在OpenGL中纹理大小必须为2的次方. 
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

    /*
       在FreeType中我们使用8位的颜色表示位图, 所以 FreeType 的位图可以保存亮度
       信息.
     */

	//Allocate memory for the texture data.
    // 申请两倍大小的原因: 我们使用的是 GL_LUMINANCE_ALPHA !
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
    // 这里我们使用8位表示亮度, 8位表示alpha值
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}


	//Now we just setup some texture paramaters.
    glBindTexture( GL_TEXTURE_2D, tex_base[(int)ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		  0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

#if 1 // For Debug
    char dstBmpFile[128];
    if ( ch == '\0' )
        sprintf(dstBmpFile, "%03d-0x%02x-0%03o-00.bmp", (int)ch, (int)ch, (int)ch);
    else if ( ch == '\\' ) 
        sprintf(dstBmpFile, "%03d-0x%02x-0%03o-5c.bmp", (int)ch, (int)ch, (int)ch);
    else if ( ch == '/' ) 
        sprintf(dstBmpFile, "%03d-0x%02x-0%03o-2f.bmp", (int)ch, (int)ch, (int)ch);
    else
        sprintf(dstBmpFile, "%03d-0x%02x-0%03o-%c.bmp", (int)ch, (int)ch, (int)ch, ch);
    sbitData *bmpw = bmpCreateObjForWrite(EBMP_BGR, 1, width, height, 16, expanded_data);
    assert( bmpw != NULL );
    assert( bmpWrite(dstBmpFile, bmpw) );
    assert( bmpDestroyObjForWrite(&bmpw) );
#endif

	//With the texture created, we don't need to expanded data anymore
    delete [] expanded_data;

	//So now we can create the display list
	glNewList(list_base+ch,GL_COMPILE); // NOTE1

	glBindTexture(GL_TEXTURE_2D,tex_base[(int)ch]);

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(bitmap_glyph->left,0,0);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line 
	//(this is only true for characters like 'g' or 'y'.
	glPushMatrix();
	glTranslatef(0,bitmap_glyph->top-bitmap.rows,0);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by 
	//the actual character and store that information in 
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not 
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
    /*
     *      ^
     *      |      (1,1)                      (w,0)
     * (0,1)|--------             (0,0)|----------->
     *      |       |                  |       |
     *      |       |(1,0)             |       |(w,r)
     * (0,0)|-------|--->         (0,r)|--------
     *    <Texture Cord>               |
     *                                 v  <Freetype Bitmap Cord>
     */
    // 绘制一个正方形, 显示字符
	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
	glTexCoord2d(0,y); glVertex2f(0,0);
	glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
	glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
	glEnd();
	glPopMatrix();

    // 移动光标到下一个位置, 以画下一个字符
	glTranslatef(face->glyph->advance.x >> 6 ,0,0);


	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	//Finnish the display list
	glEndList();
}


void font_data::init(const char * fname, unsigned int h) {
	//Allocate some memory to store the texture ids.
	textures = new GLuint[128];

	this->h=h;

	//Create and initilize a freetype font library.
    // 创建FreeType库
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
    // 在FreeType库中保存字体信息的类叫做face
	// FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
    // 使用输入的Freetype字符文件初始化face类
	if (FT_New_Face( library, fname, 0, &face )) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
    // 在FreeType中使用1/64作为一个像素的高度所以我们需要缩放h来满足这个要求
	FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.  
	list_base=glGenLists(128);
	glGenTextures( 128, textures );

	//This is where we actually create each of the fonts display lists.
	for(unsigned char i=0;i<128;i++)
		make_dlist(face,i,list_base,textures);

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	// FT_Done_Face(face);

	//Ditto for the library.
	// FT_Done_FreeType(library);
}

void font_data::clean() {
	glDeleteLists(list_base,128);
	glDeleteTextures(128,textures);
	delete [] textures;
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
    // 把到目前为止的所有的矩阵变换保存, 以防止其被glLoadIdentity等破坏!
    glPushAttrib(GL_TRANSFORM_BIT); {
        GLint	viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix(); // PM1

        glLoadIdentity();
        gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
    } glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT); {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // PM1
    } glPopAttrib();
}

#define TEXT_SIZE   256

///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void print(const font_data &ft_font, float x, float y, const char *fmt, ...)  
{
    // We want a coordinate system where things coresponding to window pixels.
    // 画字时, 我们选择正交投影
    pushScreenCoordinateMatrix();					

    GLuint font=ft_font.list_base;
    // 使行与行之间有一定的空隙
    float h=ft_font.h/.63f;						//We make the height about 1.5* that of

    char		text[TEXT_SIZE];						// Holds Our String
    va_list		ap;										// Pointer To List Of Arguments

    if (fmt == NULL)									// If There's No Text
        *text=0;											// Do Nothing
    else {
        va_start(ap, fmt);
        // vsnprintf() 从 fmt 中最多格式化 TEXT_SIZE 个字符到 text 中(包括'\0'),
        // 返回值是 fmt 字符串的长度(不包括'\0').
        int fmt_len = vsnprintf(text, TEXT_SIZE, fmt, ap);
        if ( fmt_len + 1 > TEXT_SIZE )
            fprintf(stdout, "NO enough space to format!!\n");
        va_end(ap);
    }

    //Here is some code to split the text that we have been
    //given into a set of lines.
    //This could be made much neater by using
    //a regular expression library such as the one avliable from
    //boost.org (I've only done it out by hand to avoid complicating
    //this tutorial with unnecessary library dependencies).
    const char *start_line=text;
    vector<string> lines; // 用于保存提取出来的各行
    const char * c = text;
    for(;*c;c++) {
        if(*c=='\n') {
            string line;
            for(const char *n=start_line;n<c;n++) line.append(1,*n);
            lines.push_back(line);
            start_line=c+1;
        }
    }
    if(start_line) {
        string line;
        for(const char *n=start_line;n<c;n++) line.append(1,*n);
        lines.push_back(line);
    }

    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT); {
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

        glListBase(font);

        float modelview_matrix[16];	
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        //This is where the text display actually happens.
        //For each line of text we reset the modelview matrix
        //so that the line's text will start in the correct position.
        //Notice that we need to reset the matrix, rather than just translating
        //down by h. This is because when each character is
        //draw it modifies the current matrix so that the next character
        //will be drawn immediatly after it.  
        for(unsigned int i=0;i<lines.size();i++) {
            glPushMatrix(); {
                glLoadIdentity();
                glTranslatef(x,y-h*i,0);
                glMultMatrixf(modelview_matrix);

                //  The commented out raster position stuff can be useful if you need to
                //  know the length of the text that you are creating.
                //  If you decide to use it make sure to also uncomment the glBitmap command
                //  in make_dlist().
                glRasterPos2f(0,0);
                // glCallLists() 会修改当前矩阵!     // NOTE1
                glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
                float rpos[4];
                glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
                float len;
                len = x - rpos[0];
            } glPopMatrix();
        }
    } glPopAttrib();		

    pop_projection_matrix();
}

int getULongList(string &str, vector<FT_ULong> &list)
{
    if ( str == "" )
        return 0;

    char          buff[256];
    unsigned int  len = str.size();  // 单位为字节
    unsigned int  i = 0;
    FT_ULong      tmpdata = 0;
    char          unibuff[3];

    while ( i < len )
    {
        char c = str[i];
        int size = enc_get_utf8_size(c);
        if (size > 0) // 非ASCII字符
        {
            memset(buff, 0, 256);
            unsigned int count = i + size;
            unsigned int k = 0;
            while (i < count)
                buff[k++] = str[i++];
            enc_utf8_to_unicode(buff, unibuff, 3);
            assert(sizeof(FT_ULong) ==  4);
            unsigned short tmp = *((unsigned short *)&unibuff);
            tmpdata = (FT_ULong)tmp;
        }
        else          // ASCII字符
        {
            tmpdata = str[i++];
        }
        list.push_back(tmpdata);
    } // while
    return 0;

}

bool hasTexListData(FT_ULong ch, STF_TEXTURE_DATA &data)
{
    std::map<FT_ULong,STF_TEXTURE_DATA>::iterator iter = m_ch2texlistMap.find(ch);
    if (iter != m_ch2texlistMap.end())
    {
        data = iter->second;
        return true;
    }
    return false;
}

// 处理多字节字符的情况
void make_wlist(FT_Face face, FT_ULong ch,
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

void createTexListData(FT_ULong ch, STF_TEXTURE_DATA& data)
{
    data.m_list = glGenLists(1);
    glGenTextures(1, &data.m_texture);

    make_wlist(face, ch, data.m_list, data.m_texture);
}

void drawlinetext(const font_data &ft_font, vector<FT_ULong> &list)
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
            tmplist[i] = ft_font.list_base + list[i];
            //glCallList(m_list_base + list[i]);
            //glCallList(list[i]);
        }
    }

    // 获取了所有的显示列表 开始显示 数据
    // 这里可能要一个一个调用哦 如果下面的这个函数无法实现的话
    glCallLists(list.size(), GL_UNSIGNED_INT, tmplist);
}

void wprint(const font_data &ft_font, float x, float y, const char *fmt, ...)  
{
    // We want a coordinate system where things coresponding to window pixels.
    // 画字时, 我们选择正交投影
    pushScreenCoordinateMatrix();					

    // GLuint font = ft_font.list_base;
    // 使行与行之间有一定的空隙
    float       h = ft_font.h/.63f;						//We make the height about 1.5* that of
    char		text[TEXT_SIZE];						// Holds Our String
    va_list		ap;										// Pointer To List Of Arguments

    if (fmt == NULL)									// If There's No Text
        *text=0;											// Do Nothing
    else {
        va_start(ap, fmt);
        // vsnprintf() 从 fmt 中最多格式化 TEXT_SIZE 个字符到 text 中(包括'\0'),
        // 返回值是 fmt 字符串的长度(不包括'\0').
        int fmt_len = vsnprintf(text, TEXT_SIZE, fmt, ap);
        if ( fmt_len + 1 > TEXT_SIZE )
            fprintf(stderr, "NO enough space to format!!\n");
        va_end(ap);
    }

    //Here is some code to split the text int lines.
    const char *start_line=text;
    vector<string> lines; // 用于保存提取出来的各行
    const char * c = text;
    for(;*c;c++) {
        if(*c=='\n') {
            string line;
            for(const char *n=start_line;n<c;n++) line.append(1,*n);
            lines.push_back(line);
            start_line=c+1;
        }
    }
    if(start_line) {
        string line;
        for(const char *n=start_line;n<c;n++) line.append(1,*n);
        lines.push_back(line);
    }

    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT); {
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

        // glListBase(font);

        float modelview_matrix[16];	
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

        for(unsigned int i=0;i<lines.size();i++) {
            glPushMatrix(); {
                glLoadIdentity();
                glTranslatef(x,y-h*i,0);
                glMultMatrixf(modelview_matrix);

                vector<FT_ULong> ftlist;
                getULongList(lines[i], ftlist);
                drawlinetext(ft_font, ftlist);

            } glPopMatrix();
        }
    } glPopAttrib();		

    pop_projection_matrix();
}

}
