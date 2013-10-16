#include "hive_gui.h"
extern "C" { 

static int
lstart_driver(lua_State *L) {
	struct desktop_driver * driver = alloc_driver();
	int r = start_driver(driver);
	lua_pushinteger(L,r);
	if(r==0){
		lua_pushlightuserdata(L, driver);
		return 2;
	}else{
		return 1;
	}
	
}

static int
lstop_driver(lua_State *L) {
	struct desktop_driver * driver = (struct desktop_driver * )lua_touserdata(L, 1);
	stop_free_driver(driver);
	return 0;
}

int
hive_gui_lib(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"start_driver", lstart_driver},
		{"stop_driver", lstop_driver},
		{NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}


}