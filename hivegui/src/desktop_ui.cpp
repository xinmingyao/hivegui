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
		//��ȥ���߿�
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);break;
		//ȥ����ʾ�ı�����
		case WM_SIZE:          lRes=OnSize(uMsg,wParam,lParam,bHandled);break;
		//�����ס������Ҽ�ʱ����ֱ߿������
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		//ʵ�ְ�ס���ڣ�����ť�⣩�϶�����
		case WM_NCHITTEST:     lRes=OnNcHitTest(uMsg,wParam,lParam,bHandled);break;
		//��ȡ�߿������Сֵ
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