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
    return ERROR_SUCCESS;
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
ULONG INFO_proc_Add(IN const CHAR *pcInputStr)
{
    INFO_CFG_S * pstCfg;    /*�û���������*/
    BOOL_T is_exist;    /*�ж��û��Ƿ��Ѿ�����*/
    ULONG is_success;   /*�ж��Ƿ�ɹ���������*/

    INFO_parse_InputStr(pcInputStr, pstCfg);  /*�����û����룬�õ�������������*/

    DBGASSERT(NULL != pstCfg);

    is_exist = INFO_data_IsExist(pstCfg->uiId);
    if(BOOL_FALSE == is_exist)
    {
        is_success = INFO_data_Create(pstCfg->uiId);
    }
    if(ERROR_SUCCESS == is_success)
    {
        INFO_data_SetName(pstCfg->uiId,pstCfg->szName);
        INFO_data_SetSex(pstCfg->uiId,pstCfg->enSex);
        INFO_data_SetAge(pstCfg->uiId,pstCfg->uiAge);
        INFO_data_SetHeight(pstCfg->uiId,pstCfg->uiHeight);
    }
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

