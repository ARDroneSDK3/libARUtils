/**
 * @file ARUTILS_Ftp.h
 * @brief libARUtils Ftp header file.
 * @date 19/12/2013
 * @author david.flattin.ext@parrot.com
 **/

#ifndef _ARUTILS_BLE_FTP_PRIVATE_H_
#define _ARUTILS_BLE_FTP_PRIVATE_H_

#include "libARUtils/ARUTILS_Manager.h"

@interface ARUtils_BLEFtp : NSObject


- (id)initWithPeripheral:(CBPeripheral *)peripheral cancelSem:(ARSAL_Sem_t*)cancelSem port:(int)port;
- (BOOL)registerCharacteristics;

- (BOOL)listFiles:(NSString*)remotePath list:(NSMutableString*)list;
- (BOOL)sizeFile:(NSString*)remoteFile fileSize:(double*)fileSize;
- (BOOL)getFile:(NSString*)remoteFile localFile:(NSString*)localFile progressCallback:(ARUTILS_Ftp_ProgressCallback_t)progressCallback progressArg:(void *)progressArg;
- (BOOL)getFileWithBuffer:(NSString*)remoteFile data:(uint8_t**)data dataLen:(uint32_t*)dataLen progressCallback:(ARUTILS_Ftp_ProgressCallback_t)progressCallback progressArg:(void *)progressArg;
- (BOOL)putFile:(NSString*)remoteFile localFile:(NSString*)localFile progressCallback:(ARUTILS_Ftp_ProgressCallback_t)progressCallback progressArg:(void *)progressArg resume:(BOOL)resume;
- (BOOL)abortPutFile:(NSString*)remoteFile;
- (BOOL)deleteFile:(NSString*)remoteFile;

@end


/**
 * @brief WifiFtp Connection structure
 * @param cancelSem The semaphore to cancel Ftp command
 * @param bleFtpObject The ble manager
 * @see ARUTILS_BLEFtp_Connection_New
 */
typedef struct _ARUTILS_BLEFtp_Connection_t_
{
    ARSAL_Sem_t *cancelSem;
    void *bleFtpObject;
    
} ARUTILS_BLEFtp_Connection_t;

/**
 * @brief Create a new Ftp Connection
 * @warning This function allocates memory
 * @param cancelSem The pointer of the Ftp get/put cancel semaphore or null
 * @param device The BLE Ftp device
 * @param[out] error The pointer of the error code: if success ARUTILS_OK, otherwise an error number of eARUTILS_ERROR
 * @retval On success, returns an ARUTILS_FtpAL_Connection_t. Otherwise, it returns null.
 * @see ARUTILS_FtpAL_DeleteConnection ()
 */
ARUTILS_BLEFtp_Connection_t * ARUTILS_BLEFtp_Connection_New(ARSAL_Sem_t *cancelSem, ARUTILS_BLEDevice_t device, int port, eARUTILS_ERROR *error);

/**
 * @brief Delete an Ftp Connection
 * @warning This function frees memory
 * @param connection The address of the pointer on the Ftp Connection
 * @see ARUTILS_FtpAL_NewConnection ()
 */
void ARUTILS_BLEFtp_Connection_Delete(ARUTILS_BLEFtp_Connection_t **connection);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_BLEFtp_Connection_Cancel(ARUTILS_BLEFtp_Connection_t *connection);

/**
 * @brief Check if the connection has received a cancel to it's semaphore
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see cURL
 */
eARUTILS_ERROR ARUTILS_BLEFtp_IsCanceled(ARUTILS_BLEFtp_Connection_t *connection);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_BLEFtp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_BLEFtp_List(ARUTILS_BLEFtp_Connection_t *connection, const char *remotePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Delete an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Ftp_NewConnection ()
 */
eARUTILS_ERROR ARUTILS_BLEFtp_Delete(ARUTILS_BLEFtp_Connection_t *connection, const char *remotePath);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_BLEFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtp_Get_WithBuffer(ARUTILS_BLEFtp_Connection_t *connection, const char *remotePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_BLEFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtp_Get(ARUTILS_BLEFtp_Connection_t *connection, const char *remotePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Put an remote Ftp server file
 * @param connection The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_BLEFtp_NewConnection (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtp_Put(ARUTILS_BLEFtp_Connection_t *connection, const char *remotePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param cancelSem The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_BLEFtp_IsCanceledSem(ARSAL_Sem_t *cancelSem);

/**
 * @brief Cancel an Ftp Connection command in progress (get, put, list etc)
 * @param connection The address of the pointer on the Ftp Connection
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_Connection_Cancel(ARUTILS_Manager_t *manager);
eARUTILS_ERROR ARUTILS_BLEFtpAL_Connection_IsCanceled(ARUTILS_Manager_t *manager);

/**
 * @brief Execute Ftp List command to retrieve directory content
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the directory path on the remote Ftp server
 * @param resultList The pointer of the string of the directory content null terminated
 * @param resultListLen The pointer of the lenght of the resultList string including null terminated
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_List(ARUTILS_Manager_t *manager, const char *namePath, char **resultList, uint32_t *resultListLen);

/**
 * @brief Get an remote Ftp server file
 * @warning This function allocates memory
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param data The pointer of the data buffer of the file data
 * @param dataLen The pointer of the length of the data buffer
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_Get_WithBuffer(ARUTILS_Manager_t *manager, const char *namePath, uint8_t **data, uint32_t *dataLen,  ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg);

/**
 * @brief Get an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param dstFile The string of the local file name path to be get
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_Get(ARUTILS_Manager_t *manager, const char *namePath, const char *dstFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Put an remote Ftp server file
 * @param manager The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @param srcFile The string of the local file name path to be put
 * @param progressCallback The progress callback function
 * @param progressArg The progress callback function arg
 * @param resume The resume capability requested
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp (), ARUTILS_Ftp_ProgressCallback_t, eARUTILS_FTP_RESUME
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_Put(ARUTILS_Manager_t *manager, const char *namePath, const char *srcFile, ARUTILS_Ftp_ProgressCallback_t progressCallback, void* progressArg, eARUTILS_FTP_RESUME resume);

/**
 * @brief Delete an remote Ftp server file
 * @param delete The address of the pointer on the Ftp Connection
 * @param namePath The string of the file name path on the remote Ftp server
 * @retval On success, returns ARUTILS_OK. Otherwise, it returns an error number of eARUTILS_ERROR.
 * @see ARUTILS_Manager_NewBLEFtp ()
 */
eARUTILS_ERROR ARUTILS_BLEFtpAL_Delete(ARUTILS_Manager_t *manager, const char *namePath);

#endif /* _ARUTILS_BLE_FTP_PRIVATE_H_ */