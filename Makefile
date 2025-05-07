# -----BEGIN USER CONFIGURATION-----
TARGET := all

CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
RM := rm -rf
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1
# -----END USER CONFIGURATION-----

TARGETS := linux win lib-linux lib-win cli-linux cli-win web
MODS_CLI := main
MODS_COMMON := cadena dict lde log
MODS_LIB :=

.PHONY: all $(TARGETS) install remove clean

all: linux win web

linux: lib-linux cli-linux

win: lib-win cli-win

lib-linux: dist/liblily.so

lib-win: dist/liblily.dll

cli-linux: dist/lily

cli-win: dist/lily.exe

web: dist/liblily.js

lua-linux:
	cd lib/lua-5.4.7/src && make

lua-win:
	cd lib/lua-5.4.7/src && make mingw

lua-web:
	cd lib/lua-5.4.7/src && make generic CC='emcc -s WASM=1'

$(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON))): $(addprefix src/common/,$(addsuffix .c,$(MODS_COMMON)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/common/

$(addprefix src/cli/,$(addsuffix .o,$(MODS_CLI))): $(addprefix src/cli/,$(addsuffix .c,$(MODS_CLI)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/cli/

$(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))): $(addprefix src/lib/,$(addsuffix .c,$(MODS_LIB)))
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv *.o src/lib/

src/web/main.o: src/web/main.c
	@echo Generando $(notdir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^
	mv main.o src/web/

dist/liblily.so: $(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))) $(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON))) lua-linux
	@mkdir -p dist
	$(CC) -shared -fPIC $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/liblily.dll: $(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))) $(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON))) lua-win
	@mkdir -p dist
	$(CC) -shared $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/lily: $(addprefix src/cli/,$(addsuffix .o,$(MODS_CLI))) lua-linux
	@mkdir -p dist
	$(CC) $(LDFLAGS) $^ -L dist -llily $(LDLIBS) -o $@

dist/lily.exe: $(addprefix src/cli,$(addsuffix .o,$(MODS_CLI))) lua-win
	@mkdir -p dist 
	(CC) $(LDFLAGS) $^ -L dist -llily  $(LDLIBS) -o $@

dist/liblily.js: lua-web
	emcc -Ilib/lua-5.4.7/src main.c lua-5.4.0/src/liblua.a -s WASM=1 -O2 -o dist/liblily.js -s EXPORTED_FUNCTIONS="['_run_lua']" -s 'EXPORTED_RUNTIME_METHODS=["ccall", "cwrap"]' -s MODULARIZE=1 -s 'EXPORT_NAME="initWasmModule"'

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

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete
