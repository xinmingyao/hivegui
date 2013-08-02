
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <assert.h> 

#include "../lib/UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "./lib/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "./lib/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "./lib/DuiLib_u.lib")
#   else
#       pragma comment(lib, "./lib/DuiLib.lib")
#   endif
#endif


#include "../lib/msgpack.hpp"
extern "C" { 
	#include "../lib/msgpack.h"
	#include "../lib/lua.h" 
	#include "../lib/lauxlib.h" 
	#include "../lib/lualib.h" 
	#include "../lib/hive.h" 
}

#pragma comment(lib,"./lib/lua52.lib")
#pragma comment(lib,"./lib/msgpackd.lib")
#pragma comment(lib,"./lib/hivecore.lib")

#include  <winsock2.h> 
#pragma comment(lib, "WS2_32")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
