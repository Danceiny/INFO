/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              info_proc.c
  Project Code: Comware V700R001
   Module Name: INFO
  Date Created: 2016-07-27
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
    Func Name: INFO_proc_DispSt[*]
 Date Created: 2016-07-28
       Author: xxxx 00000
  Description: ��ʾ�����ṹ��������
        Input: IN INFO_CFG_S stInputSt      ���ݽṹ��
       Output:

       Return: VOID
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  2014-11-05  peng yanyu       complete code
  --------------------------------------------------------------------------
  YYYY-MM-DD


*****************************************************************************/
VOID INFO_proc_DisStruct(IN INFO_CFG_S *pstInputStruct)
{
    printf("%u\t%s\t", (*pstInputStruct).uiId, (*pstInputStruct).szName);
    if (INFO_SEX_FEMALE == (*pstInputStruct).enSex)
    {
        printf("FEMALE\t");
    }
    else
    {
        printf("MALE\t");
    }
    printf("%u\t%u\r\n", (*pstInputStruct).uiAge, (*pstInputStruct).uiHeight);
}


/*****************************************************************************
    Func Name: INFO_proc_Display[*]
 Date Created: 2016-07-28
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
    IGNORE_PARAM(pcInputStr);

    UINT uiId = INFO_ID_INVALID;
    ULONG ulHaveGotData = ERROR_FAILED;
    INFO_CFG_S stData = { 0 };

    /* û���κ���Ϣ */
    if (BOOL_TRUE == INFO_data_IsEmpty())
    {
        printf("No info.\r\n");
    }
    else
    {
        printf("ID\tName\tSex\tAge\tHeight\r\n");

        /* ���λ�ȡ���������ݵĹ��ż�������Ϣ */
        uiId = INFO_data_GetFirst();
        ulHaveGotData = INFO_data_GetData(uiId, &stData);
        if (ERROR_SUCCESS == ulHaveGotData)
        {
            INFO_proc_DisStruct(&stData);
        }

        uiId = INFO_data_GetNext(uiId);
        while (INFO_ID_INVALID != uiId)
        {
            ulHaveGotData = INFO_data_GetData(uiId, &stData);
            if (ERROR_SUCCESS == ulHaveGotData)
            {
                INFO_proc_DisStruct(&stData);
            }
            uiId = INFO_data_GetNext(uiId);
        }
    }
    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_proc_Add[*]
 Date Created: 2016-07-29
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
    ULONG ulErrCode = ERROR_FAILED;
    const CHAR *pcErrInfo = NULL;
    INFO_CFG_S stCfg = { 0 };

    /* ���������ַ����õ��������ݲ�����stCfg�� */
    INFO_parse_InputStr(pcInputStr, &stCfg);
    if (INFO_ALL_ISVALID(&stCfg))
    {
        /* �������벻ȫ������ȡֵ�Ƿ� */
        pcErrInfo = "The parameter is incorrect.\r\n";
        ulErrCode = ERROR_INVALID_PARAMETER;
    }
    else if (BOOL_TRUE == INFO_data_IsExist(stCfg.uiId))
    {
        /* �����Ѿ����� */
        pcErrInfo = "The item already exists.\r\n";
        ulErrCode = ERROR_ALREADY_EXIST;
    }
    else
    {
        ulErrCode = INFO_data_AddData(&stCfg);

        if (ERROR_SUCCESS != ulErrCode)
        {
            /* ��Դ�������ʧ�� */
            pcErrInfo = "Not enough resources are available to complete the operation.\r\n";
        }
        else
        {
            /* ��ӳɹ� */
            pcErrInfo = "YOu can see the newly added info through Display function .\r\n";
        }
    }

    printf("%s", pcErrInfo);
    return ulErrCode;
}

/*****************************************************************************
    Func Name: INFO_proc_Delete[*]
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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
 Date Created: 2016-07-27
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

