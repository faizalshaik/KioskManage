// original name : PIN_CS04_598M.cpp
// more info @   : PIN_CS04_598M.h

#include "pin_cs04_598m.h"

using namespace codesaur;

EPP_ZT598M::EPP_ZT598M()
{
	hModule = ::LoadLibrary(TEXT("PIN_CS04_598M.dll"));

	if (GetHandle())
	{
		ZT_PIN_OpenDevice = (ZT_PIN_OPENDEVICE) ::GetProcAddress(GetHandle(), "ZT_PIN_OpenDevice");
		ZT_PIN_CloseDevice = (ZT_PIN_CLOSEDEVICE) ::GetProcAddress(GetHandle(), "ZT_PIN_CloseDevice");
		ZT_PIN_Reset = (ZT_PIN_RESET) ::GetProcAddress(GetHandle(), "ZT_PIN_Reset");
		ZT_PIN_DeleteKey = (ZT_PIN_DELETEKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_DeleteKey");
		ZT_PIN_SetEntryMode = (ZT_PIN_SETENTRYMODE) ::GetProcAddress(GetHandle(), "ZT_PIN_SetEntryMode");
		ZT_PIN_ReadInputData = (ZT_PIN_READINPUTDATA) ::GetProcAddress(GetHandle(), "ZT_PIN_ReadInputData");
		ZT_PIN_ImportKey = (ZT_PIN_IMPORTKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_ImportKey");
		ZT_PIN_GetPinBlock = (ZT_PIN_GETPRINTBLOCK) ::GetProcAddress(GetHandle(), "ZT_PIN_GetPinBlock");
		ZT_PIN_Crypt = (ZT_PIN_CRYPT) ::GetProcAddress(GetHandle(), "ZT_PIN_Crypt");
		ZT_PIN_InitialDevice = (ZT_PIN_INITIALDEVICE) ::GetProcAddress(GetHandle(), "ZT_PIN_InitialDevice");
		ZT_PIN_ReadKeyAttribute = (ZT_PIN_READKEYATTRIBUTE) ::GetProcAddress(GetHandle(), "ZT_PIN_ReadKeyAttribute");
		ZT_PIN_ReadKCV = (ZT_PIN_READKCV) ::GetProcAddress(GetHandle(), "ZT_PIN_ReadKCV");
		ZT_PIN_WriteUserData = (ZT_PIN_WRITEUSERDATA) ::GetProcAddress(GetHandle(), "ZT_PIN_WriteUserData");
		ZT_PIN_ReadUserData = (ZT_PIN_READUSERDATA) ::GetProcAddress(GetHandle(), "ZT_PIN_ReadUserData");
		ZT_PIN_WriteKeyValue = (ZT_PIN_WRITEKEYVALUE) ::GetProcAddress(GetHandle(), "ZT_PIN_WriteKeyValue");
		ZT_PIN_ReadEPPInfo = (ZT_PIN_READEPPINFO) ::GetProcAddress(GetHandle(), "ZT_PIN_ReadEPPInfo");
		ZT_PIN_Backspace = (ZT_PIN_BACKSPACE) ::GetProcAddress(GetHandle(), "ZT_PIN_Backspace");
		ZT_PIN_GetDeviceStatus = (ZT_PIN_GETDEVICESTATUS) ::GetProcAddress(GetHandle(), "ZT_PIN_GetDeviceStatus");
		ZT_PIN_UpdatePassword = (ZT_PIN_UPDATEPASSWORD) ::GetProcAddress(GetHandle(), "ZT_PIN_UpdatePassword");
		ZT_PIN_RemoveInstall = (ZT_PIN_REMOVEINSTALL) ::GetProcAddress(GetHandle(), "ZT_PIN_RemoveInstall");
		ZT_PIN_VirtualKeyInput = (ZT_PIN_VIRTUALKEYINPUT) ::GetProcAddress(GetHandle(), "ZT_PIN_VirtualKeyInput");
		ZT_PIN_ImportRsaKey = (ZT_PIN_IMPORTRSAKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_ImportRsaKey");
		ZT_PIN_ExportRSAKey = (ZT_PIN_EXPORTRSAKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_ExportRSAKey");
		ZT_PIN_ImportDesKey = (ZT_PIN_IMPORTDESKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_ImportDesKey");
		ZT_PIN_ExportDesKey = (ZT_PIN_EXPORTDESKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_ExportDesKey");
		ZT_PIN_RawRsaEncrypt = (ZT_PIN_RAWRSAENCRYPT) ::GetProcAddress(GetHandle(), "ZT_PIN_RawRsaEncrypt");
		ZT_PIN_RSAEncrypt = (ZT_PIN_RSAENCRYPT) ::GetProcAddress(GetHandle(), "ZT_PIN_RSAEncrypt");
		ZT_PIN_CreateOffset = (ZT_PIN_CREATEOFFSET) ::GetProcAddress(GetHandle(), "ZT_PIN_CreateOffset");
		ZT_PIN_LocalDes = (ZT_PIN_LOCALDES) ::GetProcAddress(GetHandle(), "ZT_PIN_LocalDes");
		ZT_PIN_StoreKey = (ZT_PIN_STOREKEY) ::GetProcAddress(GetHandle(), "ZT_PIN_StoreKey");
	}
	else
		NUllify();

	m_IsOpen = FALSE;
}

EPP_ZT598M::~EPP_ZT598M()
{
	if (GetHandle())
	{
		if (IsOpen())
			CloseDevice();
		
		::FreeLibrary(hModule);
		NUllify();
	}
}

HMODULE EPP_ZT598M::GetHandle()
{
	return hModule;
}

BOOL EPP_ZT598M::IsLoaded()
{
	return hModule != NULL;
}

BOOL EPP_ZT598M::IsOpen()
{
	return m_IsOpen;
}

int EPP_ZT598M::OpenDevice(BYTE byType, LPCTSTR szDescription)
{
	if (IsOpen())
		CloseDevice();

	int result;
	if (ZT_PIN_OpenDevice)
		result = ZT_PIN_OpenDevice(byType, szDescription);
	else
		result = ERR_NO_DLL;

	m_IsOpen = (0 == result);

	return result;
}

int EPP_ZT598M::CloseDevice()
{
	return (ZT_PIN_CloseDevice) ? ZT_PIN_CloseDevice() : ERR_NO_DLL;
}

int EPP_ZT598M::Reset()
{
	return (ZT_PIN_Reset) ? ZT_PIN_Reset() : ERR_NO_DLL;
}

int EPP_ZT598M::DeleteKey(WORD wKeyId)
{
	return (ZT_PIN_DeleteKey) ? ZT_PIN_DeleteKey(wKeyId) : ERR_NO_DLL;
}

int EPP_ZT598M::SetEntryMode(BYTE byMode)
{
	return (ZT_PIN_SetEntryMode) ? ZT_PIN_SetEntryMode(byMode) : ERR_NO_DLL;
}

int EPP_ZT598M::ReadInputData(LPBYTE lpInputData, LPBYTE lpInputDataLen)
{
	return (ZT_PIN_ReadInputData) ? ZT_PIN_ReadInputData(lpInputData, lpInputDataLen) : ERR_NO_DLL;
}

int EPP_ZT598M::ImportKey(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPBYTE lpValue, BYTE byValeLen, WORD wUse, BYTE byMode, LPBYTE lpKCV, LPBYTE lpKCVL)
{
	return (ZT_PIN_ImportKey) ? ZT_PIN_ImportKey(wKeyId, wEnKeyId, wVI, lpValue, byValeLen, wUse, byMode, lpKCV, lpKCVL) : ERR_NO_DLL;
}

int EPP_ZT598M::GetPinBlock(WORD wKeyId, WORD wEnKeyId, WORD wVI, WORD wFormat, LPSTR lpCustomerData, LPSTR lpXORData, BYTE byPadding, BYTE byPinLen, LPBYTE lpPinBlock, LPBYTE lpPinBlockLen)
{
	return (ZT_PIN_GetPinBlock) ? ZT_PIN_GetPinBlock(wKeyId, wEnKeyId, wVI, wFormat, lpCustomerData, lpXORData, byPadding, byPinLen, lpPinBlock, lpPinBlockLen) : ERR_NO_DLL;
}

int EPP_ZT598M::Crypt(WORD wKeyId, LPBYTE lpEnKeyValue, BYTE byEnKeyValueLen, WORD wVI, WORD wMode, WORD wAlgorithm, WORD wStartValueKeyId, LPBYTE lpStartValue, LPBYTE lpCryptData, WORD wCryptDataLen, LPBYTE lpOutCryptData, LPWORD lpOutCryptDataLen)
{
	return (ZT_PIN_Crypt) ? ZT_PIN_Crypt(wKeyId, lpEnKeyValue, byEnKeyValueLen, wVI, wMode, wAlgorithm, wStartValueKeyId, lpStartValue, lpCryptData, wCryptDataLen, lpOutCryptData, lpOutCryptDataLen) : ERR_NO_DLL;
}

int EPP_ZT598M::InitialDevice(BYTE byMode)
{
	return (ZT_PIN_InitialDevice) ? ZT_PIN_InitialDevice(byMode) : ERR_NO_DLL;
}

int EPP_ZT598M::ReadKeyAttribute(WORD wKeyId, LPWORD lpKeyAttributes, LPBYTE lpKeyCheckValue)
{
	return (ZT_PIN_ReadKeyAttribute) ? ZT_PIN_ReadKeyAttribute(wKeyId, lpKeyAttributes, lpKeyCheckValue) : ERR_NO_DLL;
}

int EPP_ZT598M::ReadKCV(WORD wKeyId, LPBYTE lpKeyCheckValue)
{
	return (ZT_PIN_ReadKCV) ? ZT_PIN_ReadKCV(wKeyId, lpKeyCheckValue) : ERR_NO_DLL;
}

int EPP_ZT598M::WriteUserData(BYTE byBlockAddress, LPBYTE lpUserData)
{
	return (ZT_PIN_WriteUserData) ? ZT_PIN_WriteUserData(byBlockAddress, lpUserData) : ERR_NO_DLL;
}

int EPP_ZT598M::ReadUserData(BYTE byBlockAddress, LPBYTE lpUserData)
{
	return (ZT_PIN_ReadUserData) ? ZT_PIN_ReadUserData(byBlockAddress, lpUserData) : ERR_NO_DLL;
}

int EPP_ZT598M::WriteKeyValue(BYTE bMode, LPBYTE lpKeyValueList)
{
	return (ZT_PIN_WriteKeyValue) ? ZT_PIN_WriteKeyValue(bMode, lpKeyValueList) : ERR_NO_DLL;
}

int EPP_ZT598M::ReadEPPInfo(LPBYTE szEPPInfo, LPBYTE lpInfoLen)
{
	return (ZT_PIN_ReadEPPInfo) ? ZT_PIN_ReadEPPInfo(szEPPInfo, lpInfoLen) : ERR_NO_DLL;
}

int EPP_ZT598M::Backspace()
{
	return (ZT_PIN_Backspace) ? ZT_PIN_Backspace() : ERR_NO_DLL;
}

int EPP_ZT598M::GetDeviceStatus(LPBYTE lpStatus)
{
	return (ZT_PIN_GetDeviceStatus) ? ZT_PIN_GetDeviceStatus(lpStatus) : ERR_NO_DLL;
}

int EPP_ZT598M::UpdatePassword(WORD wKeyId, BYTE byMode)
{
	return (ZT_PIN_UpdatePassword) ? ZT_PIN_UpdatePassword(wKeyId, byMode) : ERR_NO_DLL;
}

int EPP_ZT598M::RemoveInstall(BYTE byMode)
{
	return (ZT_PIN_RemoveInstall) ? ZT_PIN_RemoveInstall(byMode) : ERR_NO_DLL;
}

int EPP_ZT598M::VirtualKeyInput(char *KeyTable, int KeyTableLen)
{
	return (ZT_PIN_VirtualKeyInput) ? ZT_PIN_VirtualKeyInput(KeyTable, KeyTableLen) : ERR_NO_DLL;
}

int EPP_ZT598M::ImportRsaKey(WORD wKeyID, WORD wKeyAttr, WORD wSignKeyID, BYTE bSignAlgorithm, LPBYTE lpKeyTag, WORD wKeyTagLen, BYTE bKeyCheckMode, LPBYTE lpKCV, LPWORD wKCVLen)
{
	return (ZT_PIN_ImportRsaKey) ? ZT_PIN_ImportRsaKey(wKeyID, wKeyAttr, wSignKeyID, bSignAlgorithm, lpKeyTag, wKeyTagLen, bKeyCheckMode, lpKCV, wKCVLen) : ERR_NO_DLL;
}

int EPP_ZT598M::ExportRSAKey(WORD wKeyId, WORD wsigKeyId, LPBYTE lpSigAlgorithm, LPBYTE lpPublicKey, LPWORD wPublicKeyLen)
{
	return (ZT_PIN_ExportRSAKey) ? ZT_PIN_ExportRSAKey(wKeyId, wsigKeyId, lpSigAlgorithm, lpPublicKey, wPublicKeyLen) : ERR_NO_DLL;
}

int EPP_ZT598M::ImportDesKey(WORD wKeyID, WORD wKeyAttr, WORD wDecryptSK, BYTE bSignAlgorithm, BYTE bVerifyMode, BYTE bKCM, LPBYTE lpKCV, WORD wHOSTPK, BYTE bEncipherAlg, BYTE bRandMode, LPBYTE lpEncryptedKeytag, WORD wKeyTagLen, LPBYTE lpEppKCV, LPWORD wEppKCVLen)
{
	return (ZT_PIN_ImportDesKey) ? ZT_PIN_ImportDesKey(wKeyID, wKeyAttr, wDecryptSK, bSignAlgorithm, bVerifyMode, bKCM, lpKCV, wHOSTPK, bEncipherAlg, bRandMode, lpEncryptedKeytag, wKeyTagLen, lpEppKCV, wEppKCVLen) : ERR_NO_DLL;
}

int EPP_ZT598M::ExportDesKey(WORD wKeyId, WORD wKeyAttr, WORD wSignKey, BYTE bSignAlgorithm, WORD wPKEncrypt, BYTE bGenMode, BYTE bHOSTCKM, LPBYTE lpEPPRand, LPBYTE lpHOSTRand, LPBYTE lpHostID, WORD wHOSTSignID, LPBYTE lpEPPSNpackage, WORD wEPPSNpackageLen, LPBYTE lpEPPSymmetricKey, LPWORD wEPPSymmetricKeyLen)
{
	return (ZT_PIN_ExportDesKey) ? ZT_PIN_ExportDesKey(wKeyId, wKeyAttr, wSignKey, bSignAlgorithm, wPKEncrypt, bGenMode, bHOSTCKM, lpEPPRand, lpHOSTRand, lpHostID, wHOSTSignID, lpEPPSNpackage, wEPPSNpackageLen, lpEPPSymmetricKey, wEPPSymmetricKeyLen) : ERR_NO_DLL;
}

int EPP_ZT598M::RawRsaEncrypt(WORD wRSAModulusLen, LPBYTE lpRSAModulus, WORD wRSAExponentLen, LPBYTE lpRSAExponent, WORD wInDataLen, LPBYTE lpInData, LPWORD lpEncrptedDataLen, LPBYTE lpEncrptedData)
{
	return (ZT_PIN_RawRsaEncrypt) ? ZT_PIN_RawRsaEncrypt(wRSAModulusLen, lpRSAModulus, wRSAExponentLen, lpRSAExponent, wInDataLen, lpInData, lpEncrptedDataLen, lpEncrptedData) : ERR_NO_DLL;
}

int EPP_ZT598M::RSAEncrypt(WORD wKeyId, LPBYTE lpInputData, WORD wInputDataLen, LPBYTE lpOutData, LPWORD OutDataLen)
{
	return (ZT_PIN_RSAEncrypt) ? ZT_PIN_RSAEncrypt(wKeyId, lpInputData, wInputDataLen, lpOutData, OutDataLen) : ERR_NO_DLL;
}

int EPP_ZT598M::CreateOffset(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPSTR lpValidationData, BYTE byValDigits, LPSTR lpDecTable, LPSTR lpPINOffset)
{
	return (ZT_PIN_CreateOffset) ? ZT_PIN_CreateOffset(wKeyId, wEnKeyId, wVI, lpValidationData, byValDigits, lpDecTable, lpPINOffset) : ERR_NO_DLL;
}

int EPP_ZT598M::LocalDes(WORD wKeyId, WORD wEnKeyId, WORD wVI, LPSTR lpValidationData, BYTE byValDigits, LPSTR lpDecTable, LPSTR lpPINOffset)
{
	return (ZT_PIN_LocalDes) ? ZT_PIN_LocalDes(wKeyId, wEnKeyId, wVI, lpValidationData, byValDigits, lpDecTable, lpPINOffset) : ERR_NO_DLL;
}

int EPP_ZT598M::StoreKey(WORD wKeyId, WORD wUse, BYTE byMode, LPBYTE lpKCV, LPBYTE lpKCVL)
{
	return (ZT_PIN_StoreKey) ? ZT_PIN_StoreKey(wKeyId, wUse, byMode, lpKCV, lpKCVL) : ERR_NO_DLL;
}

void EPP_ZT598M::NUllify()
{
	ZT_PIN_OpenDevice = NULL;
	ZT_PIN_CloseDevice = NULL;
	ZT_PIN_Reset = NULL;
	ZT_PIN_DeleteKey = NULL;
	ZT_PIN_SetEntryMode = NULL;
	ZT_PIN_ReadInputData = NULL;
	ZT_PIN_ImportKey = NULL;
	ZT_PIN_GetPinBlock = NULL;
	ZT_PIN_Crypt = NULL;
	ZT_PIN_InitialDevice = NULL;
	ZT_PIN_ReadKeyAttribute = NULL;
	ZT_PIN_ReadKCV = NULL;
	ZT_PIN_WriteUserData = NULL;
	ZT_PIN_ReadUserData = NULL;
	ZT_PIN_WriteKeyValue = NULL;
	ZT_PIN_ReadEPPInfo = NULL;
	ZT_PIN_Backspace = NULL;
	ZT_PIN_GetDeviceStatus = NULL;
	ZT_PIN_UpdatePassword = NULL;
	ZT_PIN_RemoveInstall = NULL;
	ZT_PIN_VirtualKeyInput = NULL;
	ZT_PIN_ImportRsaKey = NULL;
	ZT_PIN_ExportRSAKey = NULL;
	ZT_PIN_ImportDesKey = NULL;
	ZT_PIN_ExportDesKey = NULL;
	ZT_PIN_RawRsaEncrypt = NULL;
	ZT_PIN_RSAEncrypt = NULL;
	ZT_PIN_CreateOffset = NULL;
	ZT_PIN_LocalDes = NULL;
	ZT_PIN_StoreKey = NULL;
}