/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2015, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** $Workfile: lmerr.cpp $
** $Revision: 22 $
** $Modtime: 6/26/01 10:53a $
** $Reuse Tracing Code: 1 $
*/

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

USING_WFC_NAMESPACE

// These error codes come from lmerr.h

void PASCAL Win32FoundationClasses::Convert_NERR_Code_to_String( _In_ DWORD const error_code, _Out_ std::wstring& error_message ) noexcept
{
   switch( error_code )
   {
      case NERR_Success:

         error_message.assign(WSTRING_VIEW(L"Success." ) );
         return;

      case NERR_NetNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The workstation driver is not installed." ) );
         return;

      case NERR_UnknownServer:
   
         error_message.assign(WSTRING_VIEW(L"The server could not be located." ) );
         return;

      case NERR_ShareMem:
   
         error_message.assign(WSTRING_VIEW(L"An internal error occurred. The network cannot access a shared memory segment." ) );
         return;

      case NERR_NoNetworkResource:
   
         error_message.assign(WSTRING_VIEW(L"A network resource shortage occurred." ) );
         return;

      case NERR_RemoteOnly:
   
         error_message.assign(WSTRING_VIEW(L"This operation is not supported on workstations." ) );
         return;

      case NERR_DevNotRedirected:
   
         error_message.assign(WSTRING_VIEW(L"The device is not connected." ) );
         return;

      case NERR_ServerNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The Server service is not started." ) );
         return;

      case NERR_ItemNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The queue is empty." ) );
         return;

      case NERR_UnknownDevDir:
   
         error_message.assign(WSTRING_VIEW(L"The device or directory does not exist." ) );
         return;

      case NERR_RedirectedPath:
   
         error_message.assign(WSTRING_VIEW(L"The operation is invalid on a redirected resource." ) );
         return;

      case NERR_DuplicateShare:
   
         error_message.assign(WSTRING_VIEW(L"The name has already been shared." ) );
         return;

      case NERR_NoRoom:
   
         error_message.assign(WSTRING_VIEW(L"The server is currently out of the requested resource." ) );
         return;

      case NERR_TooManyItems:
   
         error_message.assign(WSTRING_VIEW(L"Requested addition of items exceeds the maximum allowed." ) );
         return;

      case NERR_InvalidMaxUsers:
   
         error_message.assign(WSTRING_VIEW(L"The Peer service supports only two simultaneous users." ) );
         return;

      case NERR_BufTooSmall:
   
         error_message.assign(WSTRING_VIEW(L"The API return buffer is too small." ) );
         return;

      case NERR_RemoteErr:
   
         error_message.assign(WSTRING_VIEW(L"A remote API error occurred." ) );
         return;

      case NERR_LanmanIniError:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred when opening or reading the configuration file." ) );
         return;

      case NERR_NetworkError:
   
         error_message.assign(WSTRING_VIEW(L"A general network error occurred." ) );
         return;

      case NERR_WkstaInconsistentState:
   
         error_message.assign(WSTRING_VIEW(L"The Workstation service is in an inconsistent state. Restart the computer before restarting the Workstation service." ) );
         return;

      case NERR_WkstaNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The Workstation service has not been started." ) );
         return;

      case NERR_BrowserNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The requested information is not available." ) );
         return;

      case NERR_InternalError:
   
         error_message.assign(WSTRING_VIEW(L"An internal Windows NT error occurred." ) );
         return;

      case NERR_BadTransactConfig:
   
         error_message.assign(WSTRING_VIEW(L"The server is not configured for transactions." ) );
         return;

      case NERR_InvalidAPI:
   
         error_message.assign(WSTRING_VIEW(L"The requested API is not supported on the remote server." ) );
         return;

      case NERR_BadEventName:
   
         error_message.assign(WSTRING_VIEW(L"The event name is invalid." ) );
         return;

      case NERR_DupNameReboot:
   
         error_message.assign(WSTRING_VIEW(L"The computer name already exists on the network. Change it and restart the computer." ) );
         return;

      case NERR_CfgCompNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The specified component could not be found in the configuration information." ) );
         return;

      case NERR_CfgParamNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The specified parameter could not be found in the configuration information." ) );
         return;

      case NERR_LineTooLong:
   
         error_message.assign(WSTRING_VIEW(L"A line in the configuration file is too long." ) );
         return;

      case NERR_QNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The printer does not exist." ) );
         return;

      case NERR_JobNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The print job does not exist." ) );
         return;

      case NERR_DestNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The printer destination cannot be found." ) );
         return;

      case NERR_DestExists:
   
         error_message.assign(WSTRING_VIEW(L"The printer destination already exists." ) );
         return;

      case NERR_QExists:
   
         error_message.assign(WSTRING_VIEW(L"The printer queue already exists." ) );
         return;

      case NERR_QNoRoom:
   
         error_message.assign(WSTRING_VIEW(L"No more printers can be added." ) );
         return;

      case NERR_JobNoRoom:
   
         error_message.assign(WSTRING_VIEW(L"No more print jobs can be added." ) );
         return;

      case NERR_DestNoRoom:
   
         error_message.assign(WSTRING_VIEW(L"No more printer destinations can be added." ) );
         return;

      case NERR_DestIdle:
   
         error_message.assign(WSTRING_VIEW(L"This printer destination is idle and cannot accept control operations." ) );
         return;

      case NERR_DestInvalidOp:
   
         error_message.assign(WSTRING_VIEW(L"This printer destination request contains an invalid control function." ) );
         return;

      case NERR_ProcNoRespond:
   
         error_message.assign(WSTRING_VIEW(L"The print processor is not responding." ) );
         return;

      case NERR_SpoolerNotLoaded:
   
         error_message.assign(WSTRING_VIEW(L"The spooler is not running." ) );
         return;

      case NERR_DestInvalidState:
   
         error_message.assign(WSTRING_VIEW(L"This operation cannot be performed on the print destination in its current state." ) );
         return;

      case NERR_QInvalidState:
   
         error_message.assign(WSTRING_VIEW(L"This operation cannot be performed on the printer queue in its current state." ) );
         return;

      case NERR_JobInvalidState:
   
         error_message.assign(WSTRING_VIEW(L"This operation cannot be performed on the print job in its current state." ) );
         return;

      case NERR_SpoolNoMemory:
   
         error_message.assign(WSTRING_VIEW(L"A spooler memory allocation failure occurred." ) );
         return;

      case NERR_DriverNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The device driver does not exist." ) );
         return;

      case NERR_DataTypeInvalid:
   
         error_message.assign(WSTRING_VIEW(L"The data type is not supported by the print processor." ) );
         return;

      case NERR_ProcNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The print processor is not installed." ) );
         return;

      case NERR_ServiceTableLocked:
   
         error_message.assign(WSTRING_VIEW(L"The service database is locked." ) );
         return;

      case NERR_ServiceTableFull:
   
         error_message.assign(WSTRING_VIEW(L"The service table is full." ) );
         return;

      case NERR_ServiceInstalled:
   
         error_message.assign(WSTRING_VIEW(L"The requested service has already been started." ) );
         return;

      case NERR_ServiceEntryLocked:
   
         error_message.assign(WSTRING_VIEW(L"The service does not respond to control actions." ) );
         return;

      case NERR_ServiceNotInstalled:
   
         error_message.assign(WSTRING_VIEW(L"The service has not been started." ) );
         return;

      case NERR_BadServiceName:
   
         error_message.assign(WSTRING_VIEW(L"The service name is invalid." ) );
         return;

      case NERR_ServiceCtlTimeout:
   
         error_message.assign(WSTRING_VIEW(L"The service is not responding to the control function." ) );
         return;

      case NERR_ServiceCtlBusy:
   
         error_message.assign(WSTRING_VIEW(L"The service control is busy." ) );
         return;

      case NERR_BadServiceProgName:
   
         error_message.assign(WSTRING_VIEW(L"The configuration file contains an invalid service program name." ) );
         return;

      case NERR_ServiceNotCtrl:
   
         error_message.assign(WSTRING_VIEW(L"The service could not be controlled in its present state." ) );
         return;

      case NERR_ServiceKillProc:
   
         error_message.assign(WSTRING_VIEW(L"The service ended abnormally." ) );
         return;

      case NERR_ServiceCtlNotValid:
   
         error_message.assign(WSTRING_VIEW(L"The requested pause or stop is not valid for this service." ) );
         return;

      case NERR_NotInDispatchTbl:
   
         error_message.assign(WSTRING_VIEW(L"The service control dispatcher could not find the service name in the dispatch table." ) );
         return;

      case NERR_BadControlRecv:
   
         error_message.assign(WSTRING_VIEW(L"The service control dispatcher pipe read failed." ) );
         return;

      case NERR_ServiceNotStarting:
   
         error_message.assign(WSTRING_VIEW(L"A thread for the new service could not be created." ) );
         return;

      case NERR_AlreadyLoggedOn:
   
         error_message.assign(WSTRING_VIEW(L"This workstation is already logged on to the local-area network." ) );
         return;

      case NERR_NotLoggedOn:
   
         error_message.assign(WSTRING_VIEW(L"The workstation is not logged on to the local-area network." ) );
         return;

      case NERR_BadUsername:
   
         error_message.assign(WSTRING_VIEW(L"The user name or group name parameter is invalid." ) );
         return;

      case NERR_BadPassword:
   
         error_message.assign(WSTRING_VIEW(L"The password parameter is invalid." ) );
         return;

      case NERR_UnableToAddName_W:
   
         error_message.assign(WSTRING_VIEW(L"@W The logon processor did not add the message alias." ) );
         return;

      case NERR_UnableToAddName_F:
   
         error_message.assign(WSTRING_VIEW(L"The logon processor did not add the message alias." ) );
         return;

      case NERR_UnableToDelName_W:
   
         error_message.assign(WSTRING_VIEW(L"@W The logoff processor did not delete the message alias." ) );
         return;

      case NERR_UnableToDelName_F:
   
         error_message.assign(WSTRING_VIEW(L"The logoff processor did not delete the message alias." ) );
         return;

      case NERR_LogonsPaused:
   
         error_message.assign(WSTRING_VIEW(L"Network logons are paused." ) );
         return;

      case NERR_LogonServerConflict:
   
         error_message.assign(WSTRING_VIEW(L"A centralized logon-server conflict occurred." ) );
         return;

      case NERR_LogonNoUserPath:
   
         error_message.assign(WSTRING_VIEW(L"The server is configured without a valid user path." ) );
         return;

      case NERR_LogonScriptError:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred while loading or running the logon script." ) );
         return;

      case NERR_StandaloneLogon:
   
         error_message.assign(WSTRING_VIEW(L"The logon server was not specified. Your computer will be logged on as STANDALONE." ) );
         return;

      case NERR_LogonServerNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The logon server could not be found." ) );
         return;

      case NERR_LogonDomainExists:
   
         error_message.assign(WSTRING_VIEW(L"There is already a logon domain for this computer." ) );
         return;

      case NERR_NonValidatedLogon:
   
         error_message.assign(WSTRING_VIEW(L"The logon server could not validate the logon." ) );
         return;

      case NERR_ACFNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The security database could not be found." ) );
         return;

      case NERR_GroupNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The group name could not be found." ) );
         return;

      case NERR_UserNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The user name could not be found." ) );
         return;

      case NERR_ResourceNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The resource name could not be found." ) );
         return;

      case NERR_GroupExists:
   
         error_message.assign(WSTRING_VIEW(L"The group already exists." ) );
         return;

      case NERR_UserExists:
   
         error_message.assign(WSTRING_VIEW(L"The user account already exists." ) );
         return;

      case NERR_ResourceExists:
   
         error_message.assign(WSTRING_VIEW(L"The resource permission list already exists." ) );
         return;

      case NERR_NotPrimary:
   
         error_message.assign(WSTRING_VIEW(L"This operation is only allowed on the primary domain controller of the domain." ) );
         return;

      case NERR_ACFNotLoaded:
   
         error_message.assign(WSTRING_VIEW(L"The security database has not been started." ) );
         return;

      case NERR_ACFNoRoom:
   
         error_message.assign(WSTRING_VIEW(L"There are too many names in the user accounts database." ) );
         return;

      case NERR_ACFFileIOFail:
   
         error_message.assign(WSTRING_VIEW(L"A disk I/O failure occurred." ) );
         return;

      case NERR_ACFTooManyLists:
   
         error_message.assign(WSTRING_VIEW(L"The limit of 64 entries per resource was exceeded." ) );
         return;

      case NERR_UserLogon:
   
         error_message.assign(WSTRING_VIEW(L"Deleting a user with a session is not allowed." ) );
         return;

      case NERR_ACFNoParent:
   
         error_message.assign(WSTRING_VIEW(L"The parent directory could not be located." ) );
         return;

      case NERR_CanNotGrowSegment:
   
         error_message.assign(WSTRING_VIEW(L"Unable to add to the security database session cache segment." ) );
         return;

      case NERR_SpeGroupOp:
   
         error_message.assign(WSTRING_VIEW(L"This operation is not allowed on this special group." ) );
         return;

      case NERR_NotInCache:
   
         error_message.assign(WSTRING_VIEW(L"This user is not cached in user accounts database session cache." ) );
         return;

      case NERR_UserInGroup:
   
         error_message.assign(WSTRING_VIEW(L"The user already belongs to this group." ) );
         return;

      case NERR_UserNotInGroup:
   
         error_message.assign(WSTRING_VIEW(L"The user does not belong to this group." ) );
         return;

      case NERR_AccountUndefined:
   
         error_message.assign(WSTRING_VIEW(L"This user account is undefined." ) );
         return;

      case NERR_AccountExpired:
   
         error_message.assign(WSTRING_VIEW(L"This user account has expired." ) );
         return;

      case NERR_InvalidWorkstation:
   
         error_message.assign(WSTRING_VIEW(L"The user is not allowed to log on from this workstation." ) );
         return;

      case NERR_InvalidLogonHours:
   
         error_message.assign(WSTRING_VIEW(L"The user is not allowed to log on at this time." ) );
         return;

      case NERR_PasswordExpired:
   
         error_message.assign(WSTRING_VIEW(L"The password of this user has expired." ) );
         return;

      case NERR_PasswordCantChange:
   
         error_message.assign(WSTRING_VIEW(L"The password of this user cannot change." ) );
         return;

      case NERR_PasswordHistConflict:
   
         error_message.assign(WSTRING_VIEW(L"This password cannot be used now." ) );
         return;

      case NERR_PasswordTooShort:
   
         error_message.assign(WSTRING_VIEW(L"The password is shorter than required." ) );
         return;

      case NERR_PasswordTooRecent:
   
         error_message.assign(WSTRING_VIEW(L"The password of this user is too recent to change." ) );
         return;

      case NERR_InvalidDatabase:
   
         error_message.assign(WSTRING_VIEW(L"The security database is corrupted." ) );
         return;

      case NERR_DatabaseUpToDate:
   
         error_message.assign(WSTRING_VIEW(L"No updates are necessary to this replicant network/local security database." ) );
         return;

      case NERR_SyncRequired:
   
         error_message.assign(WSTRING_VIEW(L"This replicant database is outdated; synchronization is required." ) );
         return;

      case NERR_UseNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The network connection could not be found." ) );
         return;

      case NERR_BadAsgType:
   
         error_message.assign(WSTRING_VIEW(L"This asg_type is invalid." ) );
         return;

      case NERR_DeviceIsShared:
   
         error_message.assign(WSTRING_VIEW(L"This device is currently being shared." ) );
         return;

      case NERR_NoComputerName:
   
         error_message.assign(WSTRING_VIEW(L"The computer name could not be added as a message alias. The name may already exist on the network." ) );
         return;

      case NERR_MsgAlreadyStarted:
   
         error_message.assign(WSTRING_VIEW(L"The Messenger service is already started." ) );
         return;

      case NERR_MsgInitFailed:
   
         error_message.assign(WSTRING_VIEW(L"The Messenger service failed to start." ) );
         return;

      case NERR_NameNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The message alias could not be found on the network." ) );
         return;

      case NERR_AlreadyForwarded:
   
         error_message.assign(WSTRING_VIEW(L"This message alias has already been forwarded." ) );
         return;

      case NERR_AddForwarded:
   
         error_message.assign(WSTRING_VIEW(L"This message alias has been added but is still forwarded." ) );
         return;

      case NERR_AlreadyExists:
   
         error_message.assign(WSTRING_VIEW(L"This message alias already exists locally." ) );
         return;

      case NERR_TooManyNames:
   
         error_message.assign(WSTRING_VIEW(L"The maximum number of added message aliases has been exceeded." ) );
         return;

      case NERR_DelComputerName:
   
         error_message.assign(WSTRING_VIEW(L"The computer name could not be deleted." ) );
         return;

      case NERR_LocalForward:
   
         error_message.assign(WSTRING_VIEW(L"Messages cannot be forwarded back to the same workstation." ) );
         return;

      case NERR_GrpMsgProcessor:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred in the domain message processor." ) );
         return;

      case NERR_PausedRemote:
   
         error_message.assign(WSTRING_VIEW(L"The message was sent, but the recipient has paused the Messenger service." ) );
         return;

      case NERR_BadReceive:
   
         error_message.assign(WSTRING_VIEW(L"The message was sent but not received." ) );
         return;

      case NERR_NameInUse:
   
         error_message.assign(WSTRING_VIEW(L"The message alias is currently in use. Try again later." ) );
         return;

      case NERR_MsgNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The Messenger service has not been started." ) );
         return;

      case NERR_NotLocalName:
   
         error_message.assign(WSTRING_VIEW(L"The name is not on the local computer." ) );
         return;

      case NERR_NoForwardName:
   
         error_message.assign(WSTRING_VIEW(L"The forwarded message alias could not be found on the network." ) );
         return;

      case NERR_RemoteFull:
   
         error_message.assign(WSTRING_VIEW(L"The message alias table on the remote station is full." ) );
         return;

      case NERR_NameNotForwarded:
   
         error_message.assign(WSTRING_VIEW(L"Messages for this alias are not currently being forwarded." ) );
         return;

      case NERR_TruncatedBroadcast:
   
         error_message.assign(WSTRING_VIEW(L"The broadcast message was truncated." ) );
         return;

      case NERR_InvalidDevice:
   
         error_message.assign(WSTRING_VIEW(L"This is an invalid device name." ) );
         return;

      case NERR_WriteFault:
   
         error_message.assign(WSTRING_VIEW(L"A write fault occurred." ) );
         return;

      case NERR_DuplicateName:
   
         error_message.assign(WSTRING_VIEW(L"A duplicate message alias exists on the network." ) );
         return;

      case NERR_DeleteLater:
   
         error_message.assign(WSTRING_VIEW(L"@W This message alias will be deleted later." ) );
         return;

      case NERR_IncompleteDel:
   
         error_message.assign(WSTRING_VIEW(L"The message alias was not successfully deleted from all networks." ) );
         return;

      case NERR_MultipleNets:
   
         error_message.assign(WSTRING_VIEW(L"This operation is not supported on computers with multiple networks." ) );
         return;

      case NERR_NetNameNotFound:
   
         error_message.assign(WSTRING_VIEW(L"This shared resource does not exist." ) );
         return;

      case NERR_DeviceNotShared:
   
         error_message.assign(WSTRING_VIEW(L"This device is not shared." ) );
         return;

      case NERR_ClientNameNotFound:
   
         error_message.assign(WSTRING_VIEW(L"A session does not exist with that computer name." ) );
         return;

      case NERR_FileIdNotFound:
   
         error_message.assign(WSTRING_VIEW(L"There is not an open file with that identification number." ) );
         return;

      case NERR_ExecFailure:
   
         error_message.assign(WSTRING_VIEW(L"A failure occurred when executing a remote administration command." ) );
         return;

      case NERR_TmpFile:
   
         error_message.assign(WSTRING_VIEW(L"A failure occurred when opening a remote temporary file." ) );
         return;

      case NERR_TooMuchData:
   
         error_message.assign(WSTRING_VIEW(L"The data    returned from a remote administration command has been truncated to 64K." ) );
         return;

      case NERR_DeviceShareConflict:
   
         error_message.assign(WSTRING_VIEW(L"This device cannot be shared as both a spooled and a non-spooled resource." ) );
         return;

      case NERR_BrowserTableIncomplete:
   
         error_message.assign(WSTRING_VIEW(L"The information in the list of servers may be incorrect." ) );
         return;

      case NERR_NotLocalDomain:
   
         error_message.assign(WSTRING_VIEW(L"The computer is not active in this domain." ) );
         return;

      case NERR_DevInvalidOpCode:
   
         error_message.assign(WSTRING_VIEW(L"The operation is invalid for this device." ) );
         return;

      case NERR_DevNotFound:
   
         error_message.assign(WSTRING_VIEW(L"This device cannot be shared." ) );
         return;

      case NERR_DevNotOpen:
   
         error_message.assign(WSTRING_VIEW(L"This device was not open." ) );
         return;

      case NERR_BadQueueDevString:
   
         error_message.assign(WSTRING_VIEW(L"This device name list is invalid." ) );
         return;

      case NERR_BadQueuePriority:
   
         error_message.assign(WSTRING_VIEW(L"The queue priority is invalid." ) );
         return;

      case NERR_NoCommDevs:
   
         error_message.assign(WSTRING_VIEW(L"There are no shared communication devices." ) );
         return;

      case NERR_QueueNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The queue you specified does not exist." ) );
         return;

      case NERR_BadDevString:
   
         error_message.assign(WSTRING_VIEW(L"This list of devices is invalid." ) );
         return;

      case NERR_BadDev:
   
         error_message.assign(WSTRING_VIEW(L"The requested device is invalid." ) );
         return;

      case NERR_InUseBySpooler:
   
         error_message.assign(WSTRING_VIEW(L"This device is already in use by the spooler." ) );
         return;

      case NERR_CommDevInUse:
   
         error_message.assign(WSTRING_VIEW(L"This device is already in use as a communication device." ) );
         return;

      case NERR_InvalidComputer:
   
         error_message.assign(WSTRING_VIEW(L"This computer name is invalid." ) );
         return;

      case NERR_MaxLenExceeded:
   
         error_message.assign(WSTRING_VIEW(L"The string and prefix specified are too long." ) );
         return;

      case NERR_BadComponent:
   
         error_message.assign(WSTRING_VIEW(L"This path component is invalid." ) );
         return;

      case NERR_CantType:
   
         error_message.assign(WSTRING_VIEW(L"Could not determine the type of input." ) );
         return;

      case NERR_TooManyEntries:
   
         error_message.assign(WSTRING_VIEW(L"The buffer for types is not big enough." ) );
         return;

      case NERR_ProfileFileTooBig:
   
         error_message.assign(WSTRING_VIEW(L"Profile files cannot exceed 64K." ) );
         return;

      case NERR_ProfileOffset:
   
         error_message.assign(WSTRING_VIEW(L"The start offset is out of range." ) );
         return;

      case NERR_ProfileCleanup:
   
         error_message.assign(WSTRING_VIEW(L"The system cannot delete current connections to network resources." ) );
         return;

      case NERR_ProfileUnknownCmd:
   
         error_message.assign(WSTRING_VIEW(L"The system was unable to parse the command line in this file." ) );
         return;

      case NERR_ProfileLoadErr:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred while loading the profile file." ) );
         return;

      case NERR_ProfileSaveErr:
   
         error_message.assign(WSTRING_VIEW(L"@W Errors occurred while saving the profile file. The profile was partially saved." ) );
         return;

      case NERR_LogOverflow:
   
         error_message.assign(WSTRING_VIEW(L"Log file %1 is full." ) );
         return;

      case NERR_LogFileChanged:
   
         error_message.assign(WSTRING_VIEW(L"This log file has changed between reads." ) );
         return;

      case NERR_LogFileCorrupt:
   
         error_message.assign(WSTRING_VIEW(L"Log file %1 is corrupt." ) );
         return;

      case NERR_SourceIsDir:
   
         error_message.assign(WSTRING_VIEW(L"The source path cannot be a directory." ) );
         return;

      case NERR_BadSource:
   
         error_message.assign(WSTRING_VIEW(L"The source path is illegal." ) );
         return;

      case NERR_BadDest:
   
         error_message.assign(WSTRING_VIEW(L"The destination path is illegal." ) );
         return;

      case NERR_DifferentServers:
   
         error_message.assign(WSTRING_VIEW(L"The source and destination paths are on different servers." ) );
         return;

      case NERR_RunSrvPaused:
   
         error_message.assign(WSTRING_VIEW(L"The Run server you requested is paused." ) );
         return;

      case NERR_ErrCommRunSrv:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred when communicating with a Run server." ) );
         return;

      case NERR_ErrorExecingGhost:
   
         error_message.assign(WSTRING_VIEW(L"An error occurred when starting a background process." ) );
         return;

      case NERR_ShareNotFound:
   
         error_message.assign(WSTRING_VIEW(L"The shared resource you are connected to could not be found." ) );
         return;

      case NERR_InvalidLana:
   
         error_message.assign(WSTRING_VIEW(L"The LAN adapter number is invalid." ) );
         return;

      case NERR_OpenFiles:
   
         error_message.assign(WSTRING_VIEW(L"There are open files on the connection." ) );
         return;

      case NERR_ActiveConns:
   
         error_message.assign(WSTRING_VIEW(L"Active connections still exist." ) );
         return;

      case NERR_BadPasswordCore:
   
         error_message.assign(WSTRING_VIEW(L"This share name or password is invalid." ) );
         return;

      case NERR_DevInUse:
   
         error_message.assign(WSTRING_VIEW(L"The device is being accessed by an active process." ) );
         return;

      case NERR_LocalDrive:
   
         error_message.assign(WSTRING_VIEW(L"The drive letter is in use locally." ) );
         return;

      case NERR_AlertExists:
   
         error_message.assign(WSTRING_VIEW(L"The specified client is already registered for the specified event." ) );
         return;

      case NERR_TooManyAlerts:
   
         error_message.assign(WSTRING_VIEW(L"The alert table is full." ) );
         return;

      case NERR_NoSuchAlert:
   
         error_message.assign(WSTRING_VIEW(L"An invalid or nonexistent alert name was raised." ) );
         return;

      case NERR_BadRecipient:
   
         error_message.assign(WSTRING_VIEW(L"The alert recipient is invalid." ) );
         return;

      case NERR_AcctLimitExceeded:
   
         error_message.assign(WSTRING_VIEW(L"A user's session with this server has been deleted because the user's logon hours are no longer valid." ) );
         return;

      case NERR_InvalidLogSeek:
   
         error_message.assign(WSTRING_VIEW(L"The log file does not contain the requested record number." ) );
         return;

      case NERR_BadUasConfig:
   
         error_message.assign(WSTRING_VIEW(L"The user accounts database is not configured correctly." ) );
         return;

      case NERR_InvalidUASOp:
   
         error_message.assign(WSTRING_VIEW(L"This operation is not permitted when the Netlogon service is running." ) );
         return;

      case NERR_LastAdmin:
   
         error_message.assign(WSTRING_VIEW(L"This operation is not allowed on the last administrative account." ) );
         return;

      case NERR_DCNotFound:
   
         error_message.assign(WSTRING_VIEW(L"Could not find domain controller for this domain." ) );
         return;

      case NERR_LogonTrackingError:
   
         error_message.assign(WSTRING_VIEW(L"Could not set logon information for this user." ) );
         return;

      case NERR_NetlogonNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The Netlogon service has not been started." ) );
         return;

      case NERR_CanNotGrowUASFile:
   
         error_message.assign(WSTRING_VIEW(L"Unable to add to the user accounts database." ) );
         return;

      case NERR_TimeDiffAtDC:
   
         error_message.assign(WSTRING_VIEW(L"This server's clock is not synchronized with the primary domain controller's clock." ) );
         return;

      case NERR_PasswordMismatch:
   
         error_message.assign(WSTRING_VIEW(L"A password mismatch has been detected." ) );
         return;

      case NERR_NoSuchServer:
   
         error_message.assign(WSTRING_VIEW(L"The server identification does not specify a valid server." ) );
         return;

      case NERR_NoSuchSession:
   
         error_message.assign(WSTRING_VIEW(L"The session identification does not specify a valid session." ) );
         return;

      case NERR_NoSuchConnection:
   
         error_message.assign(WSTRING_VIEW(L"The connection identification does not specify a valid connection." ) );
         return;

      case NERR_TooManyServers:
   
         error_message.assign(WSTRING_VIEW(L"There is no space for another entry in the table of available servers." ) );
         return;

      case NERR_TooManySessions:
   
         error_message.assign(WSTRING_VIEW(L"The server has reached the maximum number of sessions it supports." ) );
         return;

      case NERR_TooManyConnections:
   
         error_message.assign(WSTRING_VIEW(L"The server has reached the maximum number of connections it supports." ) );
         return;

      case NERR_TooManyFiles:
   
         error_message.assign(WSTRING_VIEW(L"The server cannot open more files because it has reached its maximum number." ) );
         return;

      case NERR_NoAlternateServers:
   
         error_message.assign(WSTRING_VIEW(L"There are no alternate servers registered on this server." ) );
         return;

      case NERR_TryDownLevel:
   
         error_message.assign(WSTRING_VIEW(L"Try down-level (remote admin protocol) version of API instead." ) );
         return;

      case NERR_UPSDriverNotStarted:
   
         error_message.assign(WSTRING_VIEW(L"The UPS driver could not be accessed by the UPS service." ) );
         return;

      case NERR_UPSInvalidConfig:
   
         error_message.assign(WSTRING_VIEW(L"The UPS service is not configured correctly." ) );
         return;

      case NERR_UPSInvalidCommPort:

         error_message.assign(WSTRING_VIEW(L"The UPS service could not access the specified Comm Port." ) );
         return;

      case NERR_UPSSignalAsserted:

         error_message.assign(WSTRING_VIEW(L"The UPS indicated a line fail or low battery situation. Service not started." ) );
         return;

      case NERR_UPSShutdownFailed:

         error_message.assign(WSTRING_VIEW(L"The UPS service failed to perform a system shut down." ) );
         return;

      case NERR_BadDosRetCode:

         error_message.assign(WSTRING_VIEW(L"The program below returned an MS-DOS error code:" ) );
         return;

      case NERR_ProgNeedsExtraMem:

         error_message.assign(WSTRING_VIEW(L"The program below needs more memory:" ) );
         return;

      case NERR_BadDosFunction:

         error_message.assign(WSTRING_VIEW(L"The program below called an unsupported MS-DOS function:" ) );
         return;

      case NERR_RemoteBootFailed:
   
         error_message.assign(WSTRING_VIEW(L"The workstation failed to boot." ) );
         return;

      case NERR_BadFileCheckSum:
   
         error_message.assign(WSTRING_VIEW(L"The file below is corrupt." ) );
         return;

      case NERR_NoRplBootSystem:
   
         error_message.assign(WSTRING_VIEW(L"No loader is specified in the boot-block definition file." ) );
         return;

      case NERR_RplLoadrNetBiosErr:
   
         error_message.assign(WSTRING_VIEW(L"NetBIOS    returned an error: The NCB and SMB are dumped above." ) );
         return;

      case NERR_RplLoadrDiskErr:
   
         error_message.assign(WSTRING_VIEW(L"A disk I/O error occurred." ) );
         return;

      case NERR_ImageParamErr:
   
         error_message.assign(WSTRING_VIEW(L"Image parameter substitution failed." ) );
         return;

      case NERR_TooManyImageParams:
   
         error_message.assign(WSTRING_VIEW(L"Too many image parameters cross disk sector boundaries." ) );
         return;

      case NERR_NonDosFloppyUsed:
   
         error_message.assign(WSTRING_VIEW(L"The image was not generated from an MS-DOS diskette formatted with /S." ) );
         return;

      case NERR_RplBootRestart:
   
         error_message.assign(WSTRING_VIEW(L"Remote boot will be restarted later." ) );
         return;

      case NERR_RplSrvrCallFailed:
   
         error_message.assign(WSTRING_VIEW(L"The call to the Remoteboot server failed." ) );
         return;

      case NERR_CantConnectRplSrvr:
   
         error_message.assign(WSTRING_VIEW(L"Cannot connect to the Remoteboot server." ) );
         return;

      case NERR_CantOpenImageFile:
   
         error_message.assign(WSTRING_VIEW(L"Cannot open image file on the Remoteboot server." ) );
         return;

      case NERR_CallingRplSrvr:
   
         error_message.assign(WSTRING_VIEW(L"Connecting to the Remoteboot server..." ) );
         return;

      case NERR_StartingRplBoot:
   
         error_message.assign(WSTRING_VIEW(L"Connecting to the Remoteboot server..." ) );
         return;

      case NERR_RplBootServiceTerm:
   
         error_message.assign(WSTRING_VIEW(L"Remote boot service was stopped; check the error log for the cause of the problem." ) );
         return;

      case NERR_RplBootStartFailed:
   
         error_message.assign(WSTRING_VIEW(L"Remote boot startup failed; check the error log for the cause of the problem." ) );
         return;

      case NERR_RPL_CONNECTED:

         error_message.assign(WSTRING_VIEW(L"A second connection to a Remoteboot resource is not allowed." ) );
         return;

      case NERR_BrowserConfiguredToNotRun:

         error_message.assign(WSTRING_VIEW(L"The browser service was configured with MaintainServerList=No." ) );
         return;

      case NERR_RplNoAdaptersStarted:

         error_message.assign(WSTRING_VIEW(L"Service failed to start since none of the network adapters started with this service." ) );
         return;

      case NERR_RplBadRegistry:

         error_message.assign(WSTRING_VIEW(L"Service failed to start due to bad startup information in the registry." ) );
         return;

      case NERR_RplBadDatabase:

         error_message.assign(WSTRING_VIEW(L"Service failed to start because its database is absent or corrupt." ) );
         return;

      case NERR_RplRplfilesShare:

         error_message.assign(WSTRING_VIEW(L"Service failed to start because RPLFILES share is absent." ) );
         return;

      case NERR_RplNotRplServer:

         error_message.assign(WSTRING_VIEW(L"Service failed to start because RPLUSER group is absent." ) );
         return;

      case NERR_RplCannotEnum:

         error_message.assign(WSTRING_VIEW(L"Cannot enumerate service records." ) );
         return;

      case NERR_RplWkstaInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Workstation record information has been corrupted." ) );
         return;

      case NERR_RplWkstaNotFound:

         error_message.assign(WSTRING_VIEW(L"Workstation record was not found." ) );
         return;

      case NERR_RplWkstaNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"Workstation name is in use by some other workstation." ) );
         return;

      case NERR_RplProfileInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Profile record information has been corrupted." ) );
         return;

      case NERR_RplProfileNotFound:

         error_message.assign(WSTRING_VIEW(L"Profile record was not found." ) );
         return;

      case NERR_RplProfileNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"Profile name is in use by some other profile." ) );
         return;

      case NERR_RplProfileNotEmpty:

         error_message.assign(WSTRING_VIEW(L"There are workstations using this profile." ) );
         return;

      case NERR_RplConfigInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Configuration record information has been corrupted." ) );
         return;

      case NERR_RplConfigNotFound:

         error_message.assign(WSTRING_VIEW(L"Configuration record was not found." ) );
         return;

      case NERR_RplAdapterInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Adapter id record information has been corrupted." ) );
         return;

      case NERR_RplInternal:

         error_message.assign(WSTRING_VIEW(L"An internal service error has occured." ) );
         return;

      case NERR_RplVendorInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Vendor id record information has been corrupted." ) );
         return;

      case NERR_RplBootInfoCorrupted:

         error_message.assign(WSTRING_VIEW(L"Boot block record information has been corrupted." ) );
         return;

      case NERR_RplWkstaNeedsUserAcct:

         error_message.assign(WSTRING_VIEW(L"The user account for this workstation record is missing." ) );
         return;

      case NERR_RplNeedsRPLUSERAcct:

         error_message.assign(WSTRING_VIEW(L"The RPLUSER local group could not be found." ) );
         return;

      case NERR_RplBootNotFound:

         error_message.assign(WSTRING_VIEW(L"Boot block record was not found." ) );
         return;

      case NERR_RplIncompatibleProfile:

         error_message.assign(WSTRING_VIEW(L"Chosen profile is incompatible with this workstation." ) );
         return;

      case NERR_RplAdapterNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"Chosen network adapter id is in use by some other workstation." ) );
         return;

      case NERR_RplConfigNotEmpty:

         error_message.assign(WSTRING_VIEW(L"There are profiles using this configuration." ) );
         return;

      case NERR_RplBootInUse:

         error_message.assign(WSTRING_VIEW(L"There are workstations, profiles or configurations using this boot block." ) );
         return;

      case NERR_RplBackupDatabase:

         error_message.assign(WSTRING_VIEW(L"Service failed to backup remoteboot database." ) );
         return;

      case NERR_RplAdapterNotFound:

         error_message.assign(WSTRING_VIEW(L"Adapter record was not found." ) );
         return;

      case NERR_RplVendorNotFound:

         error_message.assign(WSTRING_VIEW(L"Vendor record was not found." ) );
         return;

      case NERR_RplVendorNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"Vendor name is in use by some other vendor record." ) );
         return;

      case NERR_RplBootNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"(boot name, vendor id) is in use by some other boot block record." ) );
         return;

      case NERR_RplConfigNameUnavailable:

         error_message.assign(WSTRING_VIEW(L"Configuration name is in use by some other configuration." ) );
         return;

      case NERR_DfsInternalCorruption:

         error_message.assign(WSTRING_VIEW(L"The internal database maintained by the Dfs service is corrupt." ) );
         return;

      case NERR_DfsVolumeDataCorrupt:

         error_message.assign(WSTRING_VIEW(L"One of the records in the internal Dfs database is corrupt." ) );
         return;

      case NERR_DfsNoSuchVolume:

         error_message.assign(WSTRING_VIEW(L"There is no volume whose entry path matches the input Entry Path." ) );
         return;

      case NERR_DfsVolumeAlreadyExists:

         error_message.assign(WSTRING_VIEW(L"A volume with the given name already exists." ) );
         return;

      case NERR_DfsAlreadyShared:

         error_message.assign(WSTRING_VIEW(L"The server share specified is already shared in the Dfs." ) );
         return;

      case NERR_DfsNoSuchShare:

         error_message.assign(WSTRING_VIEW(L"The indicated server share does not support the indicated Dfs volume." ) );
         return;

      case NERR_DfsNotALeafVolume:

         error_message.assign(WSTRING_VIEW(L"The operation is not valid on a non-leaf volume." ) );
         return;

      case NERR_DfsLeafVolume:

         error_message.assign(WSTRING_VIEW(L"The operation is not valid on a leaf volume." ) );
         return;

      case NERR_DfsVolumeHasMultipleServers:

         error_message.assign(WSTRING_VIEW(L"The operation is ambiguous because the volume has multiple servers." ) );
         return;

      case NERR_DfsCantCreateJunctionPoint:

         error_message.assign(WSTRING_VIEW(L"Unable to create a junction point." ) );
         return;

      case NERR_DfsServerNotDfsAware:

         error_message.assign(WSTRING_VIEW(L"The server is not Dfs Aware." ) );
         return;

      case NERR_DfsBadRenamePath:

         error_message.assign(WSTRING_VIEW(L"The specified rename target path is invalid." ) );
         return;

      case NERR_DfsVolumeIsOffline:

         error_message.assign(WSTRING_VIEW(L"The specified Dfs volume is offline." ) );
         return;

      case (NERR_BASE+573):

         // NERR_DfsNoSuchServer

         error_message.assign(WSTRING_VIEW(L"The specified server is not a server for this volume." ) );
         return;

      case (NERR_BASE+574):

         // NERR_DfsCyclicalName

         error_message.assign(WSTRING_VIEW(L"A cycle in the Dfs name was detected." ) );
         return;

      case (NERR_BASE+575):

         // NERR_DfsNotSupportedInServerDfs

         error_message.assign(WSTRING_VIEW(L"The operation is not supported on a server-based Dfs." ) );
         return;

      case (NERR_BASE+576):

         // NERR_DfsDuplicateService

         error_message.assign(WSTRING_VIEW(L"This volume is already supported by the specified server-share." ) );
         return;

      case (NERR_BASE+577):

         // NERR_DfsCantRemoveLastServerShare

         error_message.assign(WSTRING_VIEW(L"Can't remove the last server-share supporting this volume." ) );
         return;

      case (NERR_BASE+578):

         // NERR_DfsVolumeIsInterDfs

         error_message.assign(WSTRING_VIEW(L"The operation is not supported for an Inter-Dfs volume." ) );
         return;

      case (NERR_BASE+579):

         // NERR_DfsInconsistent

         error_message.assign(WSTRING_VIEW(L"The internal state of the Dfs Service has become inconsistent." ) );
         return;

      case (NERR_BASE+580):

         // NERR_DfsServerUpgraded

         error_message.assign(WSTRING_VIEW(L"The Dfs Service has been installed on the specified server." ) );
         return;

      case (NERR_BASE+581):

         // NERR_DfsDataIsIdentical

         error_message.assign(WSTRING_VIEW(L"The Dfs data being reconciled is identical." ) );
         return;

      case (NERR_BASE+582):

         // NERR_DfsCantRemoveDfsRoot

         error_message.assign(WSTRING_VIEW(L"The Dfs root volume cannot be deleted - Uninstall Dfs if required." ) );
         return;

      case (NERR_BASE+583):

         // NERR_DfsChildOrParentInDfs

         error_message.assign(WSTRING_VIEW(L"A child or parent directory of the share is already in a Dfs." ) );
         return;

      case (NERR_BASE+590):

         // NERR_DfsInternalError

         error_message.assign(WSTRING_VIEW(L"Dfs internal error." ) );
         return;

      case (NERR_BASE+591):

         // NERR_SetupAlreadyJoined

         error_message.assign(WSTRING_VIEW(L"This machine is already joined to a domain." ) );
         return;

      case (NERR_BASE+592):

         // NERR_SetupNotJoined

         error_message.assign(WSTRING_VIEW(L"This machine is not currently joined to a domain." ) );
         return;

      case (NERR_BASE+593):

         // NERR_SetupDomainController

         error_message.assign(WSTRING_VIEW(L"This machine is a domain controller and cannot be unjoined from a domain." ) );
         return;

      case (NERR_BASE+594):

         // NERR_DefaultJoinRequired

         error_message.assign(WSTRING_VIEW(L"The destination domain controller does not support creating machine accounts in OUs." ) );
         return;

      case (NERR_BASE+595):

         // NERR_InvalidWorkgroupName

         error_message.assign(WSTRING_VIEW(L"The specified workgroup name is invalid." ) );
         return;

      case (NERR_BASE+596):

         // NERR_NameUsesIncompatibleCodePage

         error_message.assign(WSTRING_VIEW(L"The specified computer name is incompatible with the default language used on the domain controller." ) );
         return;

      case (NERR_BASE+597):

         // NERR_ComputerAccountNotFound

         error_message.assign(WSTRING_VIEW(L"The specified computer account could not be found." ) );
         return;

      default:

         if ( error_code < NERR_BASE )
         {
            /*
            ** This ain't a LanMan error code, let's see if it'll Format...
            */

            LPVOID message_buffer = (LPVOID) nullptr;

            FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER bitor FORMAT_MESSAGE_FROM_SYSTEM,
                           nullptr,
                           error_code,
                           MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
                 (LPTSTR) &message_buffer,
                           0,
                           nullptr );

            if ( message_buffer != nullptr )
            {
               TCHAR temp_string[ 255 ];

               _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Not an NERR but it means %s" ), (LPCTSTR) message_buffer );

               ::LocalFree( message_buffer );

               error_message.assign( temp_string );
            }
            else
            {
               TCHAR temp_string[ 256 ];

               _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Not an NERR, code is %d" ), (int) error_code );
               error_message.assign( temp_string );
            }
         }
         else
         {
            wchar_t temp_string[ 256 ];

            _stprintf_s( temp_string, std::size( temp_string ), TEXT( "Unknown error %d (NERR_BASE + %d)" ), (int) error_code, (int)( (int) error_code - (int) NERR_BASE ) );
            error_message.assign( temp_string );
         }

         return;
   }
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - Convert_NERR_Code_to_String</TITLE>
</HEAD>

