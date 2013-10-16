#include "stdafx.h"
#define CELL_LOGIN "cell_login"
#define WM_CELL 999
int send_to_hive(char * name,char * data,int data_len);
int registerto_hive(char * name,int size,HWND handle);
extern "C" { 
int hive_gui_lib(lua_State *L) ;
}