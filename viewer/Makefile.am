AM_CPPFLAGS = \
	-I@top_srcdir@/src/ \
	-I@top_srcdir@/src/enum/ \
	-I@top_srcdir@/src/mas/ \
	-I@top_srcdir@/src/model/ \
	-I@top_srcdir@/src/networking/ \
	-I@top_srcdir@/src/pngfuncs/ \
	-I@top_srcdir@/src/structure/

noinst_PROGRAMS = viewer

viewer_LDADD = \
	../src/model/libmodel.a \
	../src/pngfuncs/libpngfuncs.a \
	../src/structure/libstruct.a \
	../src/triangulation/libtriangulation.a

viewer_SOURCES = \
viewer.cpp \
../src/conf.cpp \
../src/globalvar.cpp \
../src/persistence.cpp \
../src/propertymanager.cpp \
../src/texture.cpp

noinst_HEADERS = \
../src/conf.h \
../src/globalvar.h \
../src/persistence.h \
../src/propertymanager.h \
../src/SimpleOpt.h \
../src/texture.h
