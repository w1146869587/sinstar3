// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include <helper/SDpiHelper.hpp>	

#define MAX_STRLEN		100
#define SINSTAR3_SERVER_HWND _T("sinstar3_server_wnd_{85B55CBC-7D48-4860-BA88-0BE4B073A94F}")

TCHAR g_szPath[MAX_PATH] = { 0 };	//程序启动位置

const TCHAR * KFiles[] =
{
	_T("program\\isserver3.exe"),
	_T("program\\sinstar3_ime.dll"),
	_T("program\\x64\\sinstar3_ime.dll"),
	_T("program\\sinstar3_tsf.dll"),
	_T("program\\x64\\sinstar3_tsf.dll"),
};


struct CopyInfo {
	const TCHAR *pszSrc;
	const TCHAR *pszDest;
	bool   bReg;
};

const CopyInfo KSrcX86Files[] =
{
	{ _T("program\\sinstar3_ime.dll"),_T("sinstar3_ime.ime"),false },
	{ _T("program\\sinstar3_tsf.dll"),_T("sinstar3_tsf.dll"),true },
};

const CopyInfo KSrcX64Files[] =
{
	{ _T("program\\x64\\sinstar3_ime.dll"),_T("sinstar3_ime.ime"),true },
	{ _T("program\\x64\\sinstar3_tsf.dll"),_T("sinstar3_tsf.dll"),true },
};

struct UpdateInfo
{
	LPCTSTR pszFile;
	bool	bClean;	//true- delete at first.
};

const UpdateInfo KUpdateInfo[] =
{
	{_T("program"),true},
	{_T("defskin"),true },
	{ _T("sound"),true },
	{_T("skins"),false},
	{ _T("tools"),false },
	{_T("register.exe"),false },
	{_T("license.rtf"),false },
	{_T("使用说明.txt"),false },
};

BOOL Is64OS()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void ShowCaller()
{
	TCHAR szPath[MAX_PATH];
	STARTUPINFO si = { sizeof(STARTUPINFO),0 };
	PROCESS_INFORMATION pi = { 0 };
	_stprintf(szPath, _T("%s\\program\\findcallerUI.exe sinstar3_ime.ime|sinstar3_tsf.dll"), g_szPath);
	if (CreateProcess(NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		SMessageBox(GetActiveWindow(), _T("启动FindCallerUI失败"), _T("提示"), MB_OK | MB_ICONSTOP);
	}
}

DWORD CallRegsvr32(LPCTSTR pszFileName, BOOL bReg)
{
	STARTUPINFO         si = { 0 };
	PROCESS_INFORMATION pi;
	DWORD dwWaitRet = -1;
	si.cb = sizeof(si);
	si.dwFlags = 0;
	si.wShowWindow = 0;

	TCHAR szSysDir[MAX_PATH];
	GetSystemDirectory(szSysDir, MAX_PATH);

	TCHAR szCmd[1000];
	_stprintf(szCmd, _T("regsvr32 /s %s %s"), bReg ? _T("") : _T("/u"), pszFileName);
	//以后台启动方式创建服务器进程
	if (!CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP, NULL, szSysDir, &si, &pi))
	{
		return FALSE;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &dwWaitRet);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return dwWaitRet;
}

typedef UINT
(WINAPI
	*FunGetSystemWow64Directory)(
		LPTSTR lpBuffer,
		UINT uSize
		);


BOOL GetSysWow64Dir(LPTSTR pszDir, int nLen)
{
	if (!Is64OS())
		return FALSE;
	HMODULE hMod = LoadLibrary(_T("Kernel32.dll"));
#ifdef _UNICODE
	FunGetSystemWow64Directory fGetSystemWow64Directory = (FunGetSystemWow64Directory)GetProcAddress(hMod, "GetSystemWow64DirectoryW");
#else
	FunGetSystemWow64Directory fGetSystemWow64Directory = (FunGetSystemWow64Directory)GetProcAddress(hMod, "GetSystemWow64DirectoryA");
#endif
	if (!fGetSystemWow64Directory) return FALSE;
	fGetSystemWow64Directory(pszDir, nLen);
	FreeLibrary(hMod);
	return TRUE;
}

