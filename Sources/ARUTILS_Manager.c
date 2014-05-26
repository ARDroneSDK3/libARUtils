/**
 * @file ARUTILS_Manager.c
 * @brief libARUtils Manager c file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <libARSAL/ARSAL_Sem.h>
#include <libARSAL/ARSAL_Print.h>
#include <curl/curl.h>

#include "libARUtils/ARUTILS_Error.h"
#include "libARUtils/ARUTILS_Manager.h"
#include "libARUtils/ARUTILS_Ftp.h"
#include "ARUTILS_Manager.h"

#define ARUTILS_MANAGER_TAG "Manager"



ARUTILS_Manager_t* ARUTILS_Manager_New(eARUTILS_ERROR *error)
{
    ARUTILS_Manager_t *newManager = NULL;
    eARUTILS_ERROR result = ARUTILS_OK;
    
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MANAGER_TAG, "");
    
    newManager = calloc(1, sizeof(ARUTILS_Manager_t));
    if (newManager == NULL)
    {
        result = ARUTILS_ERROR_ALLOC;
    }
    
    *error = result;
    return newManager;
}

void ARUTILS_Manager_Delete(ARUTILS_Manager_t **managerAddr)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, ARUTILS_MANAGER_TAG, "");
    
    if (managerAddr != NULL)
    {
        ARUTILS_Manager_t *manager = *managerAddr;
        if (manager != NULL)
        {
            free(manager);
        }
        *managerAddr = NULL;
    }
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_Cancel(ARUTILS_Manager_t *manager)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpConnectionCancel == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpConnectionCancel(manager);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Connection_IsCanceled(ARUTILS_Manager_t *manager)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    
    if ((manager == NULL) || (manager->ftpConnectionIsCanceled == NULL))
    {
        printf("===============> %p, %p", manager, manager->ftpConnectionIsCanceled);
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpConnectionIsCanceled(manager);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_List(ARUTILS_Manager_t *manager, const char *namePath, char **resultList, uint32_t *resultListLen)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpList == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpList(manager, namePath, resultList, resultListLen);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Get_WithBuffer(ARUTILS_Manager_t *manager, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpGetWithBuffer == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpGetWithBuffer(manager, namePath, data, dataLen, progressCallback, progressArg);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Get(ARUTILS_Manager_t *manager, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpGet == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpGet(manager, namePath, dstFile, progressCallback, progressArg, resume);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Put(ARUTILS_Manager_t *manager, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpPut == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpPut(manager, namePath, srcFile, progressCallback, progressArg, resume);
    }
    return result;
}

eARUTILS_ERROR ARUTILS_Manager_Ftp_Delete(ARUTILS_Manager_t *manager, const char *namePath)
{
    eARUTILS_ERROR result = ARUTILS_OK;
    if ((manager == NULL) || (manager->ftpDelete == NULL))
    {
        result = ARUTILS_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = manager->ftpDelete(manager, namePath);
    }
    return result;
}
