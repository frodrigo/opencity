AM_CPPFLAGS = \
	-I@top_srcdir@/src/ \
	-I@top_srcdir@/src/model/

noinst_PROGRAMS = viewer

viewer_LDADD = ../src/model/libmodel.a ../src/triangulation/libtriangulation.a ../src/texture.o ../src/conf.o

viewer_SOURCES = viewer.cpp
