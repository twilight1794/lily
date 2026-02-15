# -----BEGIN USER CONFIGURATION-----
TARGET := all

CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
LDFLAGS := -Ldist
LDLIBS := -lm
RM := rm -rf
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1
# -----END USER CONFIGURATION-----

V_LILY_VERSION=$(shell git tag --points-at HEAD)
V_LILY_COMMIT=$(shell git rev-parse HEAD|cut -c 1-8)
V_LILY_MODIFICADO=$(shell if test $$(git status --porcelain --untracked-files=no | wc -l) -gt 0; then echo '-changed'; else echo ''; fi)

TARGETS := linux windows lib-linux lib-windows web
.PHONY: all $(TARGETS) install-windows install-linux remove-linux remove-windows doxy clean test-windows test-linux

all: linux windows web
linux: dist/lily
windows: dist/lily.exe
lib-linux: dist/liblily.so
lib-windows: dist/liblily.dll
web: dist/liblily.js

src/common/cadena.o: src/common/cadena.c src/common/cadena.h
src/common/dict.o: src/common/dict.c src/common/dict.h
src/common/lde.o: src/common/lde.c src/common/lde.h
src/common/log.o: src/common/log.c src/common/log.h
src/cli/main.o: CFLAGS += -DLILY_VERSION=\"$(V_LILY_VERSION)\" -DLILY_COMMIT=\"$(V_LILY_COMMIT)\" -DLILY_MODIFICADO=\"$(V_LILY_MODIFICADO)\"
src/cli/main.o: src/cli/main.c
src/lib/a_lexico.o: src/lib/a_lexico.c src/lib/a_lexico.h src/lib/a_lexico_ctipos.h
src/lib/a_lexico_simbolo.o: src/lib/a_lexico_simbolo.c src/lib/a_lexico_simbolo.h
src/lib/a_sintactico.o: src/lib/a_sintactico.c src/lib/a_sintactico.h
src/lib/lua_cpu.o: src/lib/lua_cpu.c src/lib/lua_cpu.h
src/web/main.o: src/web/main.c

dist/liblily.so: src/common/cadena.o src/common/dict.o src/common/lde.o src/common/log.o src/lib/a_lexico.o src/lib/a_lexico_simbolo.o src/lib/a_sintactico.o src/lib/lua_cpu.o
	$(CC) -shared -fPIC $(LDFLAGS) $^ -llua $(LDLIBS) -o $@

dist/liblily.dll: src/common/cadena.o src/common/dict.o src/common/lde.o src/common/log.o src/lib/a_lexico.o src/lib/a_lexico_simbolo.o src/lib/a_sintactico.o src/lib/lua_cpu.o
	$(CC) -shared $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/lily: src/cli/main.o lib-linux
	$(CC) $(LDFLAGS) src/cli/main.o -llily $(LDLIBS) -o $@

dist/lily.exe: src/cli/main.o lib-windows
	$(CC) $(LDFLAGS) src/cli/main.o -llily $(LDLIBS) -o $@

dist/liblily.js: | dist
	emcc main.c libluawasm.a -s WASM=1 -O2 -o $@ -s EXPORTED_FUNCTIONS="['_run_lua']" -s 'EXPORTED_RUNTIME_METHODS=["ccall", "cwrap"]' -s MODULARIZE=1 -s 'EXPORT_NAME="initWasmModule"'

dist:
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

lib/munit/munit.o: lib/munit/munit.c

test/common/dict.o: test/common/dict.c
test/main.o: test/main.c

dist/test: lib/munit/munit.o test/main.o test/common/dict.o lib-linux
	$(CC) $(LDFLAGS) lib/munit/munit.o test/main.o test/common/dict.o -llily $(LDLIBS) -o $@

dist/test.exe: lib/munit/munit.o test/main.o test/common/dict.o lib-windows
	$(CC) $(LDFLAGS) lib/munit/munit.o test/main.o test/common/dict.o -llily $(LDLIBS) -o $@

test-linux: dist/test
	LD_LIBRARY_PATH=dist dist/test
test-windows: dist/test.exe
	dist/test.exe
