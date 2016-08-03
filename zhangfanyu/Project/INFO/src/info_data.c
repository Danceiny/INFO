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

/* ��Ϣ���ݽṹ */
typedef struct tagInfo_Data
{
    struct tagInfo_Data *Info_Data_next;/* ������֯���[*] */
    INFO_CFG_S stCfg;          /* �������� */
}INFO_DATA_S;

/*ȫ�ֱ���������*/
INFO_DATA_S* pstHEAD;

/*******************************�ڲ�����***********************************/
/*�����ڴ沢�����뵽���ڴ�ռ����㣬���ؿռ��׵�ַ*/
INFO_DATA_S* info_data_Alloc()
{
    INFO_DATA_S* pstNode;
    pstNode=(INFO_DATA_S*)malloc(sizeof(INFO_DATA_S));
    memset(pstNode,0,sizeof(INFO_DATA_S));
    if(pstNode == NULL)
    {
        return NULL;
    }
    return pstNode;
}

/*�ͷ����ָ��ָ����ڴ�ռ�*/
VOID info_data_Free(IN INFO_DATA_S* pstNode)
{
    free(pstNode);
    pstNode=NULL;
}

/*�����ָ��ָ�������ڵ�(���չ��Ŵ�С�����˳��)��������ṹ����*/
/*˼·:���ж��Ƿ�Ϊ��������ѭ���ж�����ڵ����һ���ڵ�UID�Ƿ���ڲ���ڵ�
��Ϊ�գ������������ֱ�Ӳ���*/
VOID info_data_Add(IN INFO_DATA_S* pstNode)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    /*�������Ϊ�գ���ֱ�Ӳ������*/
    if(NULL == pstHEAD)
    {
        pstHEAD=pstNode;
        pstNode->Info_Data_next=NULL;
    }
    else
    {
        while(pstMvNode->Info_Data_next->stCfg.uiId < pstNode->stCfg.uiId
            &&pstMvNode->Info_Data_next != NULL)
        {
            pstMvNode=pstMvNode->Info_Data_next;
        }
        pstNode->Info_Data_next=pstMvNode->Info_Data_next;
        pstMvNode->Info_Data_next=pstNode;
    }
}

/*�����ָ��ָ�������ڵ������ṹ��ժ��*/
VOID info_data_Delete(IN INFO_DATA_S* pstNode)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode->Info_Data_next != NULL)
    {
        if(pstMvNode->Info_Data_next->stCfg.uiId == pstNode->stCfg.uiId)
        {
            pstMvNode->Info_Data_next=pstMvNode->Info_Data_next->Info_Data_next;
            return;
        }
        /*�������*/
        pstMvNode=pstMvNode->Info_Data_next;
    }
}

/*������ι��Ŷ�Ӧ������ڵ��ָ��*/
INFO_DATA_S* info_data_Get(IN UINT uiId)
{
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode != NULL)
    {
        if(pstMvNode->stCfg.uiId == uiId)
        {
            return pstMvNode;
        }
       /*�������*/
       pstMvNode=pstMvNode->Info_Data_next;
    }
    return NULL;
}
/**********************************�޸�����************************************/
/*����ι��ŵ�������Ϊ�������*/
VOID INFO_data_SetName(IN UINT uiId,IN CHAR szInName[])
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    strlcpy(pstCgNode->stCfg.szName,szInName,INFO_NAME_MAXLEN + 1);
}

/*����ι��ŵ��Ա��Ϊ����Ա�*/
VOID INFO_data_SetSex(IN UINT uiId,IN INFO_SEX_E enInSex)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.enSex=enInSex;
}
/*����ι��ŵ������Ϊ�������*/
VOID INFO_data_SetAge(IN UINT uiId,IN UINT uiInAge)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.uiAge=uiInAge;
}
/*����ι��ŵ���߸�Ϊ������*/
VOID INFO_data_SetHeight(IN UINT uiId,IN UINT uiInHeght)
{
    INFO_DATA_S* pstCgNode=info_data_Get(uiId);
    pstCgNode->stCfg.uiHeight=uiInHeght;
}


/*****************************************************************************
    Func Name: INFO_data_Create[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: Ϊ��ι��Ŵ���һ���ڵ㲢���ýڵ��������
        Input: IN UINT uiId         ����
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
UINT INFO_date_Create(IN UINT uiId)
{
    INFO_DATA_S* pstNode=info_data_Alloc();

    pstNode->stCfg.uiId=uiId;
    if(pstNode == NULL)
    {
        return ERROR_NO_ENOUGH_RESOURCE;
    }
    info_data_Add(pstNode);
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Destroy[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: Ϊ��ι��Ŵ���һ���ڵ㲢���ýڵ��������
        Input: IN UINT uiId         ����
       Output:
       Return:
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
VOID INFO_data_Destroy(IN UINT uiId)
{
    INFO_DATA_S* pstNode=info_data_Get(uiId);
    if(pstNode == NULL)
    {
        return;
    }
    else
    {
        info_data_Delete(pstNode);
        info_data_Free(pstNode);
    }
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
    if(info_data_Get(uiId) == NULL)
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
    if(pstHEAD == NULL)
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
    INFO_DATA_S *pstDataNode;
    pstDataNode=info_data_Get(uiId);

    if(pstDataNode == NULL)
    {
        return ERROR_FAILED;
    }
    else
    {
        *pstCfg=pstDataNode->stCfg;
        return ERROR_SUCCESS ;
    }

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
    INFO_DATA_S *pstFstNode=pstHEAD;
    if(pstFstNode == NULL)
    {
        return INFO_ID_INVALID;
    }
    else
    {
    return pstFstNode->stCfg.uiId;
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
    INFO_DATA_S *pstMvNode=pstHEAD;

    while(pstMvNode != NULL)
     {
         if(pstMvNode->stCfg.uiId > uiId)
         {
             return pstMvNode->stCfg.uiId;
         }
         /*�������*/
         pstMvNode=pstMvNode->Info_Data_next;
     }

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
    pstHEAD=NULL;

    if(pstHEAD==NULL)
    {
        return ERROR_SUCCESS;
    }
    else
    {
        return ERROR_FAILED;
    }
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
    INFO_DATA_S *pstMvNode=pstHEAD;
    INFO_DATA_S *pstTmpNode=pstHEAD;

    if(pstHEAD==NULL)
    {
        return;
    }

    while(pstMvNode->Info_Data_next != NULL)
    {
        pstTmpNode=pstMvNode;
        pstMvNode=pstMvNode->Info_Data_next;
        info_data_Free(pstTmpNode);
    }
    info_data_Free(pstMvNode);
    pstHEAD=NULL;
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

