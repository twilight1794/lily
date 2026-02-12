#!/bin/sh
# Instala Lua, y las bibliotecas de desarrollo para Linux y web
set -eu

wget https://www.lua.org/ftp/lua-5.4.8.tar.gz -q -O - | tar -xz
cd lua-5.4.8
patch -p1 src/Makefile <<EOF
10c10
< CFLAGS= -O2 -Wall -Wextra -DLUA_COMPAT_5_3 \$(SYSCFLAGS) \$(MYCFLAGS)
---
> CFLAGS= -O2 -Wall -Wextra \$(SYSCFLAGS) \$(MYCFLAGS)
144a145,147
> 
> wasm:
> 	\$(MAKE) generic CC='emcc -s WASM=1'
EOF
patch -p1 Makefile <<EOF
39c39
< PLATS= guess aix bsd c89 freebsd generic ios linux linux-readline macosx mingw posix solaris
---
> PLATS= guess aix bsd c89 freebsd generic ios linux linux-readline macosx mingw posix solaris wasm
69a70,75
> install_wasm:
> 	cd src && \$(INSTALL_DATA) liblua.a \$(INSTALL_LIB)/libluawasm.a
> 
> uninstall_wasm:
> 	cd src && cd \$(INSTALL_LIB) && \$(RM) libluawasm.a
> 
104c110
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
