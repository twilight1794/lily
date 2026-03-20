# -----BEGIN USER CONFIGURATION-----
TARGET := all

CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
LDFLAGS := -Ldist
LDLIBS := -lm
RM := rm -rf
LN := ln -s
#LN := mklink /J
PYTHON := python
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1
CPUDIR := /usr/local/share/lily/cpu
# -----END USER CONFIGURATION-----

V_LILY_VERSION=$(shell git tag --points-at HEAD)
V_LILY_COMMIT=$(shell git rev-parse HEAD|cut -c 1-8)
V_LILY_MODIFICADO=$(shell if test $$(git status --porcelain --untracked-files=no | wc -l) -gt 0; then echo '-changed'; fi)
CFLAGS += -DLILY_VERSION=\"$(V_LILY_VERSION)\" -DLILY_COMMIT=\"$(V_LILY_COMMIT)\" -DLILY_MODIFICADO=\"$(V_LILY_MODIFICADO)\" -DCPUDIR=\"$(CPUDIR)\"

TARGETS := linux windows web
.PHONY: all dist $(TARGETS) linux-dbg windows-dbg web-dbg install-windows install-linux remove-linux remove-windows doxy clean test-windows test-linux

all: linux windows web
# Release
linux: CFLAGS += -Os
linux: dist/lily
windows: dist/lily.exe
web: CFLAGS += -Oz
web: LDFLAGS += -Oz -sASSERTIONS=0
web: dist/liblily.js
# Debug
linux-dbg: CFLAGS += -ggdb3
linux-dbg: dist/lily
windows-dbg: dist/lily.exe
web-dbg: CFLAGS += -gsource-map
web-dbg: LDFLAGS += -gsource-map -sASSERTIONS
web-dbg: dist/liblily.js

src/common/cadena.o: src/common/cadena.c src/common/cadena.h
src/common/dict.o: src/common/dict.c src/common/dict.h
src/common/lde.o: src/common/lde.c src/common/lde.h
src/common/log.o: src/common/log.c src/common/log.h
src/common/nums.o: src/common/nums.c src/common/nums.h
src/cli/main.o: src/cli/main.c
src/cli/mmap.o: src/cli/mmap.c src/cli/mmap.h
src/cli/mmap_windows.o: src/cli/mmap_windows.c src/cli/mmap_windows.h
src/lib/a_lexico.o: src/lib/a_lexico.c src/lib/a_lexico.h src/lib/a_lexico_ctipos.h src/lib/a_lexico_pow.h
src/lib/a_semantico.o: src/lib/a_semantico.c src/lib/a_semantico.h
src/lib/a_sintactico.o: src/lib/a_sintactico.c src/lib/a_sintactico.h
src/lib/lily.o: src/lib/lily.c src/lib/lily.h
src/lib/lua_cpu.o: src/lib/lua_cpu.c src/lib/lua_cpu.h
src/lib/lua_ensamble.o: src/lib/lua_ensamble.c src/lib/lua_ensamble.h
src/lib/lua_entorno.o: src/lib/lua_entorno.c src/lib/lua_entorno.h
src/lib/lua_int.o: src/lib/lua_int.c src/lib/lua_entorno.h
src/lib/simbolo.o: src/lib/simbolo.c src/lib/simbolo.h

DEPS_LIBLILY := src/common/cadena.o src/common/dict.o src/common/lde.o src/common/log.o src/common/nums.o src/lib/a_lexico.o src/lib/a_semantico.o src/lib/a_sintactico.o src/lib/lily.o src/lib/lua_cpu.o src/lib/lua_ensamble.o src/lib/lua_entorno.o src/lib/lua_int.o src/lib/simbolo.o

dist/liblily.so: LDFLAGS += -shared -fPIC
dist/liblily.so: LDLIBS += -llua
dist/liblily.so: $(DEPS_LIBLILY)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/liblily.dll: LDFLAGS += -shared
dist/liblily.dll: $(DEPS_LIBLILY)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/liblily.js: CC=emcc
dist/liblily.js: CFLAGS += -I/usr/local/include -sWASM=1 -sSTRICT
dist/liblily.js: LDFLAGS += -fPIC -I/usr/local/include -L/usr/local/lib -sSTRICT -sSAFE_HEAP=1 -sALLOW_TABLE_GROWTH=1 -sALLOW_MEMORY_GROWTH=1 -sMODULARIZE=1 -sPOLYFILL=0
dist/liblily.js: LDFLAGS += -s EXPORTED_FUNCTIONS="['_lily_lily_ensamble', '_malloc', '_free']" -s 'EXPORTED_RUNTIME_METHODS=["ccall", "setValue", "getValue", "stringToUTF8", "lengthBytesUTF8", "UTF8ToString", "addFunction", "removeFunction"]'
dist/liblily.js: LDLIBS += -lluawasm
dist/liblily.js: $(DEPS_LIBLILY)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

DEPS_LILY_LINUX := src/cli/main.o src/cli/mmap.o
dist/lily: LDLIBS += -llily
dist/lily: $(DEPS_LILY_LINUX) dist/liblily.so
	$(CC) $(LDFLAGS) $(DEPS_LILY_LINUX) $(LDLIBS) -o $@

DEPS_LILY_WIN := src/cli/main.o src/cli/mmap_windows.o
dist/lily.exe: LDLIBS += -llily
dist/lily.exe: $(DEPS_LILY_WIN) dist/liblily.dll
	$(CC) $(LDFLAGS) $(DEPS_LILY_WIN) $(LDLIBS) -o $@

dist:
	mkdir -p dist

install-linux:
	cp dist/lily $(BINDIR)/lily
	cp dist/liblily.so $(LIBDIR)/liblily.so
	mkdir -p $(CPUDIR)
	cp misc/cpu/* $(CPUDIR)/
	ldconfig

install-windows:
	mkdir $(BINDIR)
	cp dist/lily.exe $(BINDIR)/lily.exe
	cp dist/liblily.dll $(LIBDIR)/liblily.dll

remove-linux:
	$(RM) $(BINDIR)/lily
	$(RM) $(LIBDIR)/liblily.so
	$(RM) $(CPUDIR)

remove-windows:
	$(RM) $(BINDIR)

doxy:
	@mkdir -p dist
	doxygen Doxyfile

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete

lib/munit/munit.o: lib/munit/munit.c

test/common/dict.o: test/common/dict.c test/common/dict.h
test/main.o: test/main.c

dist/test: lib/munit/munit.o test/main.o test/common/dict.o dist/liblily.so
	$(CC) $(LDFLAGS) $^ -llily $(LDLIBS) -o $@

dist/test.exe: lib/munit/munit.o test/main.o test/common/dict.o dist/liblily.dll
	$(CC) $(LDFLAGS) $^ -llily $(LDLIBS) -o $@

test-linux: dist/test
	LD_LIBRARY_PATH=dist dist/test
test-windows: dist/test.exe
	dist/test.exe
test-web:
	$(RM) dist/index.html
	$(RM) dist/main.js
	$(LN) ../test/web/index.html dist/index.html
	$(LN) ../test/web/main.js dist/main.js
	$(PYTHON) -m http.server -d dist
