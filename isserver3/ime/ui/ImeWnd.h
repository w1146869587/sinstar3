#pragma once
#include "skinawarewnd.h"

namespace SOUI
{

class CImeWnd : public CSkinAwareWnd
{
public:
	CImeWnd(SEventSet *pEvtSets,LPCTSTR pszLayout);
	virtual ~CImeWnd();

	HWND Create(HWND hParent=NULL);
	void Show(BOOL bShow);
	void SetOwner(HWND hOwner);
protected:
	virtual int OnRecreateUI(LPCREATESTRUCT lpCreateStruct);

	virtual SWND OnSetSwndCapture(SWND swnd);
	virtual BOOL OnReleaseSwndCapture();

	BOOL m_canReleaseCapture;
	HWND	m_hOwner;
protected:
	virtual void OnReposition(CPoint pt) {}
	void OnDragStatus(EventArgs *e);
	EVENT_MAP_BEGIN()
		EVENT_HANDLER(EventDragMove::EventID, OnDragStatus)
	EVENT_MAP_END()

protected:

	BOOL OnSetCursor(HWND wnd, UINT nHitTest, UINT message);
	void OnMouseMove(UINT nFlags, CPoint point);

	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BEGIN_MSG_MAP_EX(CImeWnd)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseEvent)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		CHAIN_MSG_MAP(CSkinAwareWnd)
	END_MSG_MAP()
};

}
