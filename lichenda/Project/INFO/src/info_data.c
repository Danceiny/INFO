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
#include "info_parse.h"

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

/*****************************************************************************
  Func Name: info_data_LeftRotate[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ����������
Input: IN INFO_DATA_S *pstX OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
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

/*****************************************************************************
  Func Name: info_data_RightRotate[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ����������
Input: IN INFO_DATA_S *pstX OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
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

/*****************************************************************************
  Func Name: info_data_fixup_right[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ��������ʱ���-��
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_fixup_left(IN INFO_DATA_S **ppstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY;
    pstY = (*ppstZ)->pstparent->pstparent->pstright;
    if(pstY->encolor == INFO_COLOR_RED)
    {
        (*ppstZ)->pstparent->encolor = INFO_COLOR_BLACK;
        pstY->encolor = INFO_COLOR_BLACK;
        (*ppstZ)->pstparent->pstparent->encolor = INFO_COLOR_RED;
        *ppstZ = (*ppstZ)->pstparent->pstparent;
    }
    else
    {
        if(*ppstZ == (*ppstZ)->pstparent->pstright)
        {
            *ppstZ = (*ppstZ)->pstparent;
            DBGASSERT(info_data_LeftRotate(*ppstZ, ppstroot));
        }
        (*ppstZ)->pstparent->encolor = INFO_COLOR_BLACK;
        (*ppstZ)->pstparent->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_RightRotate((*ppstZ)->pstparent->pstparent, ppstroot));
    }
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: info_data_fixup_left[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ��������ʱ���-��
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_fixup_right(IN INFO_DATA_S **ppstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY;
    pstY = (*ppstZ)->pstparent->pstparent->pstleft;
    if(pstY->encolor == INFO_COLOR_RED)
    {
        (*ppstZ)->pstparent->encolor = INFO_COLOR_BLACK;
        pstY->encolor = INFO_COLOR_BLACK;
        (*ppstZ)->pstparent->pstparent->encolor = INFO_COLOR_RED;
        *ppstZ = (*ppstZ)->pstparent->pstparent;
    }
    else
    {

        if(*ppstZ == (*ppstZ)->pstparent->pstleft)
        {
            *ppstZ = (*ppstZ)->pstparent;
            DBGASSERT(info_data_RightRotate(*ppstZ, ppstroot));
        }
        (*ppstZ)->pstparent->encolor = INFO_COLOR_BLACK;
        (*ppstZ)->pstparent->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_LeftRotate((*ppstZ)->pstparent->pstparent, ppstroot));
    }
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: info_data_fixup[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ��������ʱ���
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_fixup(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{
    while(pstZ->pstparent->encolor == INFO_COLOR_RED)
    {
        if(pstZ->pstparent == pstZ->pstparent->pstparent->pstleft)
        {
            DBGASSERT(info_data_fixup_left(&pstZ, ppstroot));
        }
        else
        {
            DBGASSERT(info_data_fixup_right(&pstZ, ppstroot));
        }
    }
    (*ppstroot)->encolor = INFO_COLOR_BLACK;
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: info_data_LeftRotate[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ���������
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_insert(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY = g_pstnil;
    INFO_DATA_S *pstX = *ppstroot;
    while(pstX != g_pstnil)
    {
        pstY = pstX;
        if(pstZ->stCfg.uiId < pstX->stCfg.uiId)
        {
            pstX = pstX->pstleft;
        }
        else
        {
            pstX = pstX->pstright;
        }
    }
    pstZ->pstparent = pstY;
    if(pstY == g_pstnil)
    {
        *ppstroot = pstZ;
    }
    else if(pstZ->stCfg.uiId < pstY->stCfg.uiId)
    {
        pstY->pstleft = pstZ;
    }
    else
    {
        pstY->pstright = pstZ;
    }
    pstZ->pstleft = g_pstnil;
    pstZ->pstright = g_pstnil;
    pstZ->encolor = INFO_COLOR_RED;
    DBGASSERT(info_data_fixup(pstZ, ppstroot));
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: info_data_search[*]
  Date Created: 2016-07-31
Author: lichenda
Description: �ں�����в���
Input: IN UINT uiId             ����
IN INFO_DATA_S *pstroot  ���ڵ�
Output: OUT INFO_DATA_S*   �ڵ�ָ��
Return: ULONG, ERROR_SUCCESS     ����ɹ�
OTHER             ����ʧ��
Caution: ���ν��ڷ��سɹ�ʱ��Ч
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC INFO_DATA_S* info_data_search(IN UINT uiId, IN INFO_DATA_S *pstroot)
{
    INFO_DATA_S *pstX = pstroot;
    BOOL_T bFound = BOOL_FALSE;
    while(pstX != g_pstnil)
    {
        if(uiId == pstX->stCfg.uiId)
        {
            bFound = BOOL_TRUE;
            break;
        }
        else if(uiId < pstX->stCfg.uiId)
        {
            pstX = pstX->pstleft;
        }
        else
        {
            pstX = pstX->pstright;
        }
    }
    if(bFound)
    {
        return pstX;
    }
    else{
        return NULL;
    }
}

/*****************************************************************************
  Func Name: INFO_data_IsExist[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ���һ��Ա����Ϣ
Input: IN INFO_CFG_S stCfg
Output:
Return: BOOL_T, BOOL_TRUE    ��ӳɹ�
BOOL_FALSE   ���ʧ��
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
ULONG INFO_data_Add(IN const CHAR *pcInputStr)
{
    INFO_CFG_S stCfg;
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if(!INFO_ALL_ISVALID(&stCfg))
    {
        return ERROR_INVALID_PARAMETER;
    }

    if(NULL != info_data_search(stCfg.uiId, g_pstroot))
    {
        return ERROR_ALREADY_EXIST;
    }

    INFO_DATA_S *psttoadd = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    if(psttoadd == NULL)
    {
        return ERROR_NO_ENOUGH_RESOURCE;
    }

    psttoadd->stCfg = stCfg;
    if(!info_data_insert(psttoadd, &g_pstroot))
    {
        free(psttoadd);  
    }

    return ERROR_SUCCESS;
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
    if(NULL != info_data_search(uiId, g_pstroot))
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }

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
    if(g_pstroot == g_pstnil)
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
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
    INFO_DATA_S *pstresult = info_data_search(uiId, g_pstroot);
    if(pstresult == NULL)
    {
        return ERROR_FAILED;
    }
    else
    {
        *pstCfg = pstresult->stCfg;
        return ERROR_SUCCESS;
    }
}

/*****************************************************************************
  Func Name: info_data_minimum[*]
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: ��ȡ������С�ڵ�
Input: �������ڵ�
Output:
Return: INFO_DATA_S  ��С�ڵ�
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD
 *****************************************************************************/
INFO_DATA_S *info_data_minimum(INFO_DATA_S *pstX)
{
    while(pstX->pstleft != g_pstnil)
    {
        pstX = pstX->pstleft;
    }
    return pstX;
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
    INFO_DATA_S *pstX = g_pstroot;
    if(pstX == g_pstnil)
    {
        return INFO_ID_INVALID;
    }
    pstX = info_data_minimum(pstX);
    return pstX->stCfg.uiId;
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
    INFO_DATA_S *pstX = g_pstroot;

    if(pstX == g_pstnil)
    {
        return INFO_ID_INVALID;
    }

    UINT uitmp = 0;

    while(pstX != g_pstnil)
    {
        if(pstX->stCfg.uiId > uiId)
        {
            uitmp = pstX->stCfg.uiId;
            pstX = pstX->pstleft;
        }
        else
        {
            pstX = pstX->pstright;
        }

    }
    if(uitmp == 0)
    {
        return INFO_ID_INVALID;
    }
    else
    {
        return uitmp;
    }
}


/*****************************************************************************
  Func Name: info_data_DelFixUp_left[*]
  Date Created: 2016-07-30
Author: lichenda
Description: �����ɾ�����ʱ���-��
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_DelFixUp_left(IN INFO_DATA_S **ppstNode, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstW = (*ppstNode)->pstparent->pstright;
    if(pstW->encolor == INFO_COLOR_RED)
    {
        /*case 1 */
        pstW->encolor = INFO_COLOR_BLACK;
        (*ppstNode)->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_LeftRotate((*ppstNode)->pstparent, ppstroot));
        pstW = (*ppstNode)->pstparent->pstright;
    }
    if(pstW->pstleft->encolor == INFO_COLOR_BLACK &&
            pstW->pstright->encolor == INFO_COLOR_BLACK)
    {
        /*case 2 */
        pstW->encolor = INFO_COLOR_RED;
        *ppstNode = (*ppstNode)->pstparent;
    }
    else
    {
        if(pstW->pstright->encolor == INFO_COLOR_BLACK)
        {
            /*case 3 */
            pstW->pstleft->encolor = INFO_COLOR_BLACK;
            pstW->encolor = INFO_COLOR_RED;
            DBGASSERT(info_data_RightRotate(pstW, ppstroot));
            pstW = pstW->pstparent->pstright;
        }
        /*case 4 */
        pstW->encolor = (*ppstNode)->pstparent->encolor;
        (*ppstNode)->pstparent->encolor = INFO_COLOR_BLACK;
        pstW->pstright->encolor = INFO_COLOR_BLACK;
        DBGASSERT(info_data_LeftRotate((*ppstNode)->pstparent, ppstroot));
        *ppstNode = *ppstroot;

    }

    return BOOL_TRUE;
}
/*****************************************************************************
  Func Name: info_data_DelFixUp_right[*]
  Date Created: 2016-07-30
Author: lichenda
Description: �����ɾ�����ʱ���-��
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_DelFixUp_right(IN INFO_DATA_S **ppstNode, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstW = (*ppstNode)->pstparent->pstleft;
    if(pstW->encolor == INFO_COLOR_RED)
    {
        /*case 1 */
        pstW->encolor = INFO_COLOR_BLACK;
        (*ppstNode)->pstparent->encolor = INFO_COLOR_RED;
        DBGASSERT(info_data_RightRotate((*ppstNode)->pstparent, ppstroot));
        pstW = (*ppstNode)->pstparent->pstleft;
    }
    if(pstW->pstleft->encolor == INFO_COLOR_BLACK &&
            pstW->pstright->encolor == INFO_COLOR_BLACK)
    {
        /*case 2 */
        pstW->encolor = INFO_COLOR_RED;
        *ppstNode = (*ppstNode)->pstparent;
    }
    else
    {
        if(pstW->pstleft->encolor == INFO_COLOR_BLACK)
        {
            /*case 3 */
            pstW->pstright->encolor = INFO_COLOR_BLACK;
            pstW->encolor = INFO_COLOR_RED;
            DBGASSERT(info_data_LeftRotate(pstW, ppstroot));
            pstW = pstW->pstparent->pstleft;
        }
        /*case 4 */
        pstW->encolor = (*ppstNode)->pstparent->encolor;
        (*ppstNode)->pstparent->encolor = INFO_COLOR_BLACK;
        pstW->pstleft->encolor = INFO_COLOR_BLACK;
        DBGASSERT(info_data_RightRotate((*ppstNode)->pstparent, ppstroot));
        *ppstNode = *ppstroot;

    }

    return BOOL_TRUE;
}
/*****************************************************************************
  Func Name: info_data_DelFixUp[*]
  Date Created: 2016-07-30
Author: lichenda
Description: �����ɾ�����ʱ���
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_DelFixUp(IN INFO_DATA_S *pstNode, OUT INFO_DATA_S **ppstroot)
{
    while(pstNode != *ppstroot && pstNode->encolor == INFO_COLOR_BLACK)
    {
        if(pstNode == pstNode->pstparent->pstleft)
        {
            DBGASSERT(info_data_DelFixUp_left(&pstNode, ppstroot));
        }
        else
        {
            DBGASSERT(info_data_DelFixUp_right(&pstNode, ppstroot));
        }
    }
    pstNode->encolor = INFO_COLOR_BLACK;
    return BOOL_TRUE;
}

/*****************************************************************************
  Func Name: info_data_transplant[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ������ڵ����ֲ
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_transplant(IN const INFO_DATA_S *pstU,IN INFO_DATA_S *pstV, IN INFO_DATA_S **ppstroot)
{
    if(pstU->pstparent == g_pstnil)
    {
        *ppstroot = pstV;
    }
    else
    {
        if(pstU == pstU->pstparent->pstleft)
        {
            pstU->pstparent->pstleft = pstV;
        }
        else
        {
            pstU->pstparent->pstright = pstV;
        }
    }
    pstV->pstparent = pstU->pstparent;
    return BOOL_TRUE;

}

/*****************************************************************************
  Func Name: info_data_Del[*]
  Date Created: 2016-07-30
Author: lichenda
Description: �Ӻ����ɾ��ָ���ڵ�
Input: IN INFO_DATA_S *pstZ OUT INFO_DATA_S **ppstroot
Output:
Return: BOOL_T, BOOL_TRUE
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
STATIC BOOL_T info_data_Del(IN INFO_DATA_S *pstZ, OUT INFO_DATA_S **ppstroot)
{
    INFO_DATA_S *pstY = pstZ;
    INFO_DATA_S *pstX = NULL;
    INFO_COLOR_E enYOriginColor = pstY->encolor;
    if(pstZ->pstleft == g_pstnil)
    {
        pstX = pstZ->pstright;
        DBGASSERT(info_data_transplant(pstZ, pstZ->pstright, ppstroot));
    }
    else if(pstZ->pstright == g_pstnil)
    {
        pstX = pstZ->pstleft;
        DBGASSERT(info_data_transplant(pstZ, pstZ->pstleft, ppstroot));
    }
    else
    {
        pstY = info_data_minimum(pstZ->pstright);
        enYOriginColor = pstY->encolor;
        pstX = pstY->pstright;
        if(pstY->pstparent == pstZ)
        {
            pstX->pstparent = pstY;
        }
        else
        {
            DBGASSERT(info_data_transplant(pstY, pstY->pstright, ppstroot));
            pstY->pstright = pstZ->pstright;
            pstY->pstright->pstparent = pstY;
        }
        DBGASSERT(info_data_transplant(pstZ, pstY, ppstroot));
        pstY->pstleft = pstZ->pstleft;
        pstY->pstleft->pstparent = pstY;
        pstY->encolor = pstZ->encolor;
    }

    if(enYOriginColor == INFO_COLOR_BLACK)
    {
        DBGASSERT(info_data_DelFixUp(pstX, ppstroot));
    }
    return BOOL_TRUE;
}



/*****************************************************************************
  Func Name: INFO_data_Delete[*]
  Date Created: 2016-07-30
Author: lichenda
Description: ɾ��һ��Ա��
Input: IN UINT uiId
Output:
Return: ERROR_SUCCESS
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
ULONG INFO_data_Delete(IN UINT uiId)
{
    INFO_DATA_S *psttarget = info_data_search(uiId, g_pstroot);
    if(psttarget != NULL)
    {
        DBGASSERT(info_data_Del(psttarget, &g_pstroot));
        free(psttarget);
    }
    return ERROR_SUCCESS;
}

/*****************************************************************************
  Func Name: INFO_data_Modify[*]
  Date Created: 2016-07-30
Author: lichenda
Description: �޸�����
Input: IN UINT uiId
Output:
Return: ERROR_SUCCESS
Caution:
------------------------------------------------------------------------------
Modification History
DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------
YYYY-MM-DD

 *****************************************************************************/
ULONG INFO_data_Modify(IN const CHAR *pcInputStr)
{
    INFO_CFG_S stCfg;
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if(!INFO_ID_ISVALID(stCfg.uiId))
    {
        return ERROR_INVALID_PARAMETER;
    }
    INFO_DATA_S *pstresult = info_data_search(stCfg.uiId, g_pstroot);
    if(NULL == pstresult)
    {
        return ERROR_NOT_FOUND;
    }
    if(INFO_NAME_ISVALID(stCfg.szName))
    {
        strlcpy(pstresult->stCfg.szName, stCfg.szName, strlen(stCfg.szName));
    }
    if(INFO_SEX_ISVALID(stCfg.enSex))
    {
        pstresult->stCfg.enSex = stCfg.enSex;
    }
    if(INFO_AGE_ISVALID(stCfg.uiAge))
    {
        pstresult->stCfg.uiAge = stCfg.uiAge;
    }
    if(INFO_HEIGHT_ISVALID(stCfg.uiHeight))
    {
        pstresult->stCfg.uiHeight = stCfg.uiHeight;
    }
    return ERROR_SUCCESS;
        
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
    g_pstroot = g_pstnil;
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
