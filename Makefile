# Makefile para Lua

# ============================
# === CONFIGURACIÓN BÁSICA ===
# Normalmente, basta con configurar solamente estas dos variables.

# Plataforma donde se está realizando la compilación
# Las opciones pueden ser: auto, posix, mingw, windows
PLAT_HOST := auto

# Plataforma objetivo de la compilación
# Normalmente, la misma plataforma que el host.
# Las opciones pueden ser: auto, posix, web, windows
PLAT_TARGET := $(PLAT_HOST)

# Números de versión
V1 := 0
V2 := 1

# Opciones de depuración
# Definir esto para habilitar opciones de depuración
#DEBUG := true

# =========================================================
# === MODIFICAR SOLO SI SE NECESITAN AJUSTES ESPECIALES ===
# Opciones de compilación
CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
LDFLAGS := -Ldist
LDLIBS := -lm

# Directorios para instalar los archivos
INSTALL_TOP :=
INSTALL_BIN :=
INSTALL_INC :=
INSTALL_LIB :=
INSTALL_DOC :=
INSTALL_MISC :=
# Comandos varios
MKDIR :=
RM :=
LN :=
PYTHON :=
INSTALL_EXEC :=
INSTALL_DATA :=

# =======================================
# === FIN DE CONFIGURACIÓN DE USUARIO ===
# Ya no debería modificar nada después de este punto

SHELL := /bin/sh

V := $(V1).$(V2)

# Macros para el código
V_LILY_VERSION :=$(shell git tag --points-at HEAD)
ifeq ($(strip $(V_LILY_VERSION)),)
  V_LILY_VERSION := $(V)
endif
V_LILY_COMMIT :=$(shell git rev-parse HEAD|cut -c 1-8)
ifeq ($(strip $(V_LILY_COMMIT)),)
  V_LILY_COMMIT := nogit
endif
V_LILY_MODIFICADO :=$(shell if test $$(git status --porcelain --untracked-files=no | wc -l) -gt 0; then echo '-changed'; fi)
CFLAGS += -DLILY_VERSION=\"$(V_LILY_VERSION)\" -DLILY_COMMIT=\"$(V_LILY_COMMIT)\" -DLILY_MODIFICADO=\"$(V_LILY_MODIFICADO)\" -DMISCDIR=\"$(INSTALL_MISC)\"

# Determinación de SO host
ifeq ($(PLAT_HOST),auto)
  OS_UNAME := $(shell uname -s)
  ifeq ($(findstring MINGW,$(OS_UNAME)),MINGW)
    PLAT_HOST := mingw
  else
    PLAT_HOST := posix
  endif
endif

# Determinación de SO objetivo
ifeq ($(PLAT_TARGET),auto)
  PLAT_TARGET := $(PLAT_HOST)
endif

# Determinación de directorios
ifndef INSTALL_TOP
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_TOP := %PROGRAMFILES%/Lily
  else
    INSTALL_TOP := /usr/local
  endif
endif
ifndef INSTALL_BIN
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_BIN :=
  else
    INSTALL_BIN := $(INSTALL_TOP)/bin
  endif
endif
ifndef INSTALL_INC
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_INC :=
  else
    INSTALL_INC := $(INSTALL_TOP)/include
  endif
endif
ifndef INSTALL_LIB
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_LIB :=
  else
    INSTALL_LIB := $(INSTALL_TOP)/lib
  endif
endif
ifndef INSTALL_DOC
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_DOC :=
  else
    INSTALL_DOC := $(INSTALL_TOP)/share/man
  endif
endif
ifndef INSTALL_MISC
  ifeq ($(PLAT_TARGET),windows)
    INSTALL_MISC :=
  else
    INSTALL_MISC := $(INSTALL_TOP)/share/lily
  endif
endif

# Determinación de programas a utilizar
ifndef MKDIR
  ifeq ($(PLAT_HOST),windows)
    MKDIR := mkdir
  else
    MKDIR := mkdir -p
  endif
endif
ifndef RM
  ifeq ($(PLAT_HOST),windows)
    RM := del /f
  else
    RM := rm -rf
  endif
endif
ifndef LN
  ifeq ($(PLAT_HOST),windows)
    LN := mklink /J
  else
    LN := ln -s
  endif
endif
ifndef PYTHON
  PYTHON := python
endif
ifndef INSTALL_EXEC
  ifeq ($(PLAT_HOST),windows)
    INSTALL_EXEC := cp
  else
    INSTALL_EXEC := install -p -m 0755
  endif
