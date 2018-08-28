// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "ConfigDlg.h"
#include "../include/reg.h"
#include "../helper/helper.h"

#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")


//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
ROBJ_IN_CPP

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	int nRet = 0;
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	TCHAR szPath[MAX_PATH] = { 0 };
	CRegKey reg;
	LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), KEY_READ | KEY_WOW64_64KEY);
	if (ret != ERROR_SUCCESS)
	{
		MessageBox(GetActiveWindow(), _T("注册表损坏"), _T("提示"), MB_OK | MB_ICONSTOP);
		return 1;
	}
	ULONG len = MAX_PATH;
	reg.QueryStringValue(_T("path_client"), szPath, &len);
	reg.Close();

	SStringT strIni = szPath;
	strIni += _T("\\data\\");
	strIni += KSettingINI;


	SComMgr *pComMgr = new SComMgr;
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));

	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
#ifdef _DEBUG
	_tcscpy(lpInsertPos + 1, _T("..\\..\\settings"));
#else
	_tcscpy(lpInsertPos + 1, _T("\0"));
#endif
	SetCurrentDirectory(szCurrentDir);
	{
		BOOL bLoaded = FALSE;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SApplication *theApp = new SApplication(pRenderFactory, hInstance);

		theApp->RegisterWindowClass<STabCtrlEx>();
		theApp->RegisterWindowClass<STabPageEx>();
		theApp->RegisterWindowClass<SQrCtrl>();
		//从DLL加载系统资源
		{
			HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
			if (hModSysResource)
			{
				CAutoRefPtr<IResProvider> sysResProvider;
				CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
				sysResProvider->Init((WPARAM)hModSysResource, 0);
				theApp->LoadSystemNamedResource(sysResProvider);
				FreeLibrary(hModSysResource);
			}
			else
			{
				SASSERT(0);
			}
		}

		CAutoRefPtr<IResProvider>   pResProvider;

#ifdef _DEBUG		
		//选择了仅在Release版本打包资源则在DEBUG下始终使用文件加载
		{
			CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
			bLoaded = pResProvider->Init((LPARAM)_T("uires"), 0);
			SASSERT(bLoaded);
		}
#else
		{
			CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
			bLoaded = pResProvider->Init((WPARAM)hInstance, 0);
			SASSERT(bLoaded);
		}
#endif
		theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
		theApp->AddResProvider(pResProvider);

		HANDLE hMapData = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CSettingsGlobal) + sizeof(CSettingsUI), KFileMapName_GlobalSetting);
		if (!hMapData)
		{
			SLOG_ERROR("open file map object for global settings storage failed");
			return 1;
		}
		else
		{
			bool bCreate = GetLastError() != ERROR_ALREADY_EXISTS;
			g_SettingsG = (CSettingsGlobal*)MapViewOfFile(hMapData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			if (bCreate)
			{//first create object
				Helper_SetObjectToLowIntegrity(hMapData, SE_KERNEL_OBJECT);//降低内核对象访问权限
				g_SettingsG->Load(strIni);
				g_SettingsUI->Load(strIni);
			}
		}

		// BLOCK: Run application
		{
			CConfigDlg dlgMain;
			dlgMain.Create(GetActiveWindow(), WS_POPUP, 0, 0, 0, 0, 0);
			dlgMain.SendMessage(WM_INITDIALOG);
			dlgMain.CenterWindow(dlgMain.m_hWnd);
			dlgMain.ShowWindow(SW_SHOWNORMAL);
			theApp->Run(dlgMain.m_hWnd);
		}

		g_SettingsG->Save(strIni);

		UnmapViewOfFile(g_SettingsG);
		CloseHandle(hMapData);
		delete theApp;
	}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}
