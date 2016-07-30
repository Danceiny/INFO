/*******************************************************************************
  Copypstright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All pstrights reserved.
  --------------------------------------------------------------------------------
  info_data.c
  Project Code: Comware V700R001
  Module Name: INFO
  Date Created: 201x-xx-xx
Author: xxxx 00000
Description: 内部数据操作
包括创建、删除、设置、获取、遍历

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

/*红黑树颜色*/
typedef enum tagColor{
    INFO_COLOR_RED = 1,
    INFO_COLOR_BLACK = 2
} INFO_COLOR_E;

/* 信息数据结构 */
typedef struct tagInfo_Data
{
    INFO_COLOR_E encolor;     /* 颜色 */
    struct tagInfo_Data *pstleft;     /* 左子树 */
    struct tagInfo_Data *pstright;    /* 右子树 */ 
    struct  tagInfo_Data *pstparent;   /* 父节点 */ 
    INFO_CFG_S stCfg;       /* 配置数据 */
}INFO_DATA_S;

/*红黑树nil指针指向nill节点*/
INFO_DATA_S g_stnill;
STATIC INFO_DATA_S * const g_pstnil = &g_stnill;

/*根节点*/
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
Description: 判断指定工号的数据是否存在
Input: IN UINT uiId         工号
Output: 
Return: BOOL_T, BOOL_TRUE    存在
BOOL_FALSE   不存在
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
Description: 判断整个数据组织是否为空
Input: 
Output: 
Return: BOOL_T, BOOL_TRUE    数据组织为空
BOOL_FALSE   数据组织非空
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
Description: 获取配置数据
Input: IN UINT uiId             工号
Output: OUT INFO_CFG_S *pstCfg   配置数据
Return: ULONG, ERROR_SUCCESS     处理成功
OTHER             处理失败
Caution: 出参仅在返回成功时有效
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
Description: 获取第一个有数据工号
Input: VOID
Output: 
Return: UINT, != INFO_ID_INVALID     第一个有数据的工号
== INFO_ID_INVALID     未找到
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
Description: 获取下一个有数据工号
Input: IN UINT uiId                 当前工号
Output: 
Return: UINT, != INFO_ID_INVALID     下一个工号
== INFO_ID_INVALID     未找到
Caution: 此接口获取下一个工号不依赖于入参uiId本身是否有数据
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
Description: 模块初始化
Input: 
Output: 
Return: ULONG, ERROR_SUCCESS     处理成功
OTHER             处理失败
Caution: 目前始终成功
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
Description: 模块退出
Input: 
Output: 
Return: VOID
Caution: 调用此接口前，必须已经初始化过
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

