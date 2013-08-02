#include "hive_gui.h"
#include "login_ui.h"
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
				MessageBox(NULL,m_pNameEdit->GetText()+_T("\n")+m_pPasswdEdit->GetText(),_T("ע��"),MB_OK);
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
				//MessageBox(NULL,m_pNameEdit->GetText()+_T("\n")+m_pPasswdEdit->GetText(),_T("��¼"),MB_OK);
				return;
			}
		}
	}
LRESULT LoginUI::loop(UINT uMsg, WPARAM wParam, LPARAM lParam) // handle message from hive cell
    {
        MessageBox(NULL,"LOGIN MSG",_T("��¼"),MB_OK);
        return 0;
    }
LRESULT LoginUI::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
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
		case WM_CELL:  lRes = loop(uMsg,wParam,lParam);break;
		default:               bHandled=FALSE;
		}
		if(bHandled)
			return lRes;
		if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
			return lRes;
		return CWindowWnd::HandleMessage(uMsg,wParam,lParam);
	}