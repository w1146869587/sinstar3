#pragma once

#include "ImeWnd.h"
#include "SkinMananger.h"
#include "../inputContext.h"
#include "StatusWnd.h"

namespace SOUI
{
	interface IInputWndListener {
		virtual void OnInputDelayHide() = 0;
	};

	class CInputWnd : public CImeWnd
	{
	public:
		CInputWnd(SEventSet *pEvtSets, InputContext * pCtx,IInputWndListener *pListener);
		~CInputWnd(void);

		void SetFollowCaret(BOOL bFollowCaret);
		void SetStatusWnd(CStatusWnd * pWnd);
		void SetAnchorPosition(CPoint ptAnchor);

		void MoveTo(CPoint pt,int nCaretHeight);
		void Show(BOOL bShow, BOOL bClearLocateInfo=TRUE);
		void Hide(int nDelay);
		void UpdateUI();
		
		BOOL GoNextCandidatePage();
		BOOL GoPrevCandidatePage();
		short SelectCandidate(short iCand);
		void OnFlmInfo(PFLMINFO pFlmInfo);
	protected:
		virtual void OnSetSkin(EventArgs * e);
		virtual int OnRecreateUI(LPCREATESTRUCT lpCreateStruct);

		void UpdateAnchorPosition();
		int GetCandMax(SWindow *pWnd, LPCWSTR pszCandClass) const;

		int GetCandMax2(int nCands);
	protected:
		void OnBtnPrevPage();
		void OnBtnNextPage();
		void OnUpdateBtnTooltip(EventArgs *e);

		EVENT_MAP_BEGIN()
			EVENT_ID_COMMAND(R.id.btn_prevpage,OnBtnPrevPage)
			EVENT_ID_COMMAND(R.id.btn_nextpage,OnBtnNextPage)
			EVENT_HANDLER(EventSwndUpdateTooltip::EventID, OnUpdateBtnTooltip)
		EVENT_MAP_END()

	protected:
		int OnCreate(LPCREATESTRUCT lpCreateStruct);
		void OnTimer(UINT_PTR idEvent);
		
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnRButtonUp(UINT nFlags, CPoint pt);

		BEGIN_MSG_MAP_EX(CInputWnd)
			MSG_WM_RBUTTONUP(OnRButtonUp)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_TIMER(OnTimer)
			MSG_WM_CREATE(OnCreate)
			CHAIN_MSG_MAP(__super)
		END_MSG_MAP()

	private:
		CPoint			 m_ptCaret;
		int				 m_nCaretHeight;
		BOOL			 m_bLocated;
		BOOL			 m_bShow;

		CStatusWnd	    *m_pStateWnd;
		short			 m_cPageSize;
		InputContext	* m_pInputContext;
		IInputWndListener * m_pInputWndListener;

		CPoint			m_ptClick;
		BOOL			m_bDraging;

		BOOL			m_bFollowCaret;
		CPoint			m_ptAnchor;
	};

}