endif
ifndef INSTALL_DATA
  ifeq ($(PLAT_HOST),windows)
    INSTALL_DATA := cp
  else
    INSTALL_DATA := install -p -m 0644
  endif
endif

# Determinación de artefactos
ifeq ($(PLAT_TARGET),web)
  TO_BIN := lily.mjs
else ifeq ($(PLAT_TARGET),windows)
  TO_BIN := lily.exe
else
  TO_BIN := lily
endif
TO_INC := lily.h
ifeq ($(PLAT_TARGET),web)
  TO_LIB := liblily.mjs
else ifeq ($(PLAT_TARGET),windows)
  TO_LIB := liblily.dll
else
  TO_LIB := liblily.so
endif
ifeq ($(PLAT_TARGET),posix)
  TO_DOC := lily.1 lily.lua.1
else
  TO_DOC := lily.html lily.lua.html
endif
TO_MISC := $(addsuffix .lua,z80 8086 lc3)
ifeq ($(PLAT_TARGET),web)
  TO_TEST := liblily.mjs
else ifeq ($(PLAT_TARGET),windows)
  TO_TEST := test.exe
else
  TO_TEST := test
endif


# Establecer opciones de optimización y depuración
ifeq ($(PLAT_TARGET),web)
  # Por ahora, solo emscripten tiene opciones raras
  ifdef DEBUG
    web: CFLAGS += -gsource-map
    web: LDFLAGS += -gsource-map -sASSERTIONS
  else
    web: CFLAGS += -Oz
    web: LDFLAGS += -Oz -sASSERTIONS=0
  endif
else
  ifdef DEBUG
    CFLAGS += -ggdb3
  else
    CFLAGS += -Os
  endif
endif

# Archivos fuente
DEPS_LIBLILY_COMMON := cadena defs dict lde log nums
DEPS_LIBLILY_LIB := a_lexico a_semantico a_sintactico lily lua_cpu lua_ensamble lua_entorno lua_int simbolo
DEPS_LILY_POSIX := main mmap
DEPS_LILY_WINDOWS := main mmap_windows

# Objetivos
# Determinar automáticamente el tipo de objetivo por el sistema objetivo
ifeq ($(PLAT_TARGET),web)
  PREREQ_INICIO = lib
else
  PREREQ_INICIO = cli
endif
all: $(PREREQ_INICIO)

# Objetivos
cli: LDLIBS += -llily
ifeq ($(PLAT_TARGET),windows)
  CLI_DEPS := $(addprefix src/cli/,$(addsuffix .o,$(DEPS_LILY_WINDOWS))
else
  CLI_DEPS := $(addprefix src/cli/,$(addsuffix .o,$(DEPS_LILY_POSIX)))
endif
cli: dist/$(TO_BIN)

ifeq ($(PLAT_TARGET),web)
  lib: CC := emcc
  lib: CFLAGS += -I/usr/local/include -sWASM=1 -sSTRICT
  lib: LDFLAGS += -I/usr/local/include -L/usr/local/lib -sSTRICT -sSAFE_HEAP=1 -sALLOW_TABLE_GROWTH=1 -sALLOW_MEMORY_GROWTH=1 -sMODULARIZE=1 -sPOLYFILL=0
  lib: LDFLAGS += -s EXPORTED_FUNCTIONS="['_lily_lily_ensamble', '_malloc', '_free']" -s 'EXPORTED_RUNTIME_METHODS=["ccall", "setValue", "getValue", "stringToUTF8", "lengthBytesUTF8", "UTF8ToString", "addFunction", "removeFunction"]' -sSINGLE_FILE=1
  lib: LDLIBS += -lluawasm
else
  lib: LDFLAGS += -shared
  lib: LDLIBS += -llua
endif
LIB_DEPS := $(addsuffix .o,$(addprefix src/common/,$(DEPS_LIBLILY_COMMON)) $(addprefix src/lib/,$(DEPS_LIBLILY_LIB)))
lib: dist/$(TO_LIB)

test: LDLIBS += -llily
test: dist/$(TO_TEST)
ifeq ($(PLAT_TARGET),web)
	$(RM) dist/index.html
	$(RM) dist/main.js
	$(LN) ../test/web/index.html dist/index.html
	#FIX: mover biblioteca a src
	#$(LN) ../test/web/main.js dist/main.js
	$(PYTHON) -m http.server -d dist
else ifeq ($(PLAT_TARGET),windows)
	dist/test.exe
else
	LD_LIBRARY_PATH=dist dist/test
endif

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete

install:
ifneq (,$(filter mingw windows,$(PLAT_TARGET)))
	$(MKDIR) $(INSTALL_TOP)
	$(INSTALL_EXEC) dist/$(TO_BIN) $(INSTALL_TOP)
	$(INSTALL_DATA) dist/$(TO_LIB) $(INSTALL_TOP)
	$(INSTALL_DATA) dist/$(TO_DOC) $(INSTALL_TOP)
	$(MKDIR) $(INSTALL_MISC)
	$(MKDIR) $(INSTALL_MISC)/cpu
	$(INSTALL_DATA) dist/$(TO_MISC) $(INSTALL_MISC)/cpu
else
	$(INSTALL_EXEC) dist/$(TO_BIN) $(INSTALL_BIN)
	$(INSTALL_DATA) dist/$(TO_INC) $(INSTALL_INC)
	$(INSTALL_DATA) dist/$(TO_LIB) $(INSTALL_LIB)
	$(INSTALL_DATA) dist/$(TO_DOC) $(INSTALL_DOC)
	$(MKDIR) $(INSTALL_MISC)
	$(MKDIR) $(INSTALL_MISC)/cpu
	$(INSTALL_DATA) dist/$(TO_MISC) $(INSTALL_MISC)/cpu
	ldconfig
endif

uninstall:
ifneq (,$(filter mingw windows,$(PLAT_TARGET)))
	$(RM) $(INSTALL_TOP)
else
	cd $(INSTALL_BIN) && $(RM) $(TO_BIN)
	cd $(INSTALL_LIB) && $(RM) $(TO_LIB)
	cd $(INSTALL_DOC) && $(RM) $(TO_DOC)
	$(RM) $(INSTALL_MISC)
endif

dist:
	-git submodule update --init --recursive
	PAXDIR="$${PWD##*/}" && cd .. && tar --posix --exclude='.ccls-cache' --exclude='*.o' -czvf $$PAXDIR/dist/lily_$(V1).$(V2).tar.gz $$PAXDIR/doc $$PAXDIR/lib $$PAXDIR/misc $$PAXDIR/src $$PAXDIR/test $$PAXDIR/COPYIN* $$PAXDIR/Doxyfile $$PAXDIR/.editorconfig $$PAXDIR/Makefile $$PAXDIR/README*

echo:
	@echo "PLAT_HOST := $(PLAT_HOST)"
	@echo "PLAT_TARGET := $(PLAT_TARGET)"
	@echo "V1 := $(V1)"
	@echo "V2 := $(V2)"
	@echo "DEBUG := $(DEBUG)"
	@echo "INSTALL_TOP := $(INSTALL_TOP)"
	@echo "INSTALL_BIN := $(INSTALL_BIN)"
	@echo "INSTALL_INC := $(INSTALL_INC)"
	@echo "INSTALL_LIB := $(INSTALL_LIB)"
	@echo "INSTALL_DOC := $(INSTALL_DOC)"
	@echo "INSTALL_CPU := $(INSTALL_CPU)"
	@echo "MKDIR := $(MKDIR)"
	@echo "RM := $(RM)"
	@echo "LN := $(LN)"
	@echo "PYTHON := $(PYTHON)"
	@echo "INSTALL_EXEC := $(INSTALL_EXEC)"
	@echo "INSTALL_DATA := $(INSTALL_DATA)"

doxy:
	$(MKDIR) dist
	doxygen Doxyfile

src/common/cadena.o: src/common/cadena.c src/common/cadena.h
src/common/defs.o: src/common/defs.c src/common/defs.h
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

dist/lily dist/lily.mjs dist/lily.exe: lib $(CLI_DEPS)
	$(MKDIR) dist
	$(CC) $(LDFLAGS) $(CLI_DEPS) $(LDLIBS) -o $@

dist/liblily.so dist/liblily.mjs dist/liblily.dll: $(LIB_DEPS)
	$(MKDIR) dist
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/%.1: doc/%.adoc
	$(MKDIR) dist
	asciidoctor -b manpage $^ -o $@

dist/%.html: doc/%.adoc
	$(MKDIR) dist
	asciidoctor $^ -o $@

lib/munit/munit.o: lib/munit/munit.c

test/common/dict.o: test/common/dict.c test/common/dict.h
test/main.o: test/main.c

dist/test dist/test.exe: lib/munit/munit.o test/main.o test/common/dict.o $(TO_LIB)

#$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: all cli lib test clean install uninstall dist echo doxy
