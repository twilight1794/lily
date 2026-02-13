#!/bin/sh
# Instala Lua, y las bibliotecas de desarrollo para Linux y web
set -eu

wget https://www.lua.org/ftp/lua-5.4.8.tar.gz -q -O - | tar -xz
cd lua-5.4.8
patch -p1 src/Makefile <<EOF
10c10
< CFLAGS= -O2 -Wall -Wextra -DLUA_COMPAT_5_3 \$(SYSCFLAGS) \$(MYCFLAGS)
---
> CFLAGS= -O2 -Wall -Wextra -fPIC \$(SYSCFLAGS) \$(MYCFLAGS)
35a36
> LUA_SO= liblua.so
47c48
< ALL_T= \$(LUA_A) \$(LUA_T) \$(LUAC_T)
---
> ALL_T= \$(LUA_A) \$(LUA_T) \$(LUAC_T) \$(LUA_SO)
62a64,66
> \$(LUA_SO): \$(CORE_O) \$(LIB_O)
> 	\$(CC) -shared -ldl -Wl,-soname,\$(LUA_SO).\$(V) -o \$@.\$(R) \$? -lm \$(MYLDFLAGS)
> 
144a149,150
> wasm:
> 	\$(MAKE) generic CC='emcc -s WASM=1'
EOF
# FIX: esto generará para wasm un .so que no usaremos, hay qué encontrar la forma de evitarlo
patch -p1 Makefile <<EOF
37a38,41
> # Lua version and release.
> V= 5.4
> R= \$V.8
> 
39c43
< PLATS= guess aix bsd c89 freebsd generic ios linux linux-readline macosx mingw posix solaris
---
> PLATS= guess aix bsd c89 freebsd generic ios linux linux-readline macosx mingw posix solaris wasm
44c48
< TO_LIB= liblua.a
---
> TO_LIB= liblua.a liblua.so.\$(R)
47,50d50
< # Lua version and release.
< V= 5.4
< R= \$V.8
< 
55c55
< 	@cd src && \$(MAKE) \$@
---
> 	@cd src && \$(MAKE) \$@ V=\$(V) R=\$(R)
62a63,64
> 	ln -sf \$(INSTALL_LIB)/liblua.so.\$(R) \$(INSTALL_LIB)/liblua.so.\$(V)
> 	ln -sf \$(INSTALL_LIB)/liblua.so.\$(R) \$(INSTALL_LIB)/liblua.so
69a72,77
> install_wasm:
> 	cd src && \$(INSTALL_DATA) liblua.a \$(INSTALL_LIB)/libluawasm.a
> 
> uninstall_wasm:
> 	cd src && cd \$(INSTALL_LIB) && \$(RM) libluawasm.a
> 
104c112
< .PHONY: all \$(PLATS) help test clean install uninstall local dummy echo pc
---
> .PHONY: all \$(PLATS) help test clean install uninstall install_wasm uninstall_wasm local dummy echo pc
EOF

# Generar versión principal
make clean
make linux
make install

# Generar versión wasm
make clean
make wasm
make install_wasm

# Borrar
cd ..
pwd
rm -rf lua-5.4.8
