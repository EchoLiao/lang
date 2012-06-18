/*
 * ===========================================================================
 *
 *       Filename:  tsl.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012年06月14日 10时11分27秒
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


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tsl.h"


#define TSL_TYPE_FONT       1
#define TSL_TYPE_POS        2
#define TSL_TYPE_TITLE      3
#define TSL_TYPE_LYRICS     16


static int tslSTreadContent(TSLLyrics *plyr, FILE *fp);



int  tslOpen(FILE **fp, const char *file)
{
    assert(fp != NULL && *fp == NULL && file != NULL);

    char buf[NMAGIC];

    if ( (*fp=fopen(file, "rb+")) == NULL )
        goto _CANNOT_OPEN;
    
    fread(buf, 1, NMAGIC, *fp);
    if ( strncmp(buf, FILE_MAGIC, NMAGIC) == 0 )
        return 1;

_CANNOT_OPEN:
    *fp = NULL;
    return 0;
}

int  tslReadHead(TSLFileHead *tslHead, FILE *fp)
{
    assert(tslHead != NULL && fp != NULL);

    fseek(fp, 0, SEEK_SET);
    fread(tslHead, 1, sizeof(*tslHead), fp);

    return 1;
}

int  tslRead(TSLLyrics **lyrics, int *nline, FILE *fp)
{
    assert(lyrics != NULL && *lyrics == NULL
            && nline != NULL && fp != NULL);

    int  ret, len, curline;
    char buff[512];
    TSLFileHead head;
    TSLLyrics   *plyr;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    ret = tslReadHead(&head, fp);
    *nline = head.nlyrics;

    curline = 0;
    *lyrics = (TSLLyrics *)malloc(sizeof(**lyrics) * head.nlyrics);
    if ( *lyrics == NULL )
        goto _NO_MEMORY_LYRICS;
    memset(*lyrics, 0, sizeof(**lyrics) * head.nlyrics);

    plyr = *lyrics;
    while ( 1 )
    {
        if ( ftell(fp) >= len ) 
            break;
        fread(buff, 1, 1, fp);
		switch ( buff[0] )
		{
		case TSL_TYPE_FONT:     // 字体结构
            fseek(fp, 1+1+14+2+1+1+4+4+4+4-1, SEEK_CUR);
			break;
		case TSL_TYPE_POS:      // 位置结构
            fseek(fp, 1+1+1+1+2+2-1, SEEK_CUR);
			break;
		case TSL_TYPE_TITLE:    // 片头结构
            fseek(fp, 1+1+1+1+4+(1+1+30)*8-1, SEEK_CUR);
			break;
		case TSL_TYPE_LYRICS:   // 歌词结构
            if ( ! tslSTreadContent(plyr, fp) )
                goto _NO_MEMORY_CELL;
            plyr++;
			break;
        default:
            assert(0);
            break;
		}
    }

    return 1;

_NO_MEMORY_CELL:
    for ( plyr = *lyrics; plyr != NULL; plyr++ )
    {
        free(plyr->cells);
        plyr->cells = NULL;
    }
    free(*lyrics);
_NO_MEMORY_LYRICS:
    *lyrics = NULL;
    return 0;
}

static int tslSTreadContent(TSLLyrics *plyr, FILE *fp)
{
    assert(plyr !=NULL && fp != NULL);

    int k, i;

    plyr->type = TSL_TYPE_LYRICS;
    fread(&plyr->flag, 1, 1, fp);
    fread(&plyr->fontid, 1, 1, fp);
    fread(&plyr->posid, 1, 1, fp);
    fread(&plyr->begintime, 1, 4, fp);
    fread(&plyr->ncell, 1, 1, fp);
    fread(&plyr->nsize, 1, 1, fp);
    assert(plyr->nsize <= LYRICSLENMAX - 1);
    fread(&plyr->reserve, 1, 2, fp);

    fread(&plyr->content, 1, plyr->nsize, fp);
    plyr->content[(int)plyr->nsize] = '\0';
    // 歌词内容: 占用空间为4的倍数('\0'后的数据为无效数据)
    for ( k = strlen(plyr->content); k < plyr->nsize; k++ )
        plyr->content[k] = '\0';

    plyr->cells = malloc(sizeof(TSLCell) * plyr->ncell);
    if ( plyr->cells == NULL ) 
        return 0;
    memset(plyr->cells, 0, sizeof(TSLCell) * plyr->ncell);
    for ( i = 0; i < plyr->ncell; i++ )
    {
        fread(&(plyr->cells[i].totaltimes), 1, 2, fp);
        fread(&(plyr->cells[i].percent), 1, 2, fp);
    }

    return 1;
}

