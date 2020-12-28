BINDIR = bin
COPY = cp
CXX = g++
CXXFLAGS = -g3 -Iinclude -Ideps/libal5poly/include -ID:/mingw32/i686-w64-mingw32/include -std=c++17 -Wall
DEPSDIR = deps
GDB = gdb
INCDIR = include
LIBS = -LD:/mingw32/i686-w64-mingw32/lib -Ldeps/libal5poly/bin/ -lal5poly -LD:/Users/Brandon/bin -LD:/Users/Brandon/lib -lallegro_monolith-5.2 -ljpeg -ldumb -lFLAC -lfreetype -lvorbisfile -lvorbis -logg -lphysfs -lpng16 -lzlibstatic -ldsound -lgdiplus -luuid -lkernel32 -lwinmm -lpsapi -lopengl32 -lglu32 -luser32 -lcomdlg32 -lgdi32 -lshell32 -lole32 -ladvapi32 -lws2_32 -lshlwapi -lwebp
MKDIR = mkdir -p
OBJDIR = obj
PREFIX =
REMOVE = rm -fR
SRCDIR = src

DIRS = ${BINDIR} ${OBJDIR}
GAME = ${BINDIR}/game.exe
OBJECTS = ${OBJDIR}/main.o

.PHONY: all build-deps clean debug deepclean dirs game libal5poly prerequisite run veryclean ${DIRS}

all: game

build-deps: ${DEPSDIR}/libal5poly
	cd $< && ${MAKE} -f Makefile.mingw32 library

debug:
	$(GDB) $(GAME)

clean:
	${REMOVE} ${DIRS}

deepclean: clean
	-cd ${DEPSDIR}/libal5poly && ${MAKE} -f Makefile.mingw32 clean

dirs: ${DIRS}
	@${MKDIR} $?

game: libal5poly ${GAME}

libal5poly: prerequisite ${DEPSDIR}/libal5poly/bin/libal5poly.1.dll

prerequisite: dirs ${DEPSDIR}/libal5poly/bin/libal5poly.1.dll
	${MAKE} -C ${DEPSDIR}/libal5poly -f Makefile.mingw32 library

run: game
	${GAME}

veryclean: clean
	${MAKE} -C ${DEPSDIR}/libal5poly -f Makefile.mingw32 clean

#${DEPSDIR}/libal5poly:
#	git clone git://github.com/bamccaig/libal5poly.git deps/libal5poly
#	cd $@ && git remote set-url --push origin git@github.com:bamccaig/libal5poly.git && git checkout Allegro525_GCC81_MinGW_W64_i686_posix_dwarf

${DEPSDIR}/libal5poly/bin/libal5poly.1.dll: ${DEPSDIR}/libal5poly
	${MAKE} -C ${DEPSDIR}/libal5poly -f Makefile.mingw32 library

${BINDIR}/libal5poly.1.dll:
	if [[ -x mklink ]]; then mklink $@ ../deps/libal5poly/bin/libal5poly.1.dll; else ln -s ../deps/libal5poly/bin/libal5poly.1.dll $@; fi;

${GAME}: ${OBJECTS} ${BINDIR}/libal5poly.1.dll
	${CXX} -o $@ ${OBJECTS} ${LIBS}

${OBJDIR}/main.o: ${SRCDIR}/main.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<
