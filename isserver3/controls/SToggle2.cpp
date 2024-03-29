#include "StdAfx.h"
#include "SToggle2.h"

namespace SOUI
{
	SToggle2::SToggle2(void)
	{
	}

	SToggle2::~SToggle2(void)
	{
	}

	void SOUI::SToggle2::OnPaint(IRenderTarget *pRT)
	{
		ISkinObj *pSkin = GetToggle()?m_pSkin2:m_pSkin1;
		int iState = 0;
		DWORD dwState = GetState();
		if (dwState & WndState_PushDown)
			iState = 2;
		else if (dwState & WndState_Hover)
			iState = 1;
		CRect rcWnd = GetClientRect();
		pSkin->DrawByIndex(pRT,rcWnd,iState);
	}

	CSize SToggle2::GetDesiredSize(int wid,int hei)
	{
		return m_pSkin1->GetSkinSize();
	}

}
