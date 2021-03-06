/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_data.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2016-07-28
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

/* 信息数据结构 */
typedef struct tagInfo_Data
{
    struct tagInfo_Data *pstNext;   /* 数据组织相关[*]，指向下一个节点 */
    BOOL_T bIsEmpty;            /* 数据组织相关[*]，数据是否为空，TRUE表示空 */
    INFO_CFG_S stCfg;           /* 配置数据 */
}INFO_DATA_S;

/* 数据链表头指针 */
INFO_DATA_S *g_pstINFO_DATA_HEAD = NULL;


/*****************************************************************************
    Func Name: INFO_data_FindNodeById[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: 查找指定工号的节点是否存在
        Input: IN UINT uiId         工号
       Output:
       Return: INFO_DATA_S *, != NULL     工号所在节点指针
                              == NULL     未找到
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
INFO_DATA_S *INFO_data_FindNodeById(IN UINT uiId)
{
    BOOL_T bRet = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* 遍历数据链表，查找工号 */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            bRet = BOOL_TRUE;
            break;
        }
        else if (uiId < pstNode->stCfg.uiId)
        {
            bRet = BOOL_FALSE;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    if (BOOL_TRUE == bRet)
    {
        return pstNode;
    }
    else
    {
        return NULL;
    }
}

/*****************************************************************************
    Func Name: INFO_data_IsExist[*]
 Date Created: 2016-07-28
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
    if (NULL == INFO_data_FindNodeById(uiId))
    {
        return BOOL_FALSE;
    }
    else
    {
        return BOOL_TRUE;
    }
}

/*****************************************************************************
    Func Name: INFO_data_IsEmpty[*]
 Date Created: 2016-07-28
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
    /* 判断头节点的数据是否为空 */
    if (BOOL_TRUE == g_pstINFO_DATA_HEAD->bIsEmpty)
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
 Date Created: 2016-07-28
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
    INFO_DATA_S *pstNode = INFO_data_FindNodeById(uiId);

    if (NULL != pstNode)
    {
        *pstCfg = pstNode->stCfg;
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 2016-07-28
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
    if (BOOL_TRUE == INFO_data_IsEmpty())
    {
        return INFO_ID_INVALID;
    }
    else
    {
        /* 第一个有数据的工号在头节点处 */
        return g_pstINFO_DATA_HEAD->stCfg.uiId;
    }
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 2016-07-28
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
    UINT uiRet = INFO_ID_INVALID;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* 遍历数据链表，查找工号 */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (uiId < pstNode->stCfg.uiId)
        {
            /* 比uiId大的第一个工号的节点 */
            uiRet = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    return uiRet;
}

/*****************************************************************************
    Func Name: INFO_data_GetLast[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: 获取前一个有数据工号
        Input: IN UINT uiId                 当前工号
       Output:
       Return: UINT, != INFO_ID_INVALID     前一个工号
                     == INFO_ID_INVALID     未找到
      Caution: 此接口获取下一个工号不依赖于入参uiId本身是否有数据
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_data_GetLast(IN UINT uiId)
{
    UINT uiRet = INFO_ID_INVALID;
    BOOL_T bIs_uiId = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;

    /* 遍历数据链表，查找工号 */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        /* 下一个节点为指定工号所在节点 */
        bIs_uiId = (NULL != pstNode->pstNext) \
            && (BOOL_TRUE != pstNode->pstNext->bIsEmpty) \
            && (uiId == pstNode->pstNext->stCfg.uiId);
        if ((uiId > pstNode->stCfg.uiId) && bIs_uiId)
        {
            /* 比uiId小的第一个工号的节点 */
            uiRet = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    return uiRet;
}

/*****************************************************************************
    Func Name: INFO_data_AddData[*]
 Date Created: 2016-07-29
       Author: xxxx 00000
  Description: 增加一个数据节点
        Input: IN INFO_CFG_S *pstCfg    新增数据
       Output:
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution: 此接口不检查工号是否已经存在，需外部函数进行检查
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_AddData(IN INFO_CFG_S const *pstCfg)
{
    ULONG ulRet = ERROR_FAILED;
    BOOL_T bAddable = BOOL_FALSE;
    BOOL_T bAddable_next = BOOL_FALSE;
    BOOL_T bFree_node = BOOL_FALSE;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pstNewNode = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));

    /* 判断系统能否再分配内存 */
    if (NULL == pstNewNode)
    {
        ulRet = ERROR_NO_ENOUGH_RESOURCE;

        /* 不再遍历链表 */
        pstNode = NULL;
    }
    else
    {
        if (BOOL_TRUE == pstNode->bIsEmpty)
        {
            /* 头节点为空，则直接在头节点写入数据 */
            bFree_node = BOOL_TRUE;
            ulRet = ERROR_SUCCESS;
            pstNode->stCfg = *pstCfg;
            pstNode->bIsEmpty = BOOL_FALSE;

            /* 不再遍历链表 */
            pstNode = NULL;
        }
        else if (INFO_data_GetFirst() > pstCfg->uiId)
        {
            /* 头节点的工号比要增加的节点工号大，直接在头节点处插入一个节点 */
            ulRet = ERROR_SUCCESS;
            g_pstINFO_DATA_HEAD = pstNewNode;
            g_pstINFO_DATA_HEAD->pstNext = pstNode;
            g_pstINFO_DATA_HEAD->bIsEmpty = BOOL_FALSE;
            g_pstINFO_DATA_HEAD->stCfg = *pstCfg;

            /* 不再遍历链表 */
            pstNode = NULL;
        }
    }

    /* 遍历数据链表，定位插入工号的位置 */
    while ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        bAddable_next = (NULL == pstNode->pstNext) || (pstCfg->uiId < pstNode->pstNext->stCfg.uiId);
        bAddable = (pstCfg->uiId > pstNode->stCfg.uiId) && bAddable_next;
        if (bAddable && (NULL != pstNewNode))
        {
            ulRet = ERROR_SUCCESS;
            pstNewNode->pstNext = pstNode->pstNext;
            pstNewNode->bIsEmpty = BOOL_FALSE;
            pstNewNode->stCfg = *pstCfg;
            pstNode->pstNext = pstNewNode;
            break;
        }
        else
        {
            pstNode = pstNode->pstNext;
        }
    }

    if (bFree_node)
    {
        free(pstNewNode);
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: INFO_data_DelData[*]
 Date Created: 2016-07-29
       Author: xxxx 00000
  Description: 删除一个数据节点
        Input: IN UINT uiId                  要删除的节点工号
        Output:
        Return: ULONG, ERROR_SUCCESS         处理成功
                       OTHER                 处理失败
      Caution: 此接口不检查工号是否已经存在，需外部函数进行检查
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_DelData(IN UINT uiId)
{
    UINT uiId_last = INFO_ID_INVALID;
    ULONG ulRet = ERROR_SUCCESS;
    INFO_DATA_S *pstNode = g_pstINFO_DATA_HEAD;
    INFO_DATA_S *pstNode_last = NULL;

    if(uiId == pstNode->stCfg.uiId)
    {
        /* 要删除的节点是头节点 */
        ulRet = ERROR_SUCCESS;
        if (NULL == pstNode->pstNext)
        {
            /* 若头节点后面没有节点，则只清除头节点上的数据 */
            memset(&(pstNode->stCfg), 0, sizeof(INFO_CFG_S));
            pstNode->bIsEmpty = BOOL_TRUE;
        }
        else
        {
            g_pstINFO_DATA_HEAD = g_pstINFO_DATA_HEAD->pstNext;
            free(pstNode);
        }
    }
    else
    {
        /* 要删除的节点不是头节点 */
        uiId_last = INFO_data_GetLast(uiId);
        if (INFO_ID_INVALID != uiId_last)
        {
            ulRet = ERROR_SUCCESS;
            pstNode_last = INFO_data_FindNodeById(uiId_last);
            if (NULL != pstNode_last)
            {
                pstNode = pstNode_last->pstNext;
                pstNode_last->pstNext = pstNode->pstNext;
                free(pstNode);
            }
        }
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: INFO_data_ModifyData[*]
 Date Created: 2016-07-30
       Author: xxxx 00000
  Description: 修改一个节点的数据
        Input: IN INFO_CFG_S *pstCfg         要修改的数据
        Output:
        Return: ULONG, ERROR_SUCCESS         处理成功
                       OTHER                 处理失败
      Caution: 此接口不检查工号是否已经存在，需外部函数进行检查
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_ModifyData(IN INFO_CFG_S const *pstCfg)
{
    INFO_DATA_S *pstNode = INFO_data_FindNodeById(pstCfg->uiId);
    if ((NULL != pstNode) && (BOOL_TRUE != pstNode->bIsEmpty))
    {
        if (INFO_NAME_ISVALID(pstCfg->szName))
        {
            (VOID) strlcpy(pstNode->stCfg.szName, pstCfg->szName, sizeof(pstNode->stCfg.szName));
        }
        if (INFO_SEX_ISVALID(pstCfg->enSex))
        {
            pstNode->stCfg.enSex = pstCfg->enSex;
        }
        if (INFO_AGE_ISVALID(pstCfg->uiAge))
        {
            pstNode->stCfg.uiAge = pstCfg->uiAge;
        }
        if (INFO_HEIGHT_ISVALID(pstCfg->uiHeight))
        {
            pstNode->stCfg.uiHeight = pstCfg->uiHeight;
        }
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_NOT_FOUND;
    }
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: 模块初始化
        Input:
       Output:
       Return: ULONG, ERROR_SUCCESS     处理成功
                      OTHER             处理失败
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_data_Init(VOID)
{
    ULONG ulRet = ERROR_SUCCESS;

    /* 创建数据链表头节点 */
    g_pstINFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));

    if (NULL == g_pstINFO_DATA_HEAD)
    {
        ulRet = ERROR_NO_ENOUGH_RESOURCE;
    }
    else
    {
        /* 初始化头节点 */
        g_pstINFO_DATA_HEAD->pstNext = NULL;
        g_pstINFO_DATA_HEAD->bIsEmpty = BOOL_TRUE;
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 2016-07-28
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
    INFO_DATA_S *pstNode = NULL;

    /* 遍历数据链表，释放数据链表的内存 */
    while (NULL != g_pstINFO_DATA_HEAD)
    {
        pstNode = g_pstINFO_DATA_HEAD->pstNext;
        free(g_pstINFO_DATA_HEAD);
        g_pstINFO_DATA_HEAD = pstNode;
    }

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

