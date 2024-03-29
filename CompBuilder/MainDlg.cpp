// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include "FileHelper.h"
#include "base64.h"
#include <shellapi.h>
#include "../include/compbuilder-i.h"
#include <fstream>

typedef ICompBuilder* (*FunICompBuilder_Create)();
typedef void(*FunICompBuilder_Destroy)(ICompBuilder* pCompBuilder);
typedef ICompReaderEx * (*FunICompReader_Create)();
typedef void(*FunICompReader_Destroy)(ICompReaderEx* pCompReader);

class CompBuilderLoader
{
public:
	CompBuilderLoader()
	{
		m_hMod = LoadLibrary(_T("..\\program\\iscore.dll"));
		m_funCreate = (FunICompBuilder_Create)GetProcAddress(m_hMod, "ICompBuilder_Create");
		m_funDestroy = (FunICompBuilder_Destroy)GetProcAddress(m_hMod, "ICompBuilder_Destroy");

		m_funCreateReader = (FunICompReader_Create)GetProcAddress(m_hMod, "ICompReader_Create");
		m_funDestroyReader = (FunICompReader_Destroy)GetProcAddress(m_hMod, "ICompReader_Destroy");

	}

	ICompBuilder * Create() {
		return m_funCreate();
	}

	void  Destroy(ICompBuilder * pCompBuilder) {
		return m_funDestroy(pCompBuilder);
	}

	ICompReaderEx *CreateReader()
	{
		return m_funCreateReader();
	}

	void DestroyReader(ICompReaderEx *pReader)
	{
		m_funDestroyReader(pReader);
	}

	~CompBuilderLoader()
	{
		if (m_hMod) FreeLibrary(m_hMod);
	}
private:
	FunICompBuilder_Create m_funCreate;
	FunICompBuilder_Destroy m_funDestroy;

	FunICompReader_Create  m_funCreateReader;
	FunICompReader_Destroy m_funDestroyReader;

	HMODULE m_hMod;
};


CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_pLoader = new CompBuilderLoader();
}

CMainDlg::~CMainDlg()
{
	delete m_pLoader;
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	return 0;
}

