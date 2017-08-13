//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  Register.cpp
//
//          Server registration code.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define CLSID_STRLEN 38  // strlen("{xxxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxx}")

static const TCHAR c_szInfoKeyPrefix[] = TEXT("CLSID\\");
static const TCHAR c_szInProcSvr32[] = TEXT("InProcServer32");
static const TCHAR c_szModelName[] = TEXT("ThreadingModel");



//+---------------------------------------------------------------------------
//
//  RegisterProfiles
//
//----------------------------------------------------------------------------

BOOL RegisterProfiles()
{
    ITfInputProcessorProfiles *pInputProcessProfiles;
    WCHAR achIconFile[MAX_PATH];
    HRESULT hr;

    hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER,
                          IID_ITfInputProcessorProfiles, (void**)&pInputProcessProfiles);

    if (hr != S_OK)
        return E_FAIL;

    hr = pInputProcessProfiles->Register(c_clsidBaidujpTSF);

    if (hr != S_OK)
        goto Exit;

    GetModuleFileNameW(g_hInst, achIconFile, ARRAYSIZE(achIconFile));

    hr = pInputProcessProfiles->AddLanguageProfile(c_clsidBaidujpTSF,
                                  TEXTSERVICE_LANGID, 
                                  c_guidProfile, 
                                  JP_IME_DESC_W, 
                                  (ULONG)wcslen(JP_IME_DESC_W),
                                  achIconFile,
                                  (ULONG)wcslen(achIconFile),
                                  TEXTSERVICE_ICON_INDEX);


	if (hr != S_OK)
		goto Exit;
	//
	// 22:04 2010-11-1 Ren zhi jie
	// ���ǵ������ԣ�������Ҫͬʱ�ṩime, tsf���ֿ���µ��������뷨��������ʹ�û������˲�ʹ�ø߼����ַ����Կ���ͨ�������ṩ��ime���뷨��
	// ��һ���棬 ���ϵͳ�����˸߼����ַ����û���word�������ֱ༭����ѡ���������뷨ʱ��Ϊ�˱���ϵͳͬʱ��ʾ���ǵ�ime, tsf����ģʽ�����뷨��
	// ����ʹ��SubstituteKeyboardLayout���������ã�������ֻ��ʾtsf���뷨���û�ѡ���ˡ�
	//
	HKL hKLBaiduJP=CIMEMan::GetKeyboardLayoutFromFileName( BAIDU_IME_FILE_NAME);
	if ( hKLBaiduJP)
	{
		hr = pInputProcessProfiles->SubstituteKeyboardLayout( c_clsidBaidujpTSF, TEXTSERVICE_LANGID, c_guidProfile, hKLBaiduJP);
		Helper_Trace(_T("SubstituteKeyboardLayout:%d"),SUCCEEDED(hr));
	}

	hr = pInputProcessProfiles->EnableLanguageProfile( c_clsidBaidujpTSF, TEXTSERVICE_LANGID, c_guidProfile, TRUE);
	hr = pInputProcessProfiles->EnableLanguageProfileByDefault( c_clsidBaidujpTSF, TEXTSERVICE_LANGID, c_guidProfile, TRUE);


Exit:
    pInputProcessProfiles->Release();
    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
//  UnregisterProfiles
//
//----------------------------------------------------------------------------

void UnregisterProfiles()
{
    ITfInputProcessorProfiles *pInputProcessProfiles;
    HRESULT hr;

    hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER,
                          IID_ITfInputProcessorProfiles, (void**)&pInputProcessProfiles);

    if (hr != S_OK)
        return;

    pInputProcessProfiles->Unregister(c_clsidBaidujpTSF);
    pInputProcessProfiles->Release();
}

//+---------------------------------------------------------------------------
//
//  RegisterCategories
//
//----------------------------------------------------------------------------

BOOL RegisterCategories()
{
    ITfCategoryMgr *pCategoryMgr;
    HRESULT hr;

    hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, CLSCTX_INPROC_SERVER, 
                          IID_ITfCategoryMgr, (void**)&pCategoryMgr);

    if (hr != S_OK)
        return FALSE;

    //
    // register this text service to GUID_TFCAT_TIP_KEYBOARD category.
    //
    hr = pCategoryMgr->RegisterCategory(c_clsidBaidujpTSF,
                                        GUID_TFCAT_TIP_KEYBOARD, 
                                        c_clsidBaidujpTSF);

    //
    // register this text service to GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER category.
    //
    hr = pCategoryMgr->RegisterCategory(c_clsidBaidujpTSF,
                                        GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER, 
                                        c_clsidBaidujpTSF);


    pCategoryMgr->Release();
    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
//  UnregisterCategories
//
//----------------------------------------------------------------------------

void UnregisterCategories()
{
    ITfCategoryMgr *pCategoryMgr;
    HRESULT hr;

    hr = CoCreateInstance(CLSID_TF_CategoryMgr, NULL, CLSCTX_INPROC_SERVER, 
                          IID_ITfCategoryMgr, (void**)&pCategoryMgr);

    if (hr != S_OK)
        return;

    //
    // unregister this text service from GUID_TFCAT_TIP_KEYBOARD category.
    //
    pCategoryMgr->UnregisterCategory(c_clsidBaidujpTSF,
                                     GUID_TFCAT_TIP_KEYBOARD, 
                                     c_clsidBaidujpTSF);

    //
    // unregister this text service from GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER category.
    //
    pCategoryMgr->UnregisterCategory(c_clsidBaidujpTSF,
                                     GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER, 
                                     c_clsidBaidujpTSF);

    pCategoryMgr->Release();
    return;
}

