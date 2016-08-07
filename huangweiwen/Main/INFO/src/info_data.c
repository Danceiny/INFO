/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
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
#include <malloc.h>
#include <stdlib.h>

/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_proc.h"


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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;

    while ((NULL != pstNode) && (BOOL_FALSE == pstNode->isEmpty))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            return BOOL_TRUE;
        }
        else if (uiId > pstNode->stCfg.uiId)
        {
            pstNode = pstNode->pNext;
        }
        else
        {
            /* ����ʱ����id��С�������˵�������ж�Ӧid�Ľ��Ϊ�� */
            return BOOL_FALSE;
        }
    }
    /* ���������ĩβ��δ�ҵ� */
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
    /* �ж�ͷ����Ƿ�Ϊ�ռ��� */
    if (BOOL_TRUE == INFO_DATA_HEAD->isEmpty)
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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;

    /* �������� */
    while ((BOOL_FALSE == pstNode->isEmpty) && (NULL != pstNode))
    {
        if (uiId == pstNode->stCfg.uiId)
        {
            *pstCfg = pstNode->stCfg;
            return ERROR_SUCCESS;
        }
        else
        {
            pstNode = pstNode->pNext;
        }
    }
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
    if (BOOL_TRUE == INFO_DATA_HEAD->isEmpty)
    {
        return INFO_ID_INVALID;
    }
    else
    {
        return INFO_DATA_HEAD->stCfg.uiId;
    }
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
    INFO_DATA_S *pstNode = INFO_DATA_HEAD;
    UINT uiReId = INFO_ID_INVALID;

    /* �������� */
    while ((BOOL_FALSE == pstNode->isEmpty) && (NULL != pstNode))
    {
        if (uiId < pstNode->stCfg.uiId)
        {
            uiReId = pstNode->stCfg.uiId;
            break;
        }
        else
        {
            pstNode = pstNode->pNext;
        }
    }
    return uiReId;
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
    /* ������ͷ�������ڴ�ռ� */
    INFO_DATA_HEAD = (INFO_DATA_S *)malloc(sizeof(INFO_DATA_S));
    if (NULL == INFO_DATA_HEAD)
    {
        return ERROR_FAILED;
    }
    else
    {
        /* ��ʼ����������ṹ */
        INFO_DATA_HEAD->pPrior = NULL;
        INFO_DATA_HEAD->pNext = NULL;
        INFO_DATA_HEAD->isEmpty = BOOL_TRUE;
    }
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
    INFO_DATA_S *pstNode = NULL;

    /* ����ͷ��ʼ�������ͷ��ڴ� */
    while (NULL != INFO_DATA_HEAD)
    {
        pstNode = INFO_DATA_HEAD->pNext;
        free(INFO_DATA_HEAD);
        INFO_DATA_HEAD = pstNode;
    }
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