void CMainDlg::OnBroweComp()
{
	CFileDialogEx openDlg(TRUE, _T("txt"), 0, 6, _T("码表文件(*.txt)\0*.txt\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		FindChildByID(R.id.edit_comp)->SetWindowText(openDlg.m_szFileName);

		if (GetFileAttributes(openDlg.m_szFileName) != 0xffffffff)
		{
			int nSymbolFirst = GetPrivateProfileInt(_T("Description"), _T("SymbolFirst"), 1, openDlg.m_szFileName);
			FindChildByID(R.id.chk_symbol_first)->SetCheck(nSymbolFirst != 0);
			int nYinXingMa = GetPrivateProfileInt(_T("Description"), _T("YinXingMa"), 0, openDlg.m_szFileName);
			FindChildByID(R.id.chk_yinxingma)->SetCheck(nYinXingMa != 0);
			TCHAR szWebSite[200];
			GetPrivateProfileString(_T("Description"), _T("WebSite"), _T(""), szWebSite, 200, openDlg.m_szFileName);
			FindChildByID(R.id.edit_url)->SetWindowText(szWebSite);
			char *szIcon = (char*)malloc(32 * 1024);//max to 32k
			int nLen = GetPrivateProfileSectionA("Icon2", szIcon, 32 * 1024, S_CT2A(openDlg.m_szFileName));
			if (nLen)
			{
				SStringT strIcon = openDlg.m_szFileName;
				strIcon += _T(".img");

				std::string tmp;
				char *p = szIcon;
				char *pEnd = p + nLen;

				while (p<pEnd)
				{
					tmp += p;
					p += strlen(p) + 1;
				}

				std::string result;
				result.resize(nLen);
				size_t nUsed = 0;
				Base64::DecodeFromArray(&tmp[0], tmp.length(), Base64::DO_LAX, &result, &nUsed);

				FILE *f = _tfopen(strIcon, _T("wb"));
				fwrite(&result[0], 1, result.length(), f);
				fclose(f);
				FindChildByID(R.id.edit_comp_logo)->SetWindowText(strIcon);
				UpdateLogo();
			}
			free(szIcon);
		}
	}
}


void CMainDlg::OnBroweCit()
{
	CFileDialogEx openDlg(FALSE, _T("cit"), 0, 6, _T("启程码表(*.cit)\0*.cit\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		FindChildByID(R.id.edit_cit_save)->SetWindowText(openDlg.m_szFileName);
	}
}

void CMainDlg::OnBroweLogo()
{
	CFileDialogEx openDlg(TRUE, _T("png"), 0, 6, _T("码表logo(*.png)\0*.png\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		FindChildByID(R.id.edit_comp_logo)->SetWindowText(openDlg.m_szFileName);
		UpdateLogo();
	}
}

void CMainDlg::OnBroweKeymap()
{
	CFileDialogEx openDlg(TRUE, _T("png"), 0, 6, _T("字根图(*.png,*.jpg,*.gif)\0*.png;*.jpg;*.gif\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		FindChildByID(R.id.edit_keymap)->SetWindowText(openDlg.m_szFileName);
		IBitmap *pKeyMap = SResLoadFromFile::LoadImage(openDlg.m_szFileName);
		if (pKeyMap)
		{
			FindChildByID2<SImageWnd>(R.id.img_keymap)->SetImage(pKeyMap);
			pKeyMap->Release();
		}
	}
}

void CMainDlg::UpdateLogo()
{
	SStringT strIconFile = FindChildByID(R.id.edit_comp_logo)->GetWindowText();
	IBitmap *pLogo = SResLoadFromFile::LoadImage(strIconFile);
	if (pLogo)
	{
		FindChildByID2<SImageWnd>(R.id.img_logo)->SetImage(pLogo);
		pLogo->Release();
	}

}

enum TxtCP {
	TCP_ANSI,
	TCP_UTF16,
	TCP_UTF8,
};

int ReadLine(char * pBuf, int nLen, FILE *f, TxtCP tcp)
{
	if (tcp == TCP_UTF16)
	{
		wchar_t szBuf[500] = { 0 };
		if (!fgetws(szBuf, 500, f)) return 0;
		strcpy_s(pBuf,nLen, S_CW2A((wchar_t*)szBuf));
	}
	else if (tcp == TCP_UTF8)
	{
		char szTmp[500];
		if (!fgets(szTmp, 500, f)) return 0;
		strcpy_s(pBuf, nLen, S_CA2A(szTmp, CP_UTF8, CP_ACP));
	}
	else
	{
		if (!fgets(pBuf, nLen, f)) return 0;
	}
	int nLineLen = strlen(pBuf);
	if (nLineLen > 2)
	{
		if (pBuf[nLineLen - 2] == 0x0d && pBuf[nLineLen - 1] == 0x0a)
		{
			nLineLen -= 2;
		}
		else if (pBuf[nLineLen - 1] == 0x0a || pBuf[nLineLen - 1]==0x0d)
		{
			nLineLen -= 1;
		}
		pBuf[nLineLen] = 0;
	}
	return nLineLen;
}

void CMainDlg::OnBuildComp()
{
	COMPHEAD head = { 0 };
	SStringT strComp, strKeyMap, strLogo, strSave;

	strComp = FindChildByID(R.id.edit_comp)->GetWindowText();
	strKeyMap = FindChildByID(R.id.edit_keymap)->GetWindowText();
	strSave = FindChildByID(R.id.edit_cit_save)->GetWindowText();
	strLogo = FindChildByID(R.id.edit_comp_logo)->GetWindowText();

	SStringT strUrl = FindChildByID(R.id.edit_url)->GetWindowText();

	head.bSymbolFirst = FindChildByID(R.id.chk_symbol_first)->IsChecked();
	head.dwEncrypt = FindChildByID(R.id.chk_disable_export)->IsChecked();
	head.dwYinXingMa = FindChildByID(R.id.chk_yinxingma)->IsChecked();

	if (strComp.IsEmpty() || GetFileAttributes(strComp) == 0xFFFFFFFF)
	{
		SMessageBox(m_hWnd, _T("invalid comp file"), _T("error"), MB_OK | MB_ICONSTOP);
		return;
	}
	if (!strKeyMap.IsEmpty() && GetFileAttributes(strKeyMap) == 0xFFFFFFFF)
	{
		SMessageBox(m_hWnd, _T("invalid key map file"), _T("error"), MB_OK | MB_ICONSTOP);
		return;
	}

	wchar_t szWildChar[2];
	BYTE byNumRules;
	char szRules[RULE_MAX * 100];

	TCHAR buf[100];
	GetPrivateProfileString(_T("Description"),_T("Name"),L"",buf,50,strComp);
	strcpy_s(head.szName, 50, S_CT2A(buf));
	GetPrivateProfileString(_T("Description"),L"UsedCodes",L"",buf,50,strComp);
	strcpy_s(head.szCode, 50, S_CT2A(buf));
	GetPrivateProfileString(_T("Description"), L"WildChar", L"", szWildChar, 2, strComp);
	head.cWildChar = (char)szWildChar[0];
	head.cCodeMax = GetPrivateProfileInt(_T("Description"), L"MaxCodes", 0, strComp);
	head.dwAutoSelect = GetPrivateProfileInt(_T("Description"), L"AutoSelect", 0, strComp) ? 1 : 0;//唯一重码自动上屏属性
	head.dwPhraseCompPart = GetPrivateProfileInt(_T("Description"), L"PhraseCompPart", 0, strComp) ? 1 : 0;//词组可以不是全码标志

	byNumRules = GetPrivateProfileInt(_T("Description"), L"NumRules", 0, strComp);

	SStringA strCompA = S_CT2A(strComp);
	GetPrivateProfileSectionA("Rule", szRules, RULE_MAX * 100, strCompA);

	ICompBuilder * pCompBuilder = m_pLoader->Create();
	ICodingRule * pCodingRule = pCompBuilder->CreateCodingRule();

	if (!pCodingRule->ParseRules(head.cCodeMax, byNumRules, szRules))
	{
		pCompBuilder->DestroyCodingRule(pCodingRule);
		m_pLoader->Destroy(pCompBuilder);
		SMessageBox(m_hWnd, _T("parse rules failed"), _T("error"), MB_OK | MB_ICONSTOP);
		return;
	}

	char szBom[3];
	FILE *f = _tfopen(strComp, _T("rb"));
	fread(szBom, 1, 3, f);
	TxtCP tcp = TCP_ANSI;
	const BYTE bom_utf16[2] = { 0xff,0xfe };
	const BYTE bom_utf8[3] = { 0xEF,0xBB,0xBF };
	if (memcmp(szBom, bom_utf16, 2) == 0)
	{
		tcp = TCP_UTF16;
		fseek(f, 2, SEEK_SET);
	}
	else if (memcmp(szBom, bom_utf8, 3) == 0)
		tcp = TCP_UTF8;
	else
		fseek(f, 0, SEEK_SET);

	char szLine[1000];
	bool bText = false;
	for(;!bText;){
		if (!ReadLine(szLine, 1000, f,tcp)) break;
		bText = strnicmp(szLine, "[Text]", 6) == 0;
	}
	if (!bText)
	{
		fclose(f);
		SMessageBox(m_hWnd, _T("not [Text] section were found in the file"), _T("error"), MB_OK | MB_ICONSTOP);
		return;
	}
	//import compostion parts
	while (ReadLine(szLine,1000,f,tcp))
	{
		if (szLine[0] & 0x80)
		{
			char *pszWord = szLine;
			char *pszComp = szLine + 2;
			if (pszComp[0]>0)
			{//只支持单字
				char cCompLen = 0;
				char *pszCompTemp = pszComp;
				int i = 0;
				while (pszComp[i])
				{
					if (pszComp[i] == ' ')
					{
						pCompBuilder->AddWord(pszWord, pszCompTemp, cCompLen);
						pszCompTemp += cCompLen + 1;
						cCompLen = 0;
					}
					else
						cCompLen++;
					i++;
				}
				if (cCompLen != 0) pCompBuilder->AddWord(pszWord, pszCompTemp, cCompLen);
			}
		}
	}
	fclose(f);
	if (pCompBuilder->GetWords()<2000)
	{
		pCompBuilder->DestroyCodingRule(pCodingRule);
		m_pLoader->Destroy(pCompBuilder);
		SMessageBox(m_hWnd, _T("less then 2000 words were been composited in your table"), _T("error"), MB_OK | MB_ICONSTOP);
		return;
	}
	SStringA strSaveA = S_CT2A(strSave);	
	SStringA strLogoA = S_CT2A(strLogo);
	SStringA strKeyMapA = S_CT2A(strKeyMap);
	BOOL bSuccess = pCompBuilder->Make(strSaveA, head,
		pCodingRule,
		strLogoA.IsEmpty() ? NULL : (LPCSTR)strLogoA,
		0,
		strKeyMapA.IsEmpty() ? NULL : (LPCSTR)strKeyMapA,
		NULL);

	pCompBuilder->DestroyCodingRule(pCodingRule);
	m_pLoader->Destroy(pCompBuilder);

	SMessageBox(m_hWnd, _T("build cit suceed!"), _T("Good!"), MB_OK);
}

void CMainDlg::OnExportComp()
{
	CFileDialogEx openDlg(TRUE, _T("cit"), 0, 6, _T("启程码表(*.cit)\0*.cit\0All files (*.*)\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		ICompReaderEx *pReader = m_pLoader->CreateReader();
		bool bOk = false;
		SStringA strCit = S_CT2A(openDlg.m_szFileName);
		if (pReader->Load(strCit))
		{
			char szDrive[5], szDir[MAX_PATH];
			_splitpath(strCit, szDrive, szDir, NULL, NULL);
			char szSave[MAX_PATH * 2];
			sprintf(szSave, "%s%s%s.txt", szDrive, szDir, pReader->GetProps()->szName);
			if (pReader->Export(szSave))
			{
				ShellExecuteA(NULL, "open", szSave, NULL, NULL, SW_SHOWDEFAULT);
				bOk = true;
			}
		}
		m_pLoader->DestroyReader(pReader);
	}

}

//TODO:消息映射
void CMainDlg::OnClose()
{
	GetNative()->DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

