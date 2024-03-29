// MyTTS.h: interface for the CWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTTS_H__C7E6BDC8_CD08_4D07_8EA4_9F404B9E0C18__INCLUDED_)
#define AFX_MYTTS_H__C7E6BDC8_CD08_4D07_8EA4_9F404B9E0C18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sapi.h>           // SAPI includes
#include <iscore-i.h>
#include <core/SSingleton.h>
#include "threadobject.h"


enum {
	UM_TTS_FINISH = (WM_USER + 100),
	UM_FUN_SPEAK,
	UM_FUN_STOP,
	UM_FUN_SETVOICE,
	UM_FUN_SETSPEED,
	UM_FUN_GETTOKENINFO,
	UM_FUN_CHECK_UPDATE,
	UM_FUN_DATA_REPORT,
};

class CWorker : public SNativeWnd
	, protected CThreadObject
	, public SSingleton<CWorker>
{
public:
	CWorker(LPCTSTR pszDataPath);
	virtual ~CWorker();

	int GetSpeed();
	void SetSpeed(int nSpeed);
	void Stop();
	int GetTokensInfo(bool bCh, wchar_t token[][MAX_TOKEN_NAME_LENGHT], int nBufSize);
	void SetVoice(BOOL bCh,int nToken);
	int GetVoice(BOOL bCh);
	void SpeakWText(const WCHAR * pwcText,int nLen,BOOL bCh);
	void SpeakText(LPCSTR pszText,int nLen,BOOL bCh);
	void ReportUserInfo();
	void CheckUpdate(LPCSTR pszUri, bool bManual);
private:
	void _SetSpeed(WPARAM wp);
	void _SpeakText(WPARAM wp, LPARAM lp);
	void _Stop();
	BOOL _SetVoice(WPARAM wp, LPARAM lp);
	int  _GetTokensInfo(WPARAM wp, LPARAM lp);
protected:
	virtual UINT Run(LPARAM lp);
protected:
	LRESULT OnTTSMessage(UINT uMsg, WPARAM wp, LPARAM lp);
	LRESULT OnCheckUpdate(UINT uMsg, WPARAM wp, LPARAM lp);
	LRESULT OnDataReport(UINT uMsg, WPARAM wp, LPARAM lp);

	BEGIN_MSG_MAP_EX(CWorker)
		MESSAGE_RANGE_HANDLER_EX(UM_TTS_FINISH, UM_FUN_GETTOKENINFO,OnTTSMessage)
		MESSAGE_HANDLER_EX(UM_FUN_CHECK_UPDATE, OnCheckUpdate)
		MESSAGE_HANDLER_EX(UM_FUN_DATA_REPORT, OnDataReport)
		CHAIN_MSG_MAP(__super)
	END_MSG_MAP()
private:
	void SetMsgOwner(ULONGLONG ullEvent, HWND hWnd, UINT uMsg);
	BOOL Init();
	void Uninit();
	BOOL IsTTSBusy();
	BOOL IsInited() { return m_bInitOK; }

	SStringT		m_strConfigIni;
	SComPtr<ISpVoice>   m_cpVoiceEn;
	SComPtr<ISpVoice>   m_cpVoiceCh;

	SComPtr<IEnumSpObjectTokens> m_cpChTokens;	//中文语音
	SComPtr<IEnumSpObjectTokens> m_cpEnTokens;	//英文语音
	BOOL	m_bInitOK;
	enum {VOICE_NULL,VOICE_CH,VOICE_EN} m_CurVoice;	//当前正在使用的语音

	int		m_nSpeed;
	int		m_iChVoice, m_iEnVoice;
};

#endif // !defined(AFX_MYTTS_H__C7E6BDC8_CD08_4D07_8EA4_9F404B9E0C18__INCLUDED_)
