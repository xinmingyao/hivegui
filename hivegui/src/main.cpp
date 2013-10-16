#include "hive_gui.h"
#include "login_ui.h"
struct lua_State * hive_state; //main lua_state for start hive cell
int send_to_hive(char * name,char * data,int data_len){
	return send_to_cell(hive_state,name,data,data_len);
}
int registerto_hive(char * name,int size,HWND handle){
	return regist_handle(hive_state,name,size,handle);
}
static void *
start_hive(void *p)
{   assert(!hive_state);
	hive_state = luaL_newstate();
	luaL_openlibs(hive_state);
	luaopen_base(hive_state);
	//hive gui api to lua 
	lua_pushcfunction(hive_state, hive_gui_lib);
	lua_rawsetp(hive_state, LUA_REGISTRYINDEX, "hive_gui_lib");
	char * file = (char *)p;
	int err = luaL_dofile(hive_state, file);
	
	if (err) {
		const char * err_str =lua_tostring(hive_state,-1);
		lua_close(hive_state);
		exit(999);
		return NULL;
	} 
	return NULL;

}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("skin"));
    CStdString path = CPaintManagerUI::GetInstancePath()+_T("main.lua");
	LPCTSTR s =path.GetData();
	
	HANDLE pid =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_hive,(LPVOID)s, 0, NULL);

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;


    LoginUI* ui = new LoginUI();
    if( ui == NULL ) return 0;
    ui->Create(NULL, _T("login................"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    ui->CenterWindow();
    ui->ShowWindow(true);

    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
	 return 0;
}