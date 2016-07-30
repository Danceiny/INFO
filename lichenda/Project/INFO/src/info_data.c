/*******************************************************************************
  Copypstright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All pstrights reserved.
  --------------------------------------------------------------------------------
  info_data.c
  Project Code: Comware V700R001
  Module Name: INFO
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: �ڲ����ݲ���
����������ɾ�������á���ȡ������

--------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
YYYY-MM-DD  

 *******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/* standard library */
#include <stdlib.h>
#include <string.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"

/*�������ɫ*/
typedef enum tagColor{
    INFO_COLOR_RED = 1,
    INFO_COLOR_BLACK = 2
} INFO_COLOR_E;

/* ��Ϣ���ݽṹ */
typedef struct tagInfo_Data
{
    INFO_COLOR_E encolor;     /* ��ɫ */
    struct tagInfo_Data *pstleft;     /* ������ */
    struct tagInfo_Data *pstright;    /* ������ */ 
    struct  tagInfo_Data *pstparent;   /* ���ڵ� */ 
    INFO_CFG_S stCfg;       /* �������� */
}INFO_DATA_S;

/*�����nilָ��ָ��nill�ڵ�*/
INFO_DATA_S g_stnill;
STATIC INFO_DATA_S * const g_pstnil = &g_stnill;

/*���ڵ�*/
STATIC INFO_DATA_S *g_pstroot;

STATIC BOOL_T info_data_LeftRotate(IN INFO_DATA_S *pstX, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY = pstX->pstright;
    pstX->pstright = pstY->pstleft;
    if (pstY->pstleft != g_pstnil)
    {
        pstY->pstleft->pstparent = pstX;
    }
    pstY->pstparent = pstX->pstparent;
    if(pstX->pstparent == g_pstnil)
    {
        *ppstroot = pstY;
    }
    else if (pstX == pstX->pstparent->pstleft)
    {
        pstX->pstparent->pstleft = pstY;
    }
    else
    {
        pstX->pstparent->pstright = pstY;
    }
    pstY->pstleft = pstX;
    pstX->pstparent = pstY;
    return BOOL_TRUE;
}

STATIC BOOL_T info_data_RightRotate(IN INFO_DATA_S *pstX, INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY = pstX->pstleft;
    pstX->pstleft = pstY->pstright;
    if (pstY->pstright != g_pstnil) 
    {
        pstY->pstright->pstparent = pstX;
    }
    pstY->pstparent = pstX->pstparent;
    if(pstX->pstparent == g_pstnil)
    {
        *ppstroot = pstY;
    }
    else if (pstX == pstX->pstparent->pstleft)
    {
        pstX->pstparent->pstleft = pstY;
    }
    else
    {
        pstX->pstparent->pstright = pstY;
    }
    pstY->pstright = pstX;
    pstX->pstparent = pstY;
    return BOOL_TRUE;
}
STATIC BOOL_T info_data_fixup_right(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY;
    pstY = pstZ->pstparent->pstparent->pstright;
    if(pstY->encolor == INFO_COLOR_RED)
    {
        pstZ->pstparent->encolor = INFO_COLOR_BLACK;
        pstY->encolor = INFO_COLOR_BLACK;
        pstZ->pstparent->pstparent->encolor = INFO_COLOR_RED;
        pstZ = pstZ->pstparent->pstparent;
    }
    else
    {
        if(pstZ == pstZ->pstparent->pstright)
        {
            pstZ = pstZ->pstparent;
            DBGASSERT(info_data_LeftRotate(pstZ, ppstroot));
        }
        pstZ->pstparent->encolor = INFO_COLOR_BLACK;
        pstZ->pstparent->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_RightRotate(pstZ->pstparent->pstparent, ppstroot));
    }
    return BOOL_TRUE;
}
STATIC BOOL_T info_data_fixup_left(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY;
    pstY = pstZ->pstparent->pstparent->pstleft;
    if(pstY->encolor == INFO_COLOR_RED)
    {
        pstZ->pstparent->encolor = INFO_COLOR_BLACK;
        pstY->encolor = INFO_COLOR_BLACK;
        pstZ->pstparent->pstparent->encolor = INFO_COLOR_RED;
        pstZ = pstZ->pstparent->pstparent;
    }
    else
    {

        if(pstZ == pstZ->pstparent->pstleft)
        {
            pstZ = pstZ->pstparent;
            DBGASSERT(info_data_RightRotate(pstZ, ppstroot));
        }
        pstZ->pstparent->encolor = INFO_COLOR_BLACK;
        pstZ->pstparent->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_LeftRotate(pstZ->pstparent->pstparent, ppstroot));
    }
    return BOOL_TRUE;
}