//+---------------------------------------------------------------------------
//
// CLSIDToString
//
//----------------------------------------------------------------------------

BOOL CLSIDToString(REFGUID refGUID, TCHAR *pch)
{
    static const BYTE GuidMap[] = {3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-',
                                   8, 9, '-', 10, 11, 12, 13, 14, 15};

    static const TCHAR szDigits[] = _T("0123456789ABCDEF");

    int i;
    TCHAR *p = pch;

    const BYTE * pBytes = (const BYTE *) &refGUID;

	*p++ =_T('{');
    for (i = 0; i < sizeof(GuidMap); i++)
    {
        if (GuidMap[i] == '-')
        {
            *p++ = _T('-');
        }
        else
        {
            *p++ = szDigits[ (pBytes[GuidMap[i]] & 0xF0) >> 4 ];
            *p++ = szDigits[ (pBytes[GuidMap[i]] & 0x0F) ];
        }
    }

	*p++ =_T('}');
    *p   =0;

    return TRUE;
}

//+---------------------------------------------------------------------------
//
// RecurseDeleteKey
//
// RecurseDeleteKey is necessary because on NT RegDeleteKey doesn't work if the
// specified key has subkeys
//----------------------------------------------------------------------------
LONG RecurseDeleteKey(HKEY hParentKey, LPCTSTR lpszKey)
{
    HKEY hKey;
    LONG lRes;
    FILETIME time;
    TCHAR szBuffer[256];
    DWORD dwSize = ARRAYSIZE(szBuffer);

    if (RegOpenKey(hParentKey, lpszKey, &hKey) != ERROR_SUCCESS)
        return ERROR_SUCCESS; // assume it couldn't be opened because it's not there

    lRes = ERROR_SUCCESS;
    while (RegEnumKeyEx(hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time)==ERROR_SUCCESS)
    {
        szBuffer[ARRAYSIZE(szBuffer)-1] = '\0';
        lRes = RecurseDeleteKey(hKey, szBuffer);
        if (lRes != ERROR_SUCCESS)
            break;
        dwSize = ARRAYSIZE(szBuffer);
    }
    RegCloseKey(hKey);

    return lRes == ERROR_SUCCESS ? RegDeleteKey(hParentKey, lpszKey) : lRes;
}

//+---------------------------------------------------------------------------
//
//  RegisterServer
//
//----------------------------------------------------------------------------

BOOL RegisterServer()
{
    DWORD dw;
    HKEY hKey;
    HKEY hSubKey;
    BOOL fRet;
    TCHAR achIMEKey[ARRAYSIZE(c_szInfoKeyPrefix) + CLSID_STRLEN];
    TCHAR achFileName[MAX_PATH];

    if (!CLSIDToString(c_clsidBaidujpTSF, achIMEKey + ARRAYSIZE(c_szInfoKeyPrefix) - 1))
        return FALSE;
    memcpy(achIMEKey, c_szInfoKeyPrefix, sizeof(c_szInfoKeyPrefix)-sizeof(TCHAR));

    if (fRet = RegCreateKeyEx(HKEY_CLASSES_ROOT, achIMEKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dw)
            == ERROR_SUCCESS)
    {
        fRet &= RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)JP_IME_DESC, (_tcslen(JP_IME_DESC)+1)*sizeof(TCHAR))
            == ERROR_SUCCESS;

        if (fRet &= RegCreateKeyEx(hKey, c_szInProcSvr32, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, &dw)
            == ERROR_SUCCESS)
        {
            dw = GetModuleFileName(g_hInst, achFileName, ARRAYSIZE(achFileName));

            fRet &= RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (BYTE *)achFileName, (lstrlen(achFileName)+1)*sizeof(TCHAR)) == ERROR_SUCCESS;
            fRet &= RegSetValueEx(hSubKey, c_szModelName, 0, REG_SZ, (BYTE *)TEXTSERVICE_MODEL, (_tcslen(TEXTSERVICE_MODEL)+1)*sizeof(TCHAR)) == ERROR_SUCCESS;
            RegCloseKey(hSubKey);
        }
        RegCloseKey(hKey);
    }

    return fRet;
}

//+---------------------------------------------------------------------------
//
//  UnregisterServer
//
//----------------------------------------------------------------------------

void UnregisterServer()
{
    TCHAR achIMEKey[ARRAYSIZE(c_szInfoKeyPrefix) + CLSID_STRLEN];

    if (!CLSIDToString(c_clsidBaidujpTSF, achIMEKey + ARRAYSIZE(c_szInfoKeyPrefix) - 1))
        return;
    memcpy(achIMEKey, c_szInfoKeyPrefix, sizeof(c_szInfoKeyPrefix)-sizeof(TCHAR));

    RecurseDeleteKey(HKEY_CLASSES_ROOT, achIMEKey);
}