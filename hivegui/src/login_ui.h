#if !defined(LOGIN_UI__)
#define LOGIN_UI__
#include "stdafx.h"
#include <io.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>
class LoginUI:public CWindowWnd,public INotifyUI
{
public:
	LoginUI(){};
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); }
	void OnPrepare() 
	{
		
	}
	void Notify(TNotifyUI& msg);
	LRESULT loop(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Init()
	{
		 m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		 m_pRegisterBtn=static_cast<CButtonUI*>(m_pm.FindControl(_T("registerbtn")));
		 m_pLoginBtn=static_cast<CButtonUI*>(m_pm.FindControl(_T("loginbtn")));
		 m_pNameEdit=static_cast<CEditUI*>(m_pm.FindControl(_T("nameedit")));
		 m_pPasswdEdit=static_cast<CEditUI*>(m_pm.FindControl(_T("passwdedit")));
	}

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_pm.Init(m_hWnd);
        //m_pm.SetTransparent(100);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("login.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
		registerto_hive("login",4,m_hWnd);
        return 0;
    }
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();//这个是圆角，是椭圆的半径
		//这里有个很巧妙的用法，在xml文件中一定要设置圆角（roundcorner），
		//不然不会进入下面的if，也就会出现标题栏。
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
            HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
            ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
            ::SetWindowRgn(*this, hRgn1, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
        }

        bHandled = FALSE;
        return 0;
    }
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-rcWork.left, -rcWork.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;
        lpMMI->ptMaxSize.x		= rcWork.right;
        lpMMI->ptMaxSize.y		= rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt;
		pt.x=GET_X_LPARAM(lParam);
		pt.y=GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);
		CControlUI* pControl=static_cast<CControlUI*>(m_pm.FindControl(pt));
		if(pControl && _tcscmp(pControl->GetClass(),_T("ButtonUI"))!=0 && _tcscmp(pControl->GetClass(),_T("EditUI"))!=0)
			return HTCAPTION;
		return HTCLIENT;
	}
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
public:
	CPaintManagerUI m_pm;
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pRegisterBtn;
	CButtonUI* m_pLoginBtn;
	CEditUI* m_pNameEdit;
	CEditUI* m_pPasswdEdit;
};


#endif