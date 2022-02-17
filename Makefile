MAJORVERSION=0
MINORVERSION=1

LIBDIR=/usr/lib
INCDIR=/usr/include
OPTFLAGS=-march=native -mtune=native -O3
CFLAGS=-std=c99 -pedantic -Wall -Wextra ${OPTFLAGS}
LIBS=
LDFLAGS=-fpic ${LIBS}

all: lib/libnoise.so.${MAJORVERSION}.${MINORVERSION}

install: lib/libnoise.so.${MAJORVERSION}.${MINORVERSION} uninstall
	cp include/noise.h ${INCDIR}
	cp $< ${LIBDIR}
	ln -s ${LIBDIR}/libnoise.so.${MAJORVERSION}.${MINORVERSION} ${LIBDIR}/libnoise.so.${MAJORVERSION}
	ln -s ${LIBDIR}/libnoise.so.${MAJORVERSION} ${LIBDIR}/libnoise.so

lib/libnoise.so.${MAJORVERSION}.${MINORVERSION}: obj/noise.o
	mkdir $(@D)
	cc ${LDFLAGS} -shared -o $@ $^ -Wl,-soname,libnoise.so.${MAJORVERSION}

obj/noise.o: src/noise.c
	mkdir $(@D)
	cc ${CFLAGS} ${LDFLAGS} -I include -c -o $@ $<

uninstall:
	rm -f ${INCDIR}/noise.h
	rm -f ${LIBDIR}/libnoise.*
