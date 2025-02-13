BINDIR = bin
COPY = cp
CXX = g++
CXXFLAGS = -g3 -Iinclude -Ideps/libal5poly/include -ID:/mingw32/i686-w64-mingw32/include -Wall
DEPSDIR = deps
INCDIR = include
LIBS = -LD:/mingew32/i686-w64-mingw32/lib -Ldeps/libal5poly/bin/ -llibal5poly -LD:/Users/Brandon/bin -LD:/Users/Brandon/lib -lallegro_monolith-5.2 -ljpeg -ldumb -lFLAC -lfreetype -lvorbisfile -lvorbis -logg -lphysfs -lpng16 -lzlibstatic -ldsound -lgdiplus -luuid -lkernel32 -lwinmm -lpsapi -lopengl32 -lglu32 -luser32 -lcomdlg32 -lgdi32 -lshell32 -lole32 -ladvapi32 -lws2_32 -lshlwapi -lwebp
MKDIR = mkdir -p
OBJDIR = obj
PREFIX =
REMOVE = rm -fR
SRCDIR = src

GAME = ${BINDIR}/game.exe
OBJECTS = ${OBJDIR}/main.o

.PHONY: all build-deps clean deepclean game libal5poly run veryclean

all: game

build-deps: ${DEPSDIR}/libal5poly
	cd $< && ${MAKE} -f Makefile.mingw32 clean
	cd $< && ${MAKE} -f Makefile.mingw32

clean:
	${REMOVE} ${BINDIR} ${OBJDIR}

deepclean:
	-cd ${DEPSDIR}/libal5poly && ${MAKE} -f Makefile.mingw32 clean
	${MAKE} clean

dirs: ${BINDIR} ${OBJDIR}

game: libal5poly ${GAME}

libal5poly: ${DEPSDIR}/libal5poly/bin/libal5poly.1.dll

run: game
	${GAME}

veryclean: clean
	rm -fR ${DEPSDIR}/libal5poly

${DEPSDIR}/libal5poly:
	git clone git://github.com/bamccaig/libal5poly.git deps/libal5poly
	cd $@ && git remote set-url --push origin git@github.com:bamccaig/libal5poly.git && git checkout Allegro525_GCC81_MinGW_W64_i686_posix_dwarf

${DEPSDIR}/libal5poly/bin/libal5poly.1.dll: ${DEPSDIR}/libal5poly
	cd $< && ${MAKE} -f Makefile.mingw32

${BINDIR}:
	${MKDIR} $@

${BINDIR}/libal5poly.1.dll:
	ln -s ../deps/libal5poly/bin/libal5poly.1.dll $@

${OBJDIR}:
	${MKDIR} $@

${GAME}: ${OBJECTS} ${BINDIR} ${BINDIR}/libal5poly.1.dll
	${CXX} -o $@ ${OBJECTS} ${LIBS}

${OBJDIR}/H4xDummy.o: ${SRCDIR}/H4xDummy.cpp ${INCDIR}/H4xDummy.hpp ${OBJDIR}
	${CXX} ${CXXFLAGS} -c -o $@ $<

${OBJDIR}/JumpHandler.o: ${SRCDIR}/JumpHandler.cpp ${INCDIR}/JumpHandler.hpp ${OBJDIR}
	${CXX} ${CXXFLAGS} -c -o $@ $<

${OBJDIR}/RunHandler.o: ${SRCDIR}/RunHandler.cpp ${INCDIR}/RunHandler.hpp ${OBJDIR}
	${CXX} ${CXXFLAGS} -c -o $@ $<

${OBJDIR}/main.o: ${SRCDIR}/main.cpp ${OBJDIR}
	${CXX} ${CXXFLAGS} -c -o $@ $<
