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


static int tslSTreadHead(TSLFileHead *tslHead, FILE *fp);
static int tslSTreadTitle(TSLMediaHead *title, FILE *fp);
static int tslSTreadContent(TSLLyrics *plyr, FILE *fp);



int  tslOpen(TSLFile *tslFile, const char *file)
{
    assert(tslFile != NULL && tslFile->fp == NULL &&
            tslFile->lyrics == NULL && file != NULL);

    char buf[NMAGIC];

    if ( NULL == (tslFile->fp = fopen(file, "rb+")) )
        goto _CANNOT_OPEN;

    fread(buf, 1, NMAGIC, tslFile->fp);
    if ( strncmp(buf, FILE_MAGIC, NMAGIC) == 0 )
    {
        fseek(tslFile->fp, 0, SEEK_SET);
        return 1;
    }

_CANNOT_OPEN:
    return 0;
}

int  tslRead(TSLFile *tslFile)
{
    assert(tslFile != NULL && tslFile->fp != NULL &&
            tslFile->lyrics == NULL);

    int  ret, len;
    char buff[512];
    TSLLyrics   *plyr;
    FILE        *fp = tslFile->fp;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    ret = tslSTreadHead(&tslFile->head, fp);

    tslFile->lyrics = (TSLLyrics *)malloc(
            sizeof(*(tslFile->lyrics)) * tslFile->head.nlyrics);
    if ( tslFile->lyrics == NULL )
        goto _NO_MEMORY_LYRICS;
    memset(tslFile->lyrics, 0,
            sizeof(*(tslFile->lyrics)) * tslFile->head.nlyrics);

    plyr = tslFile->lyrics;
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
                tslSTreadTitle(&tslFile->title, fp);
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
    plyr = tslFile->lyrics;
    TSLLyrics *plyrEnd = plyr + tslFile->head.nlyrics;
    for ( ; plyr < plyrEnd; plyr++ )
    {
        free(plyr->cells);
        plyr->cells = NULL;
    }
    free(plyr->cells);
_NO_MEMORY_LYRICS:
    tslFile->lyrics = NULL;
    return 0;
}

int  tslClose(TSLFile *tslFile)
{
    assert(tslFile != NULL);

    if ( tslFile->fp != NULL )
    {
        fclose(tslFile->fp);
        tslFile->fp = NULL;
    }

    if ( tslFile->lyrics != NULL )
    {
        TSLLyrics *plyr = tslFile->lyrics;
        TSLLyrics *plyrEnd = plyr + tslFile->head.nlyrics;
        for ( ; plyr < plyrEnd; plyr++ )
        {
            free(plyr->cells);
            plyr->cells = NULL;
        }
        free(tslFile->lyrics);
        tslFile->lyrics = NULL;
    }

    return 1;
}

static int tslSTreadHead(TSLFileHead *tslHead, FILE *fp)
{
    assert(tslHead != NULL && fp != NULL);

    fseek(fp, 0, SEEK_SET);
    fread(tslHead, 1, sizeof(*tslHead), fp);

    return 1;
}

static int tslSTreadTitle(TSLMediaHead *title, FILE *fp)
{
    assert(title != NULL && fp != NULL);

    int i;

    title->type = TSL_TYPE_TITLE;
    fread(&title->effect, 1, 1, fp);
    fread(&title->showtime, 1, 1, fp);
    fread(&title->reserved, 1, 1, fp);
    fread(&title->begintime, 1, 4, fp);

    for ( i = 0; i < NMEDIASMAX; i++ )
    {
        fread(&(title->contents[i].fontid),   1, 1, fp);
        fread(&(title->contents[i].posid),    1, 1, fp);
        fread(&(title->contents[i].content),  1, CONTENTLENMAX, fp);
    }

    return 1;
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

