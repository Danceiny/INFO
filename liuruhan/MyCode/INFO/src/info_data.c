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
#include "info_data.h"

/* struct len*/
#define INFO_CFG_S_LEN 32
/* ��ȡ���ݱ�־ */
BOOL_T info_data_Init = BOOL_FALSE;

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
    UINT uiLine = INFO_FIRST;
    for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
    {
        if(uiId == alData[uiLine].stCfg.uiId)
        {
            return BOOL_TRUE;
		}
	}

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
    UINT uiEndLine = INFO_FIRST;
    for(uiEndLine = INFO_DATA_MAX; alData[uiEndLine].stCfg.uiId == INFO_ID_INVALID; uiEndLine --);
	
    if(uiEndLine == INFO_FIRST)
    {
         return BOOL_FALSE;
    }
	else
		return BOOL_TRUE;
     
}

/*****************************************************************************
    Func Name: INFO_data_GetData[*]
 Date Created: 2016-08-02
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
    UINT uiLine;
	
	/* ���γ�ʼ��Ϊ�Ƿ�ֵ */    

	 for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine++)
	{
         if(alData[uiLine].stCfg.uiId == uiId)
         {
             pstCfg->enSex = alData[uiLine].stCfg.enSex;
	         strncpy(pstCfg->szName, alData[uiLine].stCfg.szName, sizeof(pstCfg->szName) - 1);		
	         pstCfg->uiAge= alData[uiLine].stCfg.uiAge;
       	     pstCfg->uiHeight= alData[uiLine].stCfg.uiHeight;
	         pstCfg->uiId= alData[uiLine].stCfg.uiId;
			 	return;
		 }
	}
	
	pstCfg->uiId = INFO_ID_INVALID;
	return;
}

/*****************************************************************************
    Func Name: INFO_data_GetFirst[*]
 Date Created: 2016-08-02
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
    if(alData[INFO_FIRST].stCfg.uiId != 0)
    {
	    return alData[INFO_FIRST].stCfg.uiId;
    }
	return INFO_ID_INVALID;
}

/*****************************************************************************
    Func Name: INFO_data_GetNext[*]
 Date Created: 2016-08-02
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
    UINT uiLine;
	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine++)
	{
	    if(alData[uiLine].stCfg.uiId == uiId)
	    {
	        return alData[uiLine+1].stCfg.uiId;
	    }
	}
    return INFO_ID_INVALID;
}

/*****************************************************************************
    Func Name: INFO_data_Init[*]
 Date Created: 2016-08-02
       Author: xxxx 00000
  Description: ģ���ʼ��
        Input: 
       Output: 
       Return: ULONG, ERROR_SUCCESS     ����ɹ�
                      OTHER             ����ʧ��
      Caution: 
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/
/**/ULONG INFO_data_Init(VOID)
{
	UINT uiDataLine = INFO_FIRST;	  

    for(uiDataLine = INFO_FIRST; uiDataLine < INFO_DATA_MAX; uiDataLine ++)
    {
        alData[uiDataLine].stCfg.uiId = INFO_ID_INVALID;
    }

	info_data_Init= BOOL_TRUE;

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: INFO_data_Fini[*]
 Date Created: 2016-08-02
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
    UINT uiLine = INFO_FIRST;

	if(info_data_Init == BOOL_FALSE)
    {
        printf("Forbidden!!!Did not use initial method!");
		return;
    }

	for(uiLine = INFO_FIRST; uiLine < INFO_DATA_MAX; uiLine ++)
    {
        alData[uiLine].stCfg.uiId = INFO_ID_INVALID;
    }

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