<BODY>

<H1>Convert_NERR_Code_to_String</H1>
$Revision: 22 $
<HR>

<H2>Description</H2>

This function converts NERR_* error codes to a human readable string. I had to write this
because calling FormatMessage() with the FORMAT_MESSAGE_FROM_SYSTEM doesn't work. If the
error code sent to this function is not a network error, it will still try to convert
it to something a human would understand.

<H2>Example</H2>

<PRE><CODE>&#35;include &lt;wfc.h&gt;
&#35;pragma hdrstop

int _tmain( int number_of_command_line_arguments, LPCTSTR command_line_arguments[] )
{
   <A HREF="WfcTrace.htm">WFCTRACEINIT</A>( TEXT( &quot;_tmain()&quot; ) );

   BYTE buffer[ 4096 ];

   DWORD return_code = 0;

   return_code = NetWkstaGetInfo( nullptr, 302, buffer );

   if ( return_code != NERR_Success )
   {
      std::wstring error_message;

      <B>Convert_NERR_Code_to_String</B>( return_code, error_message );

      _tprintf( TEXT( &quot;Network error, %s\n&quot; ), (LPCTSTR) error_message );
   }
}</CODE></PRE>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: lmerr.cpp $<BR>
$Modtime: 6/26/01 10:53a $
</BODY>

</HTML>
*/
