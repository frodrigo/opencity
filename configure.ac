dnl ===========================================================================
dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.ac,v 1.22 2006/04/17 09:37:33 neoneurone Exp $
dnl ===========================================================================

AC_INIT(OpenCity, 0.0.4beta)
AC_CONFIG_SRCDIR(src/main.cpp)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_CONFIG_HEADERS([header.h])
dnl AC_PREFIX_DEFAULT($HOME/bin/opencity) # We use the old standard /usr/local

AM_INIT_AUTOMAKE([1.7 dist-bzip2])
AM_BINRELOC


dnl We need ranlib to build index for accelerating symbols searching
dnl in archive files.
AC_PROG_RANLIB
AC_PROG_INSTALL


dnl ===========================================================================
dnl Detect the SDL path, with C language
dnl ===========================================================================
SDL_VERSION=1.2.9

AC_LANG_C
AM_PATH_SDL($SDL_VERSION)


dnl ===========================================================================
dnl We are doing checkings for a C++ program
dnl ===========================================================================
dnl CXX=g++ done by AC_PROG_CXX
dnl AC_SUBST(CXX)
dnl Set CXXFLAGS to disable -g -O2 and support the SDL libraries check

CXXFLAGS="$SDL_CFLAGS"
LDFLAGS="$LDFLAGS $SDL_LIBS"

AC_PROG_CXX
AC_LANG_CPLUSPLUS


dnl Checks for programs.


dnl ===========================================================================
dnl Checks for --disable-sdl-mixer argument
dnl ===========================================================================
AC_ARG_ENABLE(sdl-mixer,
        [  --disable-sdl-mixer     disable SDL_mixer linking])

if test "x$enable_sdl_mixer" = "x"; then
        AC_CHECK_HEADER(SDL_mixer.h, SDL_mixer_h="yes", SDL_mixer_h="no")
        AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, SDL_mixer_lib="yes", SDL_mixer_lib="no")
        AC_MSG_CHECKING(SDL_mixer support)

        if test "x$SDL_mixer_h" = "xyes"; then
                if test "x$SDL_mixer_lib" = "xyes"; then
                        LDFLAGS="$LDFLAGS -lSDL_mixer"
                        AC_DEFINE(AUDIO_SDL, 1, [SDL_Mixer support])
                        AC_MSG_RESULT(yes)
                else
                        AC_MSG_RESULT([no, found header but not SDL_mixer library])
                fi
        else
                AC_MSG_RESULT([no, SDL_mixer library required. See FAQ for more information])
        fi
fi
AM_CONDITIONAL(AUDIO_SDL, test "x$SDL_mixer_h" = "xyes")


dnl ===========================================================================
dnl Checks for --with-gl-prefix argument
dnl ===========================================================================
AC_ARG_WITH(gl,
        [  --with-gl-prefix=DIR    specify OpenGL prefix (DIR="/usr/X11R6")])

if test "x$with_gl_prefix" = "x"; then
        with_gl_prefix="/usr/X11R6"
fi


dnl ===========================================================================
dnl Checks for --enable-gprof argument
dnl ===========================================================================
AC_ARG_ENABLE(gprof,
        [  --enable-gprof          enable GNU profiling support])

if test "x$enable_gprof" = "xyes"; then
        enable_gprof="-pg"
else
        enable_gprof=""
fi


dnl ===========================================================================
dnl Checks for --enable-gdb argument
dnl ===========================================================================
AC_ARG_ENABLE(gdb,
        [  --enable-gdb            enable GDB support with debugging enabled])

if test "x$enable_gdb" = "xyes"; then
        enable_gdb="-ggdb"
else
        enable_gdb=""
fi


dnl ===========================================================================
dnl Checks for --enable-debug argument
dnl ===========================================================================
AC_ARG_ENABLE(debug,
        [  --enable-debug          enable ALL debugging codes])

if test "x$enable_debug" = "xyes"; then
        enable_debug="$enable_gdb"
else
        enable_debug="-DNDEBUG"
fi


dnl ===========================================================================
dnl Checks for --enable-mingw argument
dnl ===========================================================================
AC_ARG_ENABLE(mingw,
        [  --enable-mingw          enable MinGW32 support])


dnl ===========================================================================
dnl Set the compiler and the linker options
dnl ===========================================================================
CXXFLAGS="-Wall -ansi -pedantic -fno-exceptions $CXXFLAGS"

CXXFLAGS="-I$with_gl_prefix/include $enable_gprof $enable_debug $CXXFLAGS"

LDFLAGS="-L$with_gl_prefix/lib $enable_gprof $LDFLAGS"


dnl ===========================================================================
dnl Checks for libraries.
dnl ===========================================================================
if test "x$enable_mingw" = "xyes"; then
	LDFLAGS="-lmingw32 -lSDLmain -mwindows -lglu32 -lopengl32 $LDFLAGS"
else
	AC_CHECK_LIB(GL, glBegin, ,
		AC_MSG_ERROR([OpenGL library required. See FAQ for more information]))
	AC_CHECK_LIB(GLU, gluLookAt, ,
		AC_MSG_ERROR([GLU library required. See FAQ for more information]))
fi

dnl Not worked yet
dnl AC_CHECK_HEADERS([SDL_ttf.h SDL_image.h SDL_net.h])

#AC_CHECK_LIB(SDL_ttf, TTF_Init, ,
#        AC_MSG_ERROR([SDL_ttf library required. See FAQ for more information]))


AC_CHECK_LIB(SDL_image, IMG_Load, ,
        AC_MSG_ERROR([SDL_image library required. See FAQ for more information]))


AC_CHECK_LIB(SDL_net, SDLNet_Init, ,
        AC_MSG_ERROR([SDL_net library required. See FAQ for more information]))


dnl ===========================================================================
dnl Checks for header files.
dnl Checks for typedefs, structures, and compiler characteristics.
dnl Checks for library functions.

AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/binreloc/Makefile])
AC_CONFIG_FILES([src/mapgen/Makefile])
AC_CONFIG_FILES([src/mas/Makefile])
AC_CONFIG_FILES([src/model/Makefile])
AC_CONFIG_FILES([src/networking/Makefile])
AC_CONFIG_FILES([src/simulator/Makefile])
AC_CONFIG_FILES([src/structure/Makefile])
AC_CONFIG_FILES([viewer/Makefile])
AC_CONFIG_FILES([Makefile])
dnl AC_CONFIG_FILES([config/opencity.conf]) -> not used any more
dnl AC_CONFIG_FILES([sound/sound.m3u]) -> not used any more

AC_OUTPUT

echo
echo "Configure finished."
echo "To install OpenCity, do 'make && make install'"
echo "To remove OpenCity, do 'make uninstall'"
echo
echo "Or to run OpenCity from here without installation, do 'make && src/opencity --homedir `pwd`/'"
echo








