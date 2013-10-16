#include "hive_gui.h"
#include "login_ui.h"
#include "desktop_ui.h"
#include "msgpack_protocol.h"
void LoginUI::Notify(TNotifyUI& msg)
	{
		if(msg.sType==_T("windowinit"))
			OnPrepare();
		else if(msg.sType==_T("click"))
		{
			if(msg.pSender==m_pCloseBtn)
			{
				PostQuitMessage(0);
				return;
			}else if(msg.pSender==m_pRegisterBtn)
			{
				MessageBox(NULL,m_pNameEdit->GetText()+_T("\n")+m_pPasswdEdit->GetText(),_T("注册"),MB_OK);
				return;
			}else if(msg.pSender==m_pLoginBtn)
			{
				//m_pLoginBtn->SetEnabled(false);
				msgpack_sbuffer  sbuf ;
				msgpack_sbuffer_init(&sbuf);
				msgpack::sbuffer buffer ;  // simple buffer
				
				User user ;
				user.name = this->m_pNameEdit->GetText();
				user.pwd =  this->m_pPasswdEdit->GetText();
				user.r.size = 5;
				user.r.ptr = "12345";
				user.from = (int)m_hWnd;
				user.tag = (int)GetHWND();

				msgpack::pack(&buffer, user);
				char * ptr = (char *)malloc(buffer.size());
				memcpy(ptr,buffer.data(),buffer.size());
				send_to_hive(CELL_LOGIN,ptr,buffer.size());
				//::PostMessage((HWND)user.from ,999,0,999);
				//MessageBox(NULL,m_pNameEdit->GetText()+_T("\n")+m_pPasswdEdit->GetText(),_T("登录"),MB_OK);
				return;
			}
		}
	}
LRESULT LoginUI::loop(UINT uMsg, WPARAM wParam, LPARAM lParam) // handle message from hive cell
    {
		struct message_buf * buf =(struct message_buf *)lParam;
		char * data = buf->b;
		msgpack::zone mempool;
		msgpack::object deserialized;
		msgpack::unpack(buf->b, buf->len, NULL, &mempool, &deserialized);
		User user;
		deserialized.convert(&user);
		hive_free((char *)buf->b);
		hive_free((char *)buf);
		MessageBox(NULL,"LOGIN MSG",_T("登录"),MB_OK);
		DesktopUI * desktop = new DesktopUI();
		if( desktop == NULL ) return 0;
		desktop->Create(NULL, _T("share desktop................"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		desktop->CenterWindow();
		desktop->ShowWindow(true);
        return 0;
    }
LRESULT LoginUI::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
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
