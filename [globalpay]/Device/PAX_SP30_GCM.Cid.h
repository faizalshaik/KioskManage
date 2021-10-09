/*
>>> Master include file for applications that
    call functions provided by PIN_CS04_598M.dll

Founder : coden aka Narankhuu N
mailto  : codesaur@gmail.com

Copyright © 2017 Metatech LLC.
All rights reserved.

>>> 2017.03.27 - Initial version
*/

#pragma once

#include <windows.h>

namespace codesaur {
/* Flags */
// Specifies the type of access for which the can be used as a combination of the following flags
#define	WFS_PIN_USEKEYENCKEY 0x0020 // Key is used as key encryption key.
#define	WFS_PIN_USEFUNCTION  0x0002 // Key can be used for PIN function.
#define	WFS_PIN_USEMACING    0x0004 // Key can be used for MACing.
#define	WFS_PIN_USECRYPT     0x0001 // Key can be used for encryption / decryption

// Specifies the format of the PIN block, it can be only have one of the follows values
#define	WFS_PIN_FORM3624      0x0001
#define	WFS_PIN_FORMANSI      0x0002
#define	WFS_PIN_FORMISO0      0x0004
#define	WFS_PIN_FORMISO1      0x0008
#define	WFS_PIN_FORMECI2      0x0010
#define	WFS_PIN_FORMECI3      0x0020
#define	WFS_PIN_FORMVISA      0x0040
#define	WFS_PIN_FORMDIEBOLD   0x0080
#define	WFS_PIN_FORMDIEBOLDCO 0x0100
#define	WFS_PIN_FORMVISA3     0x0200
#define	WFS_PIN_FORMBANKSYS   0x0400
#define	WFS_PIN_FORMEMV       0x0800
#define	WFS_PIN_FORMISO3      0x2000

// When encrypt or decrypt data, the value as below
#define	WFS_PIN_MODEENCRYPT 1
#define	WFS_PIN_MODEDECRYPT 2
#define	WFS_PIN_MODERANDOM  3

// Specifies the encryption algorithm. Descriptions are as follows
#define	WFS_PIN_CRYPTDESECB    0x0001
#define	WFS_PIN_CRYPTDESCBC    0x0002
#define	WFS_PIN_CRYPTDESCFB    0x0004 // not support
#define	WFS_PIN_CRYPTRSA       0x0008
#define	WFS_PIN_CRYPTECMA      0x0010 // not support
#define	WFS_PIN_CRYPTDESMAC    0x0020
#define	WFS_PIN_CRYPTTRIDESECB 0x0040
#define	WFS_PIN_CRYPTTRIDESCBC 0x0080
#define	WFS_PIN_CRYPTTRIDESCFB 0x0100 // not support
#define	WFS_PIN_CRYPTTRIDESMAC 0x0200
#define	WFS_PIN_CRYPTMAAMAC    0x0400 // not support

// Specifies the type of access for which the can be used as a combination of the following flags
#define	WFS_PIN_USEKEYENCKEY 0x0020 // Key is used as key encryption key.
#define	WFS_PIN_USEFUNCTION  0x0002 // Key can be used for PIN function.
#define	WFS_PIN_USEMACING    0x0004 // Key can be used for MACing.
#define	WFS_PIN_USECRYPT     0x0001 // Key can be used for encryption / decryption

/* Error codes */
#define	STATUS_SUCCESS                      0x00 // No error – status successful
#define	STATUS_INVALID_COMMAND_LENGTH       0x01 // Command length not comply with definition
#define	STATUS_INVALID_KEYID                0x02 // KeyID(s) is outside of valid range
#define	STATUS_INVALID_WRITE_MODE           0x03 // Invalid Write Mode Specified
#define	STATUS_KEYID_NOT_EXIST              0x04 // Key with specified ID doesn’t exist
#define	STATUS_KEYID_LOCKED                 0x05 // Key with specified ID is locked
#define	STATUS_INVALID_KEY_LENGTH           0x06 // The Key to be stored has invalid length(not 8, 16 or 24 bytes or less than 16 bytes with MK Attributes or greater than 8 bytes with IV Attributes)
#define	STATUS_INVALID_KEY_ATTRIBUTES       0x07 // Key ID has wrong attribute
#define	STATUS_INVALID_IV_ATTRIBUTES        0x08 // IV has wrong attribute
#define	STATUS_KEY_ATTRIBUTES_NOT_MATCHED   0x09 // Key Attribute not Match the exist Attribute
#define	STATUS_KCV_VERIFICATION_FAIL        0x0A // Key Check Value Verification Failure
#define	STATUS_INVALID_KEY_VALUE            0x0B // Other KeyId Has the Same Key Value
#define	STATUS_KEY_USEVIOLATION             0x10 // Key with specified ID Useviolation
#define	STATUS_KEY_NOTALLOWED               0x11 // Key with specified ID Not Allowed to do that
#define	STATUS_INVALID_CHECK_MODE           0x12 // Check Mode Invalid
#define	STATUS_INVALID_VERIFY_MODE          0x13 // Verify Mode Invalid
#define	STATUS_INVALID_MACING               0x14 // KCV verification failed
#define	STATUS_INVALID_AUTHENTICATION_MODE  0x15 // Macing Invalid Or Verify Failure
#define	STATUS_INVALID_MODE                 0x16 // Invalid Mode or Entry Mode specified
#define	STATUS_INVALID_DATA_SPECIFIED       0x17 // Invalid data - values out of range
#define	STATUS_INVALID_LENGTH_OR_SUM        0x18 // Invalid Length or Offset + Length out of range
#define	STATUS_INVALID_OFFSET_LENGTH        0x19 // Invalid Offset or Length - out of range
#define	STATUS_INVALID_PIN_LENGTH           0x20 // Invalid PIN Length
#define	STATUS_PIN_VERIFICATION_FAIL        0x21 // PIN verification failed
#define	STATUS_PIN_ENCRYPTION_SUSPENDED     0x22 // PIN Encryption Suspended, try Pin after one hour
#define	STATUS_PIN_NOPIN                    0x23 // No Pin
#define	STATUS_INVALID_PIN2_LENGTH          0x24 // The password in key buffer has wrong length
#define	STATUS_PIN2_VERIFICATION_FAIL       0x25 // The password in key buffer verification failed
#define	STATUS_INVALID_MODULUS_LENGTH       0x31 // Invalid modulus length specified
#define	STATUS_INVALID_EXPONENT_LENGTH      0x32 // Invalid exponent length specified
#define	STATUS_INVALID_PKCS_STRUCTURE       0x33 // Invalid structure received
#define	STATUS_INVALID_PKCS_PADDING         0x34 // Invalid PKCS Padding
#define	STATUS_INVALID_SIGNATURE_LENGTH     0x35 // Invalid Signature Length
#define	STATUS_SIGNATURE_VERIFICATION_FAIL  0x36 // Signature verification failed
#define	STATUS_KEY_NOT_LOADED               0x37 // Key with specified ID not Loaded
#define	STATUS_INVALID_CERTIFICATE_FORMAT   0x41 // INVALID CERTIFICATE FORMAT
#define	STATUS_INVALID_CERTIFICATE_FROMAT   0x41 // INVALID CERTIFICATE FROMAT
#define	STATUS_INVALID_CERTIFICATE_VERSION  0x42 // INVALID CERTIFICATE VERSION
#define	STATUS_INVALID_CERTIFICATE_VALIDITY 0x43 // INVALID CERTIFICATE ISSUER
#define	STATUS_INVALID_CERTIFICATE_ISSUER   0x44 // INVALID CERTIFICATE VALIDITY
#define	STATUS_INVALID_CERTIFICATE_SUBJECT  0x45 // INVALID CERTIFICATE SUBJECT
#define	STATUS_INVALID_CERTIFICATE_ALGOR    0x46 // INVALID CERTIFICATE ALGOR
#define	STATUS_CERTIFICATE_NOT_EXIST        0x47 // Certificate with specified ID Not Exist
#define	STATUS_INVALID_DATA_DISABLEED       0x50 // Invalid Date Disableed
#define	STATUS_INVALID_USERBLOCK_ADDRESS    0x51 // Invalid User Block Address specified
#define	STATUS_INVALID_SERIALNO_SPECIFIED   0x52 // Invalid Serial Number specified
#define	STATUS_ALGORITHMNOTSUPP             0x71 // ALGORITHM UNSUPP
#define	STATUS_COMMANDUPSUPP                0x72 // COMMAND UNSUPP
#define	STATUS_ERROR_INTER_ERROR            0x73 // HARDWARE ERROR OR INTER ERROR
#define	STATUS_COMMAND_LOCK                 0x74 // Command Lock : Not allow Execte
#define	STATUS_KEYCOMMAN_MAC                0x75 // KEY_COMMAND_MAC value verification error
#define	STATUS_AUTHENTICATION_FAILED        0x80 // Authentication failure
#define	STATUS_NOT_AUTHENTE                 0x81 // Key with specified ID doesn't authentication
#define	STATUS_EPP_NOT_INITIALIZED          0x82 // EPP Keypair and / or Serial# signature not loaded
#define	STATUS_EPP_ALREADY_INITIALIZED      0x83 // EPP Keypair and / or Serial# signature already loaded
#define	STATUS_CERTIFICATE_ALREADY          0x84 // EPP Certification already loaded
#define	STATUS_GETPIN_TIMEOUT               0x85 // Wait for a pressed key over time in PIN ENTRY mode or KEY ENTRY mode.
#define	API_PARAMETER_ERROR                 0x90 // Parameter error
#define	API_PINFORMAT_UNSUPP                0x91 // Invalid pin format
#define	API_ALGORITHM_UNSUPP                0x92 // Invalid crypt algorithm
#define	API_EXCUTE_FAIL                     0x93 // Other execute error
#define	ERR_COMM_FAIL                       -1   // Communication failure / Open com failed
#define	ERR_COMM_TIMEOUT                    -2   // Operation time out
#define	ERR_COMM_CANCELED                   -3   // Operation cancel
#define	ERR_COMM_PARAM                      -4   // Parameter Error
#define	ERR_COMM_NODEFINED                  -100 // Other communication error

typedef int (WINAPI *ZT_PIN_OPENDEVICE)(BYTE, LPCTSTR);
typedef int (WINAPI *ZT_PIN_CLOSEDEVICE)();
typedef int (WINAPI *ZT_PIN_RESET)();
typedef int (WINAPI *ZT_PIN_DELETEKEY)(WORD);
typedef int (WINAPI *ZT_PIN_SETENTRYMODE)(BYTE);
typedef int (WINAPI *ZT_PIN_READINPUTDATA)(LPBYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_IMPORTKEY)(WORD, WORD, WORD, LPBYTE, BYTE, WORD, BYTE, LPBYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_GETPRINTBLOCK)(WORD, WORD, WORD, WORD, LPSTR, LPSTR, BYTE, BYTE, LPBYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_CRYPT)(WORD, LPBYTE, BYTE, WORD, WORD, WORD, WORD, LPBYTE, LPBYTE, WORD, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_INITIALDEVICE)(BYTE);
typedef int (WINAPI *ZT_PIN_READKEYATTRIBUTE)(WORD, LPWORD, LPBYTE);
typedef int (WINAPI *ZT_PIN_READKCV)(WORD, LPBYTE);
typedef int (WINAPI *ZT_PIN_WRITEUSERDATA)(BYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_READUSERDATA)(BYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_WRITEKEYVALUE)(BYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_READEPPINFO)(LPBYTE, LPBYTE);
typedef int (WINAPI *ZT_PIN_BACKSPACE)();
typedef int (WINAPI *ZT_PIN_GETDEVICESTATUS)(LPBYTE);
typedef int (WINAPI *ZT_PIN_UPDATEPASSWORD)(WORD, BYTE);
typedef int (WINAPI *ZT_PIN_REMOVEINSTALL)(BYTE);
typedef int (WINAPI *ZT_PIN_VIRTUALKEYINPUT)(char*, int);
typedef int (WINAPI *ZT_PIN_IMPORTRSAKEY)(WORD, WORD, WORD, BYTE, LPBYTE, WORD, BYTE, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_EXPORTRSAKEY)(WORD, WORD, LPBYTE, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_IMPORTDESKEY)(WORD, WORD, WORD, BYTE, BYTE, BYTE, LPBYTE, WORD, BYTE, BYTE, LPBYTE, WORD, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_EXPORTDESKEY)(WORD, WORD, WORD, BYTE, WORD, BYTE, BYTE, LPBYTE, LPBYTE, LPBYTE, WORD, LPBYTE, WORD, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_RAWRSAENCRYPT)(WORD, LPBYTE, WORD, LPBYTE, WORD, LPBYTE, LPWORD, LPBYTE);
typedef int (WINAPI *ZT_PIN_RSAENCRYPT)(WORD, LPBYTE, WORD, LPBYTE, LPWORD);
typedef int (WINAPI *ZT_PIN_CREATEOFFSET)(WORD, WORD, WORD, LPSTR, BYTE, LPSTR, LPSTR);
typedef int (WINAPI *ZT_PIN_LOCALDES)(WORD, WORD, WORD, LPSTR, BYTE, LPSTR, LPSTR);
typedef int (WINAPI *ZT_PIN_STOREKEY)(WORD, WORD, BYTE, LPBYTE, LPBYTE);

class EPP_ZT598M
{
#define	ERR_NO_DLL -101 // No DLL error
public:
	EPP_ZT598M();
	~EPP_ZT598M();

	HMODULE GetHandle();
	BOOL    IsLoaded();
	BOOL    IsOpen();

	int OpenDevice(BYTE byType, LPCTSTR szDescription);
	int CloseDevice();
	int Reset();
	int DeleteKey(WORD wKeyId);
	int SetEntryMode(BYTE byMode);
	int ReadInputData(LPBYTE lpInputData, LPBYTE lpInputDataLen);
	int ImportKey(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPBYTE lpValue, BYTE byValeLen, WORD wUse, BYTE byMode, LPBYTE lpKCV, LPBYTE lpKCVL);
	int GetPinBlock(WORD wKeyId, WORD wEnKeyId, WORD wVI, WORD wFormat, LPSTR lpCustomerData, LPSTR lpXORData, BYTE byPadding, BYTE byPinLen, LPBYTE lpPinBlock, LPBYTE lpPinBlockLen);
	int Crypt(WORD wKeyId, LPBYTE lpEnKeyValue, BYTE byEnKeyValueLen, WORD wVI, WORD wMode, WORD wAlgorithm, WORD wStartValueKeyId, LPBYTE lpStartValue, LPBYTE lpCryptData, WORD wCryptDataLen, LPBYTE lpOutCryptData, LPWORD lpOutCryptDataLen);
	int InitialDevice(BYTE byMode);
	int ReadKeyAttribute(WORD wKeyId, LPWORD lpKeyAttributes, LPBYTE lpKeyCheckValue);
	int ReadKCV(WORD wKeyId, LPBYTE lpKeyCheckValue);
	int WriteUserData(BYTE byBlockAddress, LPBYTE lpUserData);
	int ReadUserData(BYTE byBlockAddress, LPBYTE lpUserData);
	int WriteKeyValue(BYTE bMode, LPBYTE lpKeyValueList);
	int ReadEPPInfo(LPBYTE szEPPInfo, LPBYTE lpInfoLen);
	int Backspace();
	int GetDeviceStatus(LPBYTE lpStatus);
	int UpdatePassword(WORD wKeyId, BYTE byMode);
	int RemoveInstall(BYTE byMode);
	int VirtualKeyInput(char *KeyTable, int KeyTableLen);
	int ImportRsaKey(WORD wKeyID, WORD wKeyAttr, WORD wSignKeyID, BYTE bSignAlgorithm, LPBYTE lpKeyTag, WORD wKeyTagLen, BYTE bKeyCheckMode, LPBYTE lpKCV, LPWORD wKCVLen);
	int ExportRSAKey(WORD wKeyId, WORD wsigKeyId, LPBYTE lpSigAlgorithm, LPBYTE lpPublicKey, LPWORD wPublicKeyLen);
	int ImportDesKey(WORD wKeyID, WORD wKeyAttr, WORD wDecryptSK, BYTE bSignAlgorithm, BYTE bVerifyMode, BYTE bKCM, LPBYTE lpKCV, WORD wHOSTPK, BYTE bEncipherAlg, BYTE bRandMode, LPBYTE lpEncryptedKeytag, WORD wKeyTagLen, LPBYTE lpEppKCV, LPWORD wEppKCVLen);
	int ExportDesKey(WORD wKeyId, WORD wKeyAttr, WORD wSignKey, BYTE bSignAlgorithm, WORD wPKEncrypt, BYTE bGenMode, BYTE bHOSTCKM, LPBYTE lpEPPRand, LPBYTE lpHOSTRand, LPBYTE lpHostID, WORD wHOSTSignID, LPBYTE lpEPPSNpackage, WORD wEPPSNpackageLen, LPBYTE lpEPPSymmetricKey, LPWORD wEPPSymmetricKeyLen);
	int RawRsaEncrypt(WORD wRSAModulusLen, LPBYTE lpRSAModulus, WORD wRSAExponentLen, LPBYTE lpRSAExponent, WORD wInDataLen, LPBYTE lpInData, LPWORD lpEncrptedDataLen, LPBYTE lpEncrptedData);
	int RSAEncrypt(WORD wKeyId, LPBYTE lpInputData, WORD wInputDataLen, LPBYTE lpOutData, LPWORD OutDataLen);
	int CreateOffset(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPSTR lpValidationData, BYTE byValDigits, LPSTR lpDecTable, LPSTR lpPINOffset);
	int LocalDes(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPSTR lpValidationData, BYTE byValDigits, LPSTR lpDecTable, LPSTR lpPINOffset);
	int StoreKey(WORD wKeyId, WORD wUse, BYTE byMode, LPBYTE lpKCV, LPBYTE lpKCVL);

private:
	HMODULE                 hModule;
	BOOL                    m_IsOpen;
	ZT_PIN_OPENDEVICE       ZT_PIN_OpenDevice;
	ZT_PIN_CLOSEDEVICE      ZT_PIN_CloseDevice;
	ZT_PIN_RESET            ZT_PIN_Reset;
	ZT_PIN_DELETEKEY        ZT_PIN_DeleteKey;
	ZT_PIN_SETENTRYMODE     ZT_PIN_SetEntryMode;
	ZT_PIN_READINPUTDATA    ZT_PIN_ReadInputData;
	ZT_PIN_IMPORTKEY        ZT_PIN_ImportKey;
	ZT_PIN_GETPRINTBLOCK    ZT_PIN_GetPinBlock;
	ZT_PIN_CRYPT            ZT_PIN_Crypt;
	ZT_PIN_INITIALDEVICE    ZT_PIN_InitialDevice;
	ZT_PIN_READKEYATTRIBUTE ZT_PIN_ReadKeyAttribute;
	ZT_PIN_READKCV          ZT_PIN_ReadKCV;
	ZT_PIN_WRITEUSERDATA    ZT_PIN_WriteUserData;
	ZT_PIN_READUSERDATA     ZT_PIN_ReadUserData;
	ZT_PIN_WRITEKEYVALUE    ZT_PIN_WriteKeyValue;
	ZT_PIN_READEPPINFO      ZT_PIN_ReadEPPInfo;
	ZT_PIN_BACKSPACE        ZT_PIN_Backspace;
	ZT_PIN_GETDEVICESTATUS  ZT_PIN_GetDeviceStatus;
	ZT_PIN_UPDATEPASSWORD   ZT_PIN_UpdatePassword;
	ZT_PIN_REMOVEINSTALL    ZT_PIN_RemoveInstall;
	ZT_PIN_VIRTUALKEYINPUT  ZT_PIN_VirtualKeyInput;
	ZT_PIN_IMPORTRSAKEY     ZT_PIN_ImportRsaKey;
	ZT_PIN_EXPORTRSAKEY     ZT_PIN_ExportRSAKey;
	ZT_PIN_IMPORTDESKEY     ZT_PIN_ImportDesKey;
	ZT_PIN_EXPORTDESKEY     ZT_PIN_ExportDesKey;
	ZT_PIN_RAWRSAENCRYPT    ZT_PIN_RawRsaEncrypt;
	ZT_PIN_RSAENCRYPT       ZT_PIN_RSAEncrypt;
	ZT_PIN_CREATEOFFSET     ZT_PIN_CreateOffset;
	ZT_PIN_LOCALDES         ZT_PIN_LocalDes;
	ZT_PIN_STOREKEY         ZT_PIN_StoreKey;

	void NUllify();
};
}