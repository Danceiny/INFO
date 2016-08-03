/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 201x-xx-xx
        Author: xxxx 00000
   Description: ���幦�ܴ���

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
#include <stdio.h>
#include <string.h>
/* system   public  */
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/string_ex.h>

/* module   private */
#include "info.h"
#include "info_dbg.h"
#include "info_parse.h"
#include "info_data.h"


/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ��ʾ
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:

       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
ULONG INFO_proc_Display(IN const CHAR *pcInputStr)
{
    UINT uiId;
    INFO_CFG_S  stCfg;
    INFO_CFG_S *pstCfg=&stCfg;
    if(BOOL_TRUE == INFO_data_IsEmpty())
    {
        printf("\n No info\n");
        return ERROR_FAILED;
    }
    else
    {
         for ((uiId)  = INFO_data_GetFirst();
         (uiId) != INFO_ID_INVALID;
         (uiId) = INFO_data_GetNext(uiId))
         {
            INFO_data_GetData(uiId,pstCfg);
            printf("ID:%d\tName:%s\tSex:%d\tAge:%d\tHeight:%d\n",
            pstCfg->uiId,pstCfg->szName,pstCfg->enSex,pstCfg->uiAge,pstCfg->uiHeight);
         }
         return ERROR_SUCCESS;
    }
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
    /*�жϲ����Ƿ���Ч*/
BOOL_T info_procAdd_Isvalid(INFO_CFG_S *pstCfg)
{
    UINT uiNamesize=strlen(pstCfg->szName);
    BOOL_T uiIdValid=(pstCfg->uiId <= 100000 && pstCfg->uiId >=1);
    BOOL_T szNameValid=(uiNamesize <= 15);
    BOOL_T enSexValid=(pstCfg->enSex == 1 || pstCfg->enSex == 2);
    BOOL_T uiHeightValid=(pstCfg->uiHeight <= 300 && pstCfg->uiHeight >= 1);
    BOOL_T uiAgeValid=(pstCfg->uiAge <= 300 && pstCfg->uiAge>= 1);




    if(uiIdValid == 0 || szNameValid == 0 || enSexValid == 0
        || uiHeightValid == 0 || uiAgeValid == 0)
    {
        return BOOL_FALSE;
    }
    else
    {
       return BOOL_TRUE;
    }
}

    /*������֪���ŵĸ�����*/
VOID info_proc_SetCfg(INFO_CFG_S *pstCfg)
{
    UINT uiId=pstCfg->uiId;
    INFO_data_SetName(uiId,pstCfg->szName);
    INFO_data_SetSex(uiId,pstCfg->enSex);
    INFO_data_SetAge(uiId,pstCfg->uiAge);
    INFO_data_SetHeight(uiId,pstCfg->uiHeight);

}

ULONG INFO_proc_Add(IN const CHAR *pcInputStr)
{
    INFO_CFG_S  stCfg;
    INFO_CFG_S *pstCfg=&stCfg;
    /*�ַ�������*/
    INFO_parse_InputStr(pcInputStr,pstCfg);
    /*�жϲ����Ƿ���Ч*/
    if(BOOL_FALSE == info_procAdd_Isvalid(pstCfg))
    {
       return ERROR_INVALID_PARAMETER;
    }
    if(BOOL_TRUE == INFO_data_IsExist(pstCfg->uiId))
    {
        return ERROR_ALREADY_EXIST;
    }
    INFO_date_Create(pstCfg->uiId);
    info_proc_SetCfg(pstCfg);
    INFO_proc_Display(pcInputStr);
    return ERROR_SUCCESS;
}


/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: ɾ��
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Delete(IN const CHAR *pcInputStr)
{
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Modify[*]
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �޸�
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution: ֧�ֽ��޸Ĳ�����������
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Modify(IN const CHAR *pcInputStr)
{
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Exit
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �˳�
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_Exit(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_OpenDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �򿪵��Կ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_OpenDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_TRUE);

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_CloseDebug
 Date Created: 201x-xx-xx
       Author: xxxx 00000
  Description: �رյ��Կ���
        Input: IN const CHAR *pcInputStr    �����ַ���
       Output:
       Return: ULONG, ERROR_SUCCESS         ����ɹ�
                      OTHER                 ����ʧ��
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
ULONG INFO_proc_CloseDebug(IN const CHAR *pcInputStr)
{
    IGNORE_PARAM(pcInputStr);

    INFO_dbg_Set(BOOL_FALSE);

    return ERROR_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

