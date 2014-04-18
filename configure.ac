dnl ===========================================================================
dnl Process this file with autoconf to produce a configure script.
dnl $Id$
dnl ===========================================================================

AC_INIT(OpenCity, 0.0.7dev)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(src/main.cpp)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_CONFIG_HEADERS([config.h])


AM_INIT_AUTOMAKE([1.7 dist-bzip2])
AM_BINRELOC


dnl ===========================================================================
dnl We need ranlib to build index for accelerating symbols searching
dnl in archive files.
dnl ===========================================================================
dnl libtoolize: `AC_PROG_RANLIB' is rendered obsolete by `LT_INIT'
dnl AC_PROG_RANLIB


dnl ===========================================================================
dnl We use libtool to build convenience libraries
dnl ===========================================================================
LT_INIT
AC_PROG_LIBTOOL


dnl ===========================================================================
dnl We use install to install the program
dnl ===========================================================================
AC_PROG_INSTALL


dnl ===========================================================================
dnl Check for pkg-config
dnl ===========================================================================
AC_PATH_PROGS([PACKAGE_CONFIG], [$host_alias-pkg-config pkg-config], [none])
if test "x$PACKAGE_CONFIG" = "xnone"; then
  AC_MSG_ERROR([*** pkg-config, needed to check for libpng and libxml2 existence has not been found.])
fi

dnl ===========================================================================
dnl Detect the SDL path, with C language
dnl ===========================================================================
SDL_VERSION=1.2.12

AC_LANG_C
AM_PATH_SDL($SDL_VERSION, , AC_MSG_ERROR([SDL version $SDL_VERSION not found]))


dnl ===========================================================================
dnl We are doing checkings for a C++ program
dnl ===========================================================================
dnl CXX=g++ done by AC_PROG_CXX
dnl AC_SUBST(CXX)
dnl Set CPPFLAGS to disable -g -O2 and support the SDL libraries check

AC_PROG_CXX
AC_LANG_CPLUSPLUS

CPPFLAGS="$SDL_CFLAGS"
LIBS="$SDL_LIBS"


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
						LIBS="$SDL_LIBS -lSDL_mixer"
                        AC_DEFINE(OPENCITY_SDL_MIXER, 1, [SDL_Mixer support])
                        AC_MSG_RESULT(yes)
                else
                        AC_MSG_RESULT([no, found header but not the SDL_mixer library.])
                fi
        else
                AC_MSG_RESULT([no, audio system disabled.])
        fi
fi
AM_CONDITIONAL(OPENCITY_SDL_MIXER, test "x$SDL_mixer_h" = "xyes")


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
dnl Checks for libraries.
dnl ===========================================================================
if test "x$enable_mingw" = "xyes"; then
	LIBS="-lmingw32 -lSDLmain -mwindows -lglu32 -lopengl32 $LIBS"
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

# Need for viewer, also dependency of SDL_image
AC_MSG_CHECKING(for libpng)
for PNG in png png12 png13 dummypng; do
	if $PACKAGE_CONFIG --exists lib$PNG > /dev/null 2>&1 ; then
		CPPFLAGS="$CPPFLAGS `$PACKAGE_CONFIG --cflags lib$PNG`"
		LIBS="$LIBS `$PACKAGE_CONFIG --libs lib$PNG`"
	break
	fi
done
if test "$PNG" = "dummypng"; then
	AC_MSG_ERROR([*** libpng not found])
else
	AC_MSG_RESULT(lib$PNG)
fi


dnl ===========================================================================
dnl Optional linker --as-needed switch
dnl ===========================================================================
AC_ARG_ENABLE(as-needed,
        [  --enable-as-needed      link with --as-needed flag])

if test "x$enable_as_needed" = "xyes"; then
        enable_as_needed="-Wl,--as-needed"
else
        enable_as_needed=""
fi


dnl ===========================================================================
dnl Set the compiler and the linker options
dnl ===========================================================================
dnl Set C / C++ compiler options
CPPFLAGS="-Wall -Wmissing-braces -Wparentheses -pedantic-errors $CPPFLAGS"

dnl Set more C / C++ compiler options
CPPFLAGS="-I$with_gl_prefix/include $enable_gprof $enable_debug $CPPFLAGS"

dnl Set specific C compiler flags
CFLAGS="-std=c99 $CFLAGS"

dnl Set specific C++ compiler flags
CXXFLAGS="-std=c++98 $CXXFLAGS"

dnl Set linker options 
LDFLAGS="-L$with_gl_prefix/lib $enable_gprof $enable_as_needed $LDFLAGS"


dnl ===========================================================================
dnl Checks for header files.
dnl Checks for typedefs, structures, and compiler characteristics.
dnl Checks for library functions.
dnl ===========================================================================


dnl ===========================================================================
dnl Output makefiles
dnl ===========================================================================

AC_CONFIG_FILES([config/Makefile])
AC_CONFIG_FILES([docs/Makefile])
AC_CONFIG_FILES([map/Makefile])
AC_CONFIG_FILES([map/heightmap/Makefile])
AC_CONFIG_FILES([sound/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/binreloc/Makefile])
AC_CONFIG_FILES([src/enum/Makefile])
AC_CONFIG_FILES([src/framework/Makefile])
AC_CONFIG_FILES([src/framework/System/Makefile])
AC_CONFIG_FILES([src/framework/System/Information/Makefile])
AC_CONFIG_FILES([src/framework/System/Information/Software/Makefile])
AC_CONFIG_FILES([src/framework/System/Reflection/Makefile])
AC_CONFIG_FILES([src/mapgen/Makefile])
AC_CONFIG_FILES([src/mas/Makefile])
AC_CONFIG_FILES([src/model/Makefile])
AC_CONFIG_FILES([src/networking/Makefile])
AC_CONFIG_FILES([src/pngfuncs/Makefile])
AC_CONFIG_FILES([src/simulator/Makefile])
AC_CONFIG_FILES([src/structure/Makefile])
AC_CONFIG_FILES([src/tinyxml/Makefile])
AC_CONFIG_FILES([src/tinyxpath/Makefile])
AC_CONFIG_FILES([src/triangulation/Makefile])
AC_CONFIG_FILES([viewer/Makefile])
AC_CONFIG_FILES([Makefile])


AC_OUTPUT

echo "==============================================================================="
echo "  Configure has finished."
echo "  To compile OpenCity, do 'make'"
echo "  To install OpenCity, do 'make install'"
echo "  To remove OpenCity, do 'make uninstall'"
echo
echo "  After compiling, to run OpenCity from here without installation, do: "
echo "  'src/opencity --data-dir /path/to/share/opencity --conf-dir /path/to/etc/opencity'"
echo "==============================================================================="
