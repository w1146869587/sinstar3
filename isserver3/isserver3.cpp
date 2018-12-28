// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "IsSvrProxy.h"
#include "../include/reg.h"
#include "BuildIndexProgWnd.h"
#include "Minidump.h"
#include "ui/SkinMananger.h"

#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")


//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
ROBJ_IN_CPP

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	CMinidump::Enable();
	int nRet = 0;
	HANDLE hMapData = OpenFileMapping(FILE_MAP_READ, FALSE, NAME_ACK_MAPFILE);
	if (hMapData)
	{
		HWND hPrev = 0;
		LPVOID pData = MapViewOfFile(hMapData, FILE_MAP_READ, 0, 0, 0);
		memcpy(&hPrev, pData, sizeof(HWND));
		UnmapViewOfFile(pData);
		CloseHandle(hMapData);
		if (IsWindow(hPrev))
		{
			return 4;
		}
	}

	TCHAR szDataPath[MAX_PATH];
	CRegKey reg;
	LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), KEY_READ | KEY_WOW64_64KEY);
	if (ret != ERROR_SUCCESS)
	{
		return 1;
	}
	ULONG len = MAX_PATH;
	reg.QueryStringValue(_T("path_svr_data"), szDataPath, &len);
	reg.Close();
	if (GetFileAttributes(szDataPath) == INVALID_FILE_ATTRIBUTES)
	{
		return 2;
	}

	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	SComMgr *pComMgr = new SComMgr;
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));

	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
#ifdef _DEBUG
	_tcscpy(lpInsertPos + 1, _T("..\\..\\isserver3"));
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
		theApp->RegisterWindowClass<SChatEdit>();
		theApp->RegisterWindowClass<SToggle2>();
		theApp->RegisterWindowClass<SCandView>();
		theApp->RegisterWindowClass<SSentView>();
		theApp->RegisterWindowClass<SEnglishCand>();
		theApp->RegisterWindowClass<SPhraseCand>();
		theApp->RegisterWindowClass<SSpellView>();
		theApp->RegisterWindowClass<SCaption2>();
		theApp->RegisterWindowClass<SCompBackground>();
		theApp->RegisterWindowClass<SStatusBackground>();
		theApp->RegisterWindowClass<SMutexView>();
		theApp->RegisterWindowClass<SFlagView>();
		theApp->RegisterWindowClass<SLineComp>();
		theApp->RegisterWindowClass<SApngPlayer>();

		theApp->RegisterSkinClass<SSkinAPNG>();
		theApp->RegisterSkinClass<SSkinPNGX>();
		theApp->RegisterSkinClass<SLineSkin>();

		CAutoRefPtr<ILog4zManager> pLogMgr;
		bLoaded = pComMgr->CreateLog4z((IObjRef**)&pLogMgr);
		SASSERT_FMT(bLoaded, _T("load ILog4zManager failed!"), _T("log4z"));

		theApp->SetLogManager(pLogMgr);

		SStringT strLogDir = szCurrentDir;
		strLogDir += _T("\\log");
		pLogMgr->setLoggerPath(0, S_CT2A(strLogDir));
		pLogMgr->start();

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


		TCHAR szPath[MAX_PATH];
		CRegKey reg;
		LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), KEY_READ | KEY_WOW64_64KEY);
		if (ret == ERROR_SUCCESS)
		{
			ULONG len = MAX_PATH;
			reg.QueryStringValue(_T("path_client"), szPath, &len);
			reg.Close();
		}
		SStringT strDataPath = szPath;
		SStringT strConfigIni = strDataPath + _T("\\data\\") + KSettingINI;

		new CDataCenter(strDataPath);
		g_SettingsG = new CSettingsGlobal();
		g_SettingsUI = new CSettingsUI();

		g_SettingsG->Load(strConfigIni);
		g_SettingsUI->Load(strConfigIni);

		if (g_SettingsUI->szSkin[0])
		{//load skin
			CAutoRefPtr<IResProvider>   pSkinProvider;
			pComMgr->CreateResProvider_ZIP((IObjRef**)&pSkinProvider);
			ZIPRES_PARAM param;
			param.ZipFile(GETRENDERFACTORY, g_SettingsUI->szSkin);
			if (!pSkinProvider->Init((WPARAM)&param, 0))
			{
				g_SettingsUI->szSkin[0] = 0;
			}
			else
			{
				IUiDefInfo * pUiDef = SUiDef::CreateUiDefInfo2(pSkinProvider, _T("uidef:xml_init"));
				if (!pUiDef->GetSkinPool() && !pUiDef->GetStylePool() && !pUiDef->GetObjDefAttr())
				{//不允许皮肤中存在全局的skin, style and defobjattr数据
					theApp->AddResProvider(pSkinProvider, NULL);

					IUiDefInfo * pBuiltinUidef = SUiDef::getSingleton().GetUiDef();
					pUiDef->SetObjDefAttr(pBuiltinUidef->GetObjDefAttr());
					pUiDef->SetStylePool(pBuiltinUidef->GetStylePool());
					pUiDef->SetSkinPool(pBuiltinUidef->GetSkinPool());
					pUiDef->GetNamedColor().Merge(pBuiltinUidef->GetNamedColor());
					pUiDef->GetNamedString().Merge(pBuiltinUidef->GetNamedString());
					pUiDef->GetNamedDimension().Merge(pBuiltinUidef->GetNamedDimension());

					SUiDef::getSingleton().SetUiDef(pUiDef);
					CDataCenter::getSingletonPtr()->GetData().m_ptSkinOffset = CSkinMananger::ExtractSkinOffset(pResProvider);
				}
				else
				{//外置皮肤中禁止出现全局skin, style and defobjattr表。
					SLOG_WARN("previous skin is invalid");
					g_SettingsUI->szSkin[0] = 0;
				}
				pUiDef->Release();
			}

		}
		// BLOCK: Run application
		{
			CIsSvrProxy svrProxy(szDataPath);
			svrProxy.Create(SINSTAR3_SERVER_HWND, WS_POPUP, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL);
			if (!svrProxy.IsWindow())
			{
				nRet = 3;
			}
			else
			{
				nRet = theApp->Run(svrProxy.m_hWnd);
			}
		}

		g_SettingsG->Save(strConfigIni);
		g_SettingsUI->Save(strConfigIni);

		delete g_SettingsG;
		delete g_SettingsUI;
		delete theApp;
	}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}