BOOL Sinstar_Update(BOOL bSilent=FALSE)
{
	TCHAR szPath1[MAX_PATH], szPath2[MAX_PATH], szSysPath[MAX_PATH], szSysWow64[MAX_PATH];
	::GetSystemDirectory(szSysPath, MAX_PATH);
	GetSysWow64Dir(szSysWow64, MAX_PATH);
	TCHAR szMsgCaption[]=_T("启程输入法");

	HANDLE hMutex = CreateMutex(NULL, FALSE, SINSTAR3_MUTEX);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		if (SMessageBox(GetActiveWindow(), _T("输入法正在使用，不能更新，查看哪些程序在使用吗？"), szMsgCaption, MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			ShowCaller();
		}
		return 0;
	}
	CloseHandle(hMutex);

	BOOL bCopied = TRUE;
	if (Is64OS())
	{
		//copy x64 files to system dir
		for (int i = 0; i<ARRAYSIZE(KSrcX64Files) && bCopied; i++)
		{
			_stprintf(szPath1, _T("%s\\%s"), g_szPath, KSrcX64Files[i].pszSrc);
			_stprintf(szPath2, _T("%s\\%s"), szSysPath, KSrcX64Files[i].pszDest);
			bCopied = CopyFile(szPath1, szPath2, FALSE);
		}

		//copy x86 files to wow64 dir.
		for (int i = 0; i<ARRAYSIZE(KSrcX86Files) && bCopied; i++)
		{
			_stprintf(szPath1, _T("%s\\%s"), g_szPath, KSrcX86Files[i].pszSrc);
			_stprintf(szPath2, _T("%s\\%s"), szSysWow64, KSrcX86Files[i].pszDest);
			bCopied = CopyFile(szPath1, szPath2, FALSE);
		}
	}
	else
	{
		//copy x86 files to system dir
		for (int i = 0; i<ARRAYSIZE(KSrcX86Files) && bCopied; i++)
		{
			_stprintf(szPath1, _T("%s\\%s"), g_szPath, KSrcX86Files[i].pszSrc);
			_stprintf(szPath2, _T("%s\\%s"), szSysPath, KSrcX86Files[i].pszDest);
			bCopied = CopyFile(szPath1, szPath2, FALSE);
		}
	}
	if (bCopied)
	{
		if(!bSilent) SMessageBox(GetActiveWindow(), _T("更新成功！"), szMsgCaption, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		SMessageBox(GetActiveWindow(), _T("复制文件到系统目录失败！"), szMsgCaption, MB_OK|MB_ICONSTOP);
	}
	return bCopied;
}

BOOL Sinstar_Uninstall()
{
	TCHAR szSysPath[MAX_PATH], sysWow64[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	::GetSystemDirectory(szSysPath, MAX_PATH);
	GetSysWow64Dir(sysWow64, MAX_PATH);
	TCHAR szMsgCaption[]=_T("启程输入法-卸载");
	HANDLE hMutex = CreateMutex(NULL, FALSE, SINSTAR3_MUTEX);
	if (!hMutex)
	{
		SMessageBox(GetActiveWindow(), _T("mutex create error"), _T("?"), MB_OK);
		return 0;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		if (SMessageBox(GetActiveWindow(), _T("输入法正在使用，不能卸载，查看哪些程序在使用吗？"), szMsgCaption, MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			ShowCaller();
		}
		return 0;
	}
	CloseHandle(hMutex);
	if (SMessageBox(GetActiveWindow(), _T("确定卸载吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return 0;

	//删除输入法文件
	if (Is64OS())
	{
		for (int i = 0; i<ARRAYSIZE(KSrcX64Files); i++)
		{
			_stprintf(szPath, _T("%s\\%s"), szSysPath, KSrcX64Files[i].pszDest);
			if (KSrcX64Files[i].bReg && 0 != CallRegsvr32(szPath, FALSE))
			{
				SMessageBox(GetActiveWindow(), _T("卸载程序失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return 0;
			}
			DeleteFile(szPath);
		}

		for (int i = 0; i<ARRAYSIZE(KSrcX86Files); i++)
		{
			_stprintf(szPath, _T("%s\\%s"), sysWow64, KSrcX86Files[i].pszDest);
			if (KSrcX86Files[i].bReg && 0 != CallRegsvr32(szPath, FALSE))
			{
				SMessageBox(GetActiveWindow(), _T("卸载程序失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return 0;
			}
			DeleteFile(szPath);
		}
	}
	else
	{
		for (int i = 0; i<ARRAYSIZE(KSrcX86Files); i++)
		{
			_stprintf(szPath, _T("%s\\%s"), szSysPath, KSrcX86Files[i].pszDest);
			if (0 != CallRegsvr32(szPath, FALSE))
			{
				SMessageBox(GetActiveWindow(), _T("反注册输入法接口失败!"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return 0;
			}
			DeleteFile(szPath);
		}
	}

	//delete reg
	CRegKey reg;
	LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft"), KEY_WRITE | KEY_READ | KEY_WOW64_64KEY);
	if (ret != ERROR_SUCCESS)
	{
		SMessageBox(GetActiveWindow(), _T("打开注册表失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
		return 0;
	}

	TCHAR szClient[MAX_PATH];
	ULONG uSize = MAX_PATH;
	reg.QueryStringValue(_T("path_client"), szClient, &uSize);

	reg.RecurseDeleteKey(_T("sinstar3"));
	reg.Close();


	//退出服务器
	HWND hWndSvr = FindWindow(NULL, SINSTAR3_SERVER_HWND);
	if (IsWindow(hWndSvr))
	{
		DWORD dwProcID = 0;
		GetWindowThreadProcessId(hWndSvr, &dwProcID);
		PostMessage(hWndSvr, WM_QUIT, 0, 0);
		if (dwProcID != 0)
			WaitForSingleObject((HANDLE)dwProcID, INFINITE);
		else
			Sleep(500);
	}

	//step8:reg ime file type.
	TCHAR szRegCmd[MAX_PATH];
	_stprintf(szRegCmd, _T("%s\\program\\isserver3.exe -unreg"), g_szPath);
	ShellExecute(NULL, _T("open"), szRegCmd, NULL, NULL, 0);

	SMessageBox(GetActiveWindow(), _T("卸载成功！"), szMsgCaption, MB_OK | MB_ICONINFORMATION);

	return TRUE;
}


BOOL MyCopyFile(LPCTSTR pszSour, LPCTSTR pszDest)
{
	TCHAR szSour[512] = { 0 }, szDest[512] = { 0 };
	_tcscpy(szSour, pszSour);
	_tcscpy(szDest, pszDest);

	SHFILEOPSTRUCT fileOp = { 0 };
	fileOp.wFunc = FO_COPY;
	fileOp.pFrom = szSour;
	fileOp.pTo = szDest;
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_FILESONLY;
	int nRet = SHFileOperation(&fileOp);
	return nRet == 0;
}

BOOL Sinstar_Install()
{
	TCHAR szSysPath[MAX_PATH];
	TCHAR szSysWow64[MAX_PATH] = { 0 };

	GetSystemDirectory(szSysPath, MAX_PATH);
	GetSysWow64Dir(szSysWow64, MAX_PATH);

	TCHAR szMsgCaption[]=_T("启程输入法-安装");

	//step1: check for running.
	HANDLE hMutex = CreateMutex(NULL, FALSE, SINSTAR3_MUTEX);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		if (SMessageBox(GetActiveWindow(), _T("输入法正在使用，不能安装，查看哪些程序在使用吗？"), szMsgCaption, MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			ShowCaller();
		}
		return FALSE;
	}
	CloseHandle(hMutex);

	//step2 copy files.
	if (!Sinstar_Update(TRUE))
	{
		return FALSE;
	}

	TCHAR szSvrExe[256] = { 0 }, szSvrData[256] = { 0 };
	_stprintf(szSvrExe, _T("%s\\program\\isserver3.exe"), g_szPath);
	_stprintf(szSvrData, _T("%s\\server"), g_szPath);

	//step3: write reg
	CRegKey reg;
	LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), KEY_WRITE | KEY_WOW64_64KEY);
	if (ret != ERROR_SUCCESS)
	{
		ret = reg.Create(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), 0, 0, KEY_WRITE | KEY_WOW64_64KEY);
		if (ret != ERROR_SUCCESS)
		{
			SMessageBox(GetActiveWindow(), _T("打开注册表失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
			return 0;
		}
	}
	{
		reg.SetStringValue(_T("path_client"), g_szPath);
		reg.SetStringValue(_T("path_svr"), szSvrExe);
		reg.SetStringValue(_T("path_svr_data"), szSvrData);
		reg.Close();
	}


	TCHAR szPath[1000];
	//step6:安装输入法
	if (Is64OS())
	{
		for (int i = 0; i<ARRAYSIZE(KSrcX86Files); i++)
		{
			if (!KSrcX86Files[i].bReg) continue;
			_stprintf(szPath, _T("%s\\%s"), szSysWow64, KSrcX86Files[i].pszDest);
			if (0 != CallRegsvr32(szPath, TRUE))
			{
				SMessageBox(GetActiveWindow(), _T("注册失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}

		for (int i = 0; i<ARRAYSIZE(KSrcX64Files); i++)
		{
			if (!KSrcX64Files[i].bReg) continue;
			_stprintf(szPath, _T("%s\\%s"), szSysPath, KSrcX64Files[i].pszDest);
			if (0 != CallRegsvr32(szPath, TRUE))
			{
				SMessageBox(GetActiveWindow(), _T("注册失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
	}
	else
	{
		for (int i = 0; i<ARRAYSIZE(KSrcX86Files); i++)
		{
			_stprintf(szPath, _T("%s\\%s"), szSysPath, KSrcX86Files[i].pszDest);
			if (0 != CallRegsvr32(szPath, TRUE))
			{
				SMessageBox(GetActiveWindow(), _T("注册失败"), szMsgCaption, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	//step8:reg ime file type.
	TCHAR szRegCmd[MAX_PATH];
	_stprintf(szRegCmd, _T("%s -reg"), szSvrExe);
	ShellExecute(NULL, _T("open"), szRegCmd, NULL, NULL, 0);

	SMessageBox(GetActiveWindow(), _T("安装成功！"), szMsgCaption, MB_OK | MB_ICONINFORMATION);

	return TRUE;
}


CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	_tcscpy(g_szPath, SApplication::getSingleton().GetAppDir());
	for (int i = 0; i<ARRAYSIZE(KFiles); i++)
	{
		TCHAR szBuf[1000];
		_stprintf(szBuf, _T("%s\\%s"), g_szPath, KFiles[i]);
		if (GetFileAttributes(szBuf) == INVALID_FILE_ATTRIBUTES)
		{
			_stprintf(szBuf, _T("安装目录文件[%s]没有找到，请保证安装包完整！"), KFiles[i]);
			SMessageBox(m_hWnd, szBuf, _T("错误"), MB_OK | MB_ICONSTOP);
			PostQuitMessage(1);
			return 0;
		}
	}

	SRichEdit *pEdit = FindChildByID2<SRichEdit>(R.id.edit_license);
	pEdit->LoadRtf(SApplication::getSingleton().GetAppDir()+_T("\\license.rtf"));
	FindChildByID(R.id.wnd_btn_container)->EnableWindow(FALSE, TRUE);

	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	GetNative()->DestroyWindow();
}

void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnAgree()
{
	BOOL bAgree = FindChildByID(R.id.chk_agree)->IsChecked();
	FindChildByID(R.id.wnd_btn_container)->EnableWindow(bAgree, TRUE);
}

void CMainDlg::OnInstall()
{
	Sinstar_Install();
	OnClose();
}

void CMainDlg::OnUninstall()
{
	Sinstar_Uninstall();
	OnClose();
}

void CMainDlg::OnUpdate()
{
	TCHAR szMsgCaption[]=_T("启程输入法-更新");
	
	CRegKey reg;
	LONG ret = reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\SetoutSoft\\sinstar3"), KEY_READ | KEY_WOW64_64KEY);
	if (ret != ERROR_SUCCESS)
	{

		UINT nRet = SMessageBox(GetActiveWindow(), _T("您还没有安装启程输入法3.0+,现在安装吗？"), szMsgCaption, MB_OKCANCEL | MB_ICONQUESTION);
		if(nRet == IDOK)
			OnInstall();
		return;
	}

	//step1: check for running.
	HANDLE hMutex = CreateMutex(NULL, FALSE, SINSTAR3_MUTEX);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		if (SMessageBox(GetActiveWindow(), _T("输入法正在使用，不能安装，查看哪些程序在使用吗？"), szMsgCaption, MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			ShowCaller();
		}
		return;
	}
	CloseHandle(hMutex);

	//退出服务器
	HWND hWndSvr = FindWindow(NULL, SINSTAR3_SERVER_HWND);
	if (::IsWindow(hWndSvr))
	{
		DWORD dwProcID = 0;
		GetWindowThreadProcessId(hWndSvr, &dwProcID);
		::PostMessage(hWndSvr, WM_QUIT, 0, 0);
		if (dwProcID != 0)
			WaitForSingleObject((HANDLE)dwProcID, INFINITE);
		else
			Sleep(500);
	}


	TCHAR szPath[MAX_PATH];
	ULONG nLen = MAX_PATH;
	reg.QueryStringValue(_T("path_client"),szPath,&nLen);
	reg.Close();

	if(_tcsicmp(szPath,g_szPath)!=0)
	{
		SStringT oldSvrPath = SStringT().Format(_T("%s\\program\\isserver3.exe"),szPath);

		WORD oldVer[4];
		SDpiHelper::PEVersion(oldSvrPath,oldVer[0],oldVer[1],oldVer[2],oldVer[3]);

		SStringT newSvrPath = SStringT().Format(_T("%s\\program\\isserver3.exe"),g_szPath);
		WORD newVer[4];
		SDpiHelper::PEVersion(newSvrPath,newVer[0],newVer[1],newVer[2],newVer[3]);

		if(memcmp(newVer,oldVer,8)<=0)
		{
			SStringT strMsg = SStringT().Format(_T("您已经安装了更新版本，无需升级！\n当前版本:%d.%d.%d.%d\n已安装版本:%d.%d.%d.%d"),
				newVer[0],newVer[1],newVer[2],newVer[3],
				oldVer[0],oldVer[1],oldVer[2],oldVer[3]);
			SMessageBox(m_hWnd,strMsg,szMsgCaption,MB_OK|MB_ICONSTOP);
			OnClose();
			return;
		}
		TCHAR szOldPath[MAX_PATH]={0};
		TCHAR szNewPath[MAX_PATH]={0};
		for (int i = 0; i < ARRAYSIZE(KUpdateInfo); i++)
		{
			_stprintf(szOldPath, _T("%s\\%s\0\0"), szPath, KUpdateInfo[i].pszFile);
			_stprintf(szNewPath, _T("%s\\%s\0\0"), g_szPath, KUpdateInfo[i].pszFile);
			if(KUpdateInfo[i].bClean)
			{
				//delete old program dir
				SHFILEOPSTRUCT fileOp = { 0 };
				fileOp.wFunc = FO_DELETE;
				fileOp.pFrom = szOldPath;
				fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;
				int nRet = SHFileOperation(&fileOp);
			}
			{
				//copy file to old path.
				SHFILEOPSTRUCT fileOp = { 0 };
				fileOp.wFunc = FO_COPY;
				fileOp.pFrom = szNewPath;
				fileOp.pTo = szOldPath;
				fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
				int nRet = SHFileOperation(&fileOp);
			}
		}
	}

	Sinstar_Update();

	//step8:reg ime file type.
	SStringT strSvrCmd = SStringT().Format(_T("%s\\program\\isserver3.exe -reg"),szPath);
	ShellExecute(NULL, _T("open"), strSvrCmd, NULL, NULL, 0);

	OnClose();
}

void CMainDlg::OnDonate()
{
	SHostDialog dlgDonate(UIRES.LAYOUT.dlg_donate);
	dlgDonate.DoModal();
}

