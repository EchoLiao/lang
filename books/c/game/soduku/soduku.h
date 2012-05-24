/*
 * ===========================================================================
 *
 *       Filename:  soduku.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月21日 16时16分12秒
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


#ifndef  SOKUKU_H_
#define SOKUKU_H_


#define SODK_ROWS     9
#define SODK_COLS     9

#define SODK_DIGI_LOW           1
#define SODK_DIGI_HIGH          9
#define SODK_DIGI_ORI           0
#define SODK_DIGI_DIG           -1
#define SODK_DIGI_BANDIG        -2

#define sodk_DE_isValidDigi(x)  ( (x) >= SODK_DIGI_LOW \
        && (x) <= SODK_DIGI_HIGH )



enum em_sodkGrade {
    SODK_GRADE_LOW = 0,
    SODK_GRADE_PRIMARY,
    SODK_GRADE_MIDDLE,
    SODK_GRADE_HIGH,
    SODK_GRADE_ASHES,
    SODK_GRADE_COUNT
};



extern int  sodk_create(int *tab);
extern int  sodk_cal(int *tab);
extern int  sodk_verification(int *tab);



#endif
