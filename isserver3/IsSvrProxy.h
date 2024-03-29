#pragma once
#include "ui/trayIcon.h"
#include "ui/KeyMapDlg.h"
#include "ui/BuildIndexProgWnd.h"
#include "ime/TextServiceProxy.h"
#include "settings/ConfigDlg.h"
#include <iscore-i.h>

typedef IServerCore * (*funIscore_Create)();
typedef void(*funIscore_Destroy)(IServerCore* pCore);

#define UM_BUILD_INDEX_PROG0	(WM_USER+2000)
#define UM_BUILD_INDEX_PROG1	(WM_USER+2001)
#define UM_BUILD_INDEX_PROG2	(WM_USER+2002)
#define UM_IMPORT_USER_LIB		(WM_USER+2003)


enum{
	CD_CMD_NULL = 0,
	CD_CMD_INSTALL_CIT=100,
	CD_CMD_INSTALL_PLT,
	CD_CMD_INSTALL_SKIN,
};

class CIsSvrProxy : public SNativeWnd
	, public IUiMsgHandler
	, public IKeyMapListener
	, public IUpdateIntervalObserver
	, public SOUI::IIpcSvrCallback
	, public TAutoEventMapReg<CIsSvrProxy>
	, public IConntionFocusListener
{
	friend class CWorker;
public:
	CIsSvrProxy(const SStringT &strDataPath,const SStringT & strSvrPath);
	~CIsSvrProxy();

protected:
	virtual void OnNewConnection(IIpcHandle * pIpcHandle, IIpcConnection ** ppConn)
	{
		*ppConn = new CSvrConnection(pIpcHandle,m_hWnd,this);
	}

	virtual int GetBufSize() const {
		return 10240;
	}
	virtual void * GetSecurityAttr() const;
	virtual void ReleaseSecurityAttr(void* psa) const;
	virtual void OnConnected(IIpcConnection * pConn);
	virtual void OnDisconnected(IIpcConnection * pConn);

protected:
	virtual void OnSetFocus(CSvrConnection * pConn);
	virtual void OnKillFocus(CSvrConnection * pConn);

protected:
	virtual void OnKeyMapFree(CKeyMapDlg *pWnd);

protected:
	virtual int GetUpdateInterval() const;
	virtual void OnUpdateIntervalChanged(int nInterval);
	virtual void OnUpdateNow();
protected:
	virtual void OnBuildShapePhraseIndex(PROGTYPE uType, unsigned int dwData);
	virtual void OnBuildSpellPhraseIndex(PROGTYPE uType, unsigned int dwData) ;
	virtual void OnBuildSpellPhraseIndex2(PROGTYPE uType, unsigned int dwData) ;
	virtual void OnImportUserDict(PROGTYPE uType, unsigned int dwData);

	virtual void OnClientActive() ;
	virtual void OnClientLogin() ;
	virtual void OnClientLogout() ;

	virtual void OnShowTray(bool bTray) ;

	virtual void OnShowKeyMap(IDataBlock * pCompData, LPCSTR pszName, LPCSTR pszUrl) ;

	virtual int TtsGetSpeed();
	virtual int TtsGetVoice(bool bCh);
	virtual void TtsSetSpeed(int nSpeed) ;
	virtual void TtsSpeakText(const wchar_t* pText, int nLen, bool bChinese) ;
	virtual void TtsSetVoice(bool bCh, int iToken) ;
	virtual int TtsGetTokensInfo(bool bCh, wchar_t token[][MAX_TOKEN_NAME_LENGHT], int nBufSize);

	virtual DWORD OnQueryVersion() const;

protected:
	void OnCheckUpdateResult(EventArgs *e);
	EVENT_MAP_BEGIN()
		EVENT_HANDLER(EventCheckUpdateResult::EventID,OnCheckUpdateResult)
	EVENT_MAP_BREAK()

protected:
	void _OnBuildIndexProg(int indexMode, PROGTYPE uType, unsigned int dwData);
	int OnCreate(LPCREATESTRUCT pCS);
	void OnDestroy();
	LRESULT OnTrayNotify(UINT uMsg, WPARAM wp, LPARAM lp);
	LRESULT OnTaskbarCreated(UINT uMsg, WPARAM wp, LPARAM lp);
	LRESULT OnBuildIndexProg(UINT uMsg, WPARAM wp, LPARAM lp);
	LRESULT OnCopyData(HWND hWnd,PCOPYDATASTRUCT lpCopyData);
	void OnTimer(UINT_PTR uID);

	void OnMenuExit(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnMenuAutoExit(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnMenuSettings(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnMenuAutoRun(UINT uNotifyCode, int nID, HWND wndCtl);

	void CheckUpdate(bool bManual);
	
	INT_PTR ShowModal(SHostDialog *pDlg);
	
	void Quit(int nCode);
	bool IsAutoRun() const;
	bool SetAutoRun(bool bAutoRun) const;

	void OnEndSession(BOOL bEnding, UINT uLogOff);
	BEGIN_MSG_MAP_EX(CIsSvrProxy)
		CHAIN_MSG_MAP_2_IPC(m_ipcSvr)
		if (m_pCore) CHAIN_MSG_MAP_MEMBER(*m_pCore)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_COPYDATA(OnCopyData)
		MESSAGE_HANDLER_EX(m_uMsgTaskbarCreated,OnTaskbarCreated)
		MESSAGE_RANGE_HANDLER_EX(UM_BUILD_INDEX_PROG0, UM_IMPORT_USER_LIB,OnBuildIndexProg)
		COMMAND_ID_HANDLER_EX(R.id.menu_force_exit, OnMenuExit)
		COMMAND_ID_HANDLER_EX(R.id.menu_auto_exit, OnMenuAutoExit)
		COMMAND_ID_HANDLER_EX(R.id.menu_settings, OnMenuSettings)
		COMMAND_ID_HANDLER_EX(R.id.menu_auto_run, OnMenuAutoRun)
		MESSAGE_HANDLER_EX(UM_TRAYNOTIFY, OnTrayNotify)
		CHAIN_MSG_MAP_MEMBER(m_trayIcon)
		MSG_WM_ENDSESSION(OnEndSession)
		CHAIN_MSG_MAP(SNativeWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()


private:
	int			m_nUpdateInterval;

private:
	IServerCore * m_pCore;
	HMODULE		  m_hCoreModule;
	funIscore_Create m_funIsCore_Create;
	funIscore_Destroy m_funIsCore_Destroy;

	CAutoRefPtr<IIpcServer> m_ipcSvr;

	SStringT	m_strSvrPath;
	SStringT	m_strDataPath;

	CTrayIcon	m_trayIcon;
	UINT	    m_uMsgTaskbarCreated;

	CKeyMapDlg *  m_pKeyMapDlg;
	CBuildIndexProgWnd * m_pBuildIndexProg;

	SHostDialog	* m_pCurModalDlg;

	PCOPYDATASTRUCT m_pPendingCmd;

	CSvrConnection * m_pFocusConn;
};

