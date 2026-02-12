#include "lua_cpu.h"

enum lily_error lily_lua_cpu_cargar(char* codigo){
    // Cargar definición arquitectura a utilizar
    // FIX: por ahora, solo archivos de usuario
    lua_State* L = luaL_newstate();
    luaopen_base(L);
    luaopen_package(L);
    luaopen_string(L);
    luaopen_utf8(L);
    luaopen_table(L);
    luaopen_math(L);

    if (luaL_dostring(L, codigo) == LUA_OK) {
        printf("establa: %d\n", lua_istable(L, -1));
        printf("esnumero: %d\n", lua_isnumber(L, -1));
        if (lua_istable(L, -1)){
            lua_gettable(L, -1);
        }
        if (lua_isnumber(L, -1)){
            int res = lua_tointeger(L, -1);
            printf("el numero es %d\n", res);
        }
        lua_pop(L, lua_gettop(L));
    } else {
        puts(lua_tostring(L, lua_gettop(L)));
        lua_pop(L, lua_gettop(L));
    }

    lua_close(L);
}
