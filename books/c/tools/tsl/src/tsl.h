/*
 * ===========================================================================
 *
 *       Filename:  tsl.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年06月14日 10时11分22秒
 *        Created:
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

#ifndef TSL_H_
#define TSL_H_

#define     FILE_MAGIC                  "TSL"
#define     NMAGIC                      3
#define     LYRICSLENMAX                1024
#define     CONTENTLENMAX               30
#define     NMEDIASMAX                  8


typedef struct tagTSLFILEHEAD
{
    char            head[NMAGIC];	// tsl
    unsigned char   version;		// 版本号
    int             crc;			//
    unsigned char   fonts;		    //字体数
    unsigned char   positions;	    //位置数
    unsigned short  nlyrics;		    //歌词数
    int             codetype;    //歌词编码格式
}TSLFileHead, *lpTSLFileHead;

typedef struct tagCONTENT
{
    unsigned char fontid;		    //字体id
    unsigned char posid;		    //位置id
    char content[CONTENTLENMAX];    //文字内容
}TSLContent,*lpTSLContent;

typedef struct tagMEDIAHEAD
{
    unsigned char type;			    //类型 = 3
    unsigned char effect;		    //特效
    unsigned char showtime;		    //显示时长
    char reserved[1];			    //字保留
    int  begintime;				    //片头开始显示时间
    TSLContent contents[NMEDIASMAX];//内容
}TSLMediaHead, *lpMediaHead;

typedef struct tagCELL
{
    short percent;
    short totaltimes;
}TSLCell, *lpTSLCell;

typedef struct tagLYRICS
{
    char type;
    char flag;
    char fontid;
    char posid;
    int  begintime;		//本行歌词开始时间
    char ncell;
    char nsize;
    char reserve[2];
    char content[LYRICSLENMAX];	//本行歌词内容
    lpTSLCell cells;	//分割单元数组
}TSLLyrics, *lpTSLLyrics;

/* 
typedef struct tagLYRICSSHOW
{
    CFont Font;				//歌词使用的字体
    int CurrLine;			//当前正在显示的第几行
    int CurrLineBeginTimes;	//当前行的开始显示时间
    int CurrCell;			//当前分割单元序号
    int CurrCellBeginPos;	//当前分割单元序号填充听改应该开始的位置
    int CurrCellBeginTimes;	//当前分割单元在本行开始填充的开始时间
    double CurrFillSpeed;	//当前分割单元填充速度

    //
    int Fonts;			//字体定义数
    int Positions;		//位置定义数
    int LyricLines;		//歌词行数
    LPTSLFONTDEF FontDefs;	//
    LPTSLPOSTIONDEF	PostionDefs;

    lpLyrics Lyrics;	//每行歌词结构对应的 数组

}LYRICSSHOW, *LPLYRICSSHOW;
 */



int  tslOpen(FILE **fp, const char *file);
int  tslReadHead(TSLFileHead *tslHead, FILE *fp);
int  tslRead(TSLLyrics **lyrics, int *nline, FILE *fp);



#endif
