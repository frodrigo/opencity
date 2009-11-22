#==============================================================================
# Makefile.am for the libmapgen.a
# $Id$
#==============================================================================
AM_CXXFLAGS = \
	-I@top_srcdir@/src/mapgen \
	-I@top_srcdir@/src/mapgen/filter \
	-I@top_srcdir@/src/mapgen/generator \
	-I@top_srcdir@/src/pngfuncs \
	-I@top_srcdir@/src

noinst_LIBRARIES = libmapgen.a

libmapgen_a_SOURCES = \
	map.cpp \
	mapmaker.cpp \
	filter/choper.cpp \
	filter/context.cpp \
	filter/contextOnlyPositive.cpp \
	filter/contextualizer.cpp \
	filter/cutter.cpp \
	filter/filter.cpp \
	filter/flattern.cpp \
	filter/gaussblur.cpp \
	filter/normalize.cpp \
	filter/shaper.cpp \
	filter/shape.cpp \
	filter/shapeBubble.cpp \
	filter/shapeTopRound.cpp \
	filter/shapeVolcano.cpp \
	generator/diamon.cpp \
	generator/generator.cpp \
	generator/heightMap.cpp

noinst_HEADERS = \
	map.h \
	mapmaker.h \
	filter/choper.h \
	filter/context.h \
	filter/contextOnlyPositive.h \
	filter/contextualizer.h \
	filter/cutter.h \
	filter/filter.h \
	filter/flattern.h \
	filter/gaussblur.h \
	filter/normalize.h \
	filter/shaper.h \
	filter/shape.h \
	filter/shapeBubble.h \
	filter/shapeTopRound.h \
	filter/shapeVolcano.h \
	generator/diamon.h \
	generator/generator.h \
	generator/heightMap.h

