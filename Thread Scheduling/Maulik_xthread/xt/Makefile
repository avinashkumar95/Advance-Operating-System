#
# Makefile for building xthreads library on i386
#
# Maintain the following definition:
#	HFILES	all header files (*.h) programmer created
#	SFILES	all C source files (*.c) programmer created
#	OFILES	all object files (*.o) required to load program
#
# Use the following make targets:
#	depend	to update header file dependencies

.DEFAULT:
	co -q $@

HFILES = proc.h  
CFILES = create.c resched.c main.c queue.c join.c
SFILES = ctxsw.s 
OFILES = create.o ctxsw.o resched.o main.o queue.o join.o
XTLIB  = ./libxt.a
APP_CFILES = xmain.c xmain1.c xmain2.c xmain3.c xmain4.c xmain5.c
APP_OFILES = xmain.o

IFLAGS = -g -I. -I/usr/lib/gcc/x86_64-linux-gnu/7/include
CFLAGS =   ${IFLAGS} -Wno-implicit
DEPFLAGS = ${IFLAGS}
CC     = gcc -m32
AS     = as -32

RCS     = Makefile ${HFILES} ${CFILES}

a.out: ${XTLIB} ${APP_OFILES}
	${CC} ${CFLAGS} ${APP_OFILES} ${XTLIB} 

${XTLIB}: ${OFILES}
	ar cr ${XTLIB} ${OFILES}

ci:
	ci -u ${RCS}

clean:
	rm -f ${OFILES}	${APP_OFILES}

depend:
	makedepend ${DEPFLAGS} ${CFILES} ${SFILES}
# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT

submit:
	turnin -c cis620s -p proj2 Makefile ${HFILES} ${CFILES} ${SFILES} ${APP_CFILES}
