# -----BEGIN USER CONFIGURATION-----
TARGET := all

CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
LDLIBS := -lm
RM := rm -rf
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1
LUADIR := lib/lua-5.4.8
# -----END USER CONFIGURATION-----

TARGETS := linux windows lib-linux lib-windows web
.PHONY: all $(TARGETS) build install-windows install-linux remove-linux remove-windows doxy clean lua-linux lua-win lua-web

all: linux windows web
linux: dist/lily
windows: dist/lily.exe
lib-linux: build dist/liblily.so
lib-windows: build dist/liblily.dll
web: build dist/liblily.js

lua-linux:
	cd $(LUADIR)/src && make

lua-windows:
	cd $(LUADIR)/src && make mingw

lua-web:
	cd $(LUADIR)/src && make generic CC='emcc -s WASM=1'

src/common/cadena.o: src/common/cadena.c
src/common/dict.o: src/common/dict.c
src/common/lde.o: src/common/lde.c
src/common/log.o: src/common/log.c
src/cli/main.o: src/cli/main.c
src/lib/a_lexico.o: src/lib/a_lexico.c
src/web/main.o: src/web/main.c

dist/liblily.so: src/lib/a_lexico.o src/common/cadena.o src/common/dict.o src/common/lde.o src/common/log.o | lua-linux
	$(CC) -shared -fPIC $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/liblily.dll: src/lib/a_lexico.o src/common/cadena.o src/common/dict.o src/common/lde.o src/common/log.o | lua-windows
	$(CC) -shared $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/lily: src/cli/main.o lib-linux | lua-linux
	$(CC) $(LDFLAGS)  src/cli/main.o -L dist -llily $(LDLIBS) -o $@

dist/lily.exe: src/cli/main.o lib-windows | lua-windows
	(CC) $(LDFLAGS) src/cli/main.o -L dist -llily  $(LDLIBS) -o $@

dist/liblily.js: lua-web
	emcc -Ilib/lua-5.4.7/src main.c lua-5.4.0/src/liblua.a -s WASM=1 -O2 -o dist/liblily.js -s EXPORTED_FUNCTIONS="['_run_lua']" -s 'EXPORTED_RUNTIME_METHODS=["ccall", "cwrap"]' -s MODULARIZE=1 -s 'EXPORT_NAME="initWasmModule"'

build:
	mkdir -p dist

install-linux:
	cp dist/lily $(BINDIR)/lily
	cp dist/liblily.so $(LIBDIR)/liblily.so
	ldconfig

install-windows:
	mkdir $(BINDIR)
	cp dist/lily.exe $(BINDIR)/lily.exe
	cp dist/liblily.dll $(LIBDIR)/liblily.dll

remove-linux:
	$(RM) $(BINDIR)/lily
	$(RM) $(LIBDIR)/liblily.so

remove-windows:
	$(RM) $(BINDIR)

doxy:
	@mkdir -p dist
	doxygen Doxyfile

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete
