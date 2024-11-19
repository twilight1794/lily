CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wpedantic -funsigned-char -std=c99
RM := rm -rf
BINDIR := /usr/local/bin
LIBDIR := /usr/local/lib
MANDIR := /usr/local/share/man/man1

.PHONY: all install remove clean

all: dist/lily dist/liblily.so

MODS_CLI := main
MODS_COMMON := cadena dict lde log
MODS_LIB := z80_lexico exp_lexico

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

dist/liblily.so: $(addprefix src/lib/,$(addsuffix .o,$(MODS_LIB))) $(addprefix src/common/,$(addsuffix .o,$(MODS_COMMON)))
	@mkdir -p dist
	$(CC) -shared -fPIC $(LDFLAGS) $^ $(LDLIBS) -o $@

dist/lily: dist/liblily.so $(addprefix src/cli/,$(addsuffix .o,$(MODS_CLI)))
	@mkdir -p dist
	$(CC) $(LDFLAGS) $^ -L dist -llily $(LDLIBS) -o $@

install:
	cp dist/lily $(BINDIR)/lily
	cp dist/liblily.so $(LIBDIR)/liblily.so
	ldconfig

remove:
	$(RM) $(BINDIR)/lily
	$(RM) $(LIBDIR)/liblily.so

clean:
	$(RM) dist/*
	find . -regex .*\\.o -delete
