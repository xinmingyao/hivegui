#include "hive_gui.h"
#include "desktop_ui.h"
#include "msgpack_protocol.h"
void DesktopUI::Notify(TNotifyUI& msg)
	{
		if(msg.sType==_T("windowinit"))
			OnPrepare();
		else if(msg.sType==_T("click"))
		{
			
		}
	}
LRESULT DesktopUI::loop(UINT uMsg, WPARAM wParam, LPARAM lParam) // handle message from hive cell
    {

		MessageBox(NULL,"LOGIN MSG",_T("DESKTOP"),MB_OK);
        return 0;
    }
LRESULT DesktopUI::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRes=0;
		BOOL bHandled=TRUE;
		switch(uMsg)
		{
		case WM_CREATE:        lRes=OnCreate(uMsg,wParam,lParam,bHandled);break;
		//能去除边框
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);break;
		//去除显示的标题栏
		case WM_SIZE:          lRes=OnSize(uMsg,wParam,lParam,bHandled);break;
		//解决按住左键或右键时会出现边框的问题
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		//实现按住窗口（除按钮外）拖动窗口
		case WM_NCHITTEST:     lRes=OnNcHitTest(uMsg,wParam,lParam,bHandled);break;
		//获取边框最大最小值
		case WM_GETMINMAXINFO: lRes=OnGetMinMaxInfo(uMsg,wParam,lParam,bHandled);break;
		case WM_DESTROY:       ::PostQuitMessage(0);break;
		case WM_HIVE_CELL:  lRes = loop(uMsg,wParam,lParam);break;
		default:               bHandled=FALSE;
		}
		if(bHandled)
			return lRes;
		if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
			return lRes;
		return CWindowWnd::HandleMessage(uMsg,wParam,lParam);
	}