STATIC BOOL_T info_data_fixup(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{

    while(pstZ->pstparent->encolor == INFO_COLOR_RED)
    {
        if(pstZ->pstparent == pstZ->pstparent->pstparent->pstright)
        {
            DBGASSERT(info_data_fixup_right(pstZ, ppstroot));
        }
        else
        {
            DBGASSERT(info_data_fixup_left(pstZ, ppstroot));
        }
    }
    (*ppstroot)->encolor = INFO_COLOR_BLACK;
    return BOOL_TRUE;
}




/*****************************************************************************
  Func Name: INFO_data_IsExist[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: �ж�ָ�����ŵ������Ƿ����
Input: IN UINT uiId         ����
Output: 
Return: BOOL_T, BOOL_TRUE    ����
BOOL_FALSE   ������
Caution: 
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
BOOL_T INFO_data_IsExist(IN UINT uiId)
{
    return BOOL_FALSE;
}

/*****************************************************************************
  Func Name: INFO_data_IsEmpty[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: �ж�����������֯�Ƿ�Ϊ��
Input: 
Output: 
Return: BOOL_T, BOOL_TRUE    ������֯Ϊ��
BOOL_FALSE   ������֯�ǿ�
Caution: 
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
BOOL_T INFO_data_IsEmpty(VOID)
{
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: INFO_data_GetData[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ��ȡ��������
Input: IN UINT uiId             ����
Output: OUT INFO_CFG_S *pstCfg   ��������
Return: ULONG, ERROR_SUCCESS     ����ɹ�
OTHER             ����ʧ��
Caution: ���ν��ڷ��سɹ�ʱ��Ч
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
ULONG INFO_data_GetData(IN UINT uiId, OUT INFO_CFG_S *pstCfg)
{
    return ERROR_FAILED;
}

/*****************************************************************************
  Func Name: INFO_data_GetFirst[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ��ȡ��һ�������ݹ���
Input: VOID
Output: 
Return: UINT, != INFO_ID_INVALID     ��һ�������ݵĹ���
== INFO_ID_INVALID     δ�ҵ�
Caution: 
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
UINT INFO_data_GetFirst(VOID)
{
    return INFO_ID_INVALID;
}

/*****************************************************************************
  Func Name: INFO_data_GetNext[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ��ȡ��һ�������ݹ���
Input: IN UINT uiId                 ��ǰ����
Output: 
Return: UINT, != INFO_ID_INVALID     ��һ������
== INFO_ID_INVALID     δ�ҵ�
Caution: �˽ӿڻ�ȡ��һ�����Ų����������uiId�����Ƿ�������
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
UINT INFO_data_GetNext(IN UINT uiId)
{
    return INFO_ID_INVALID;
}

/*****************************************************************************
  Func Name: INFO_data_Init[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ģ���ʼ��
Input: 
Output: 
Return: ULONG, ERROR_SUCCESS     ����ɹ�
OTHER             ����ʧ��
Caution: Ŀǰʼ�ճɹ�
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    return ERROR_SUCCESS;
}

/*****************************************************************************
  Func Name: INFO_data_Fini[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ģ���˳�
Input: 
Output: 
Return: VOID
Caution: ���ô˽ӿ�ǰ�������Ѿ���ʼ����
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
VOID INFO_data_Fini(VOID)
{
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

