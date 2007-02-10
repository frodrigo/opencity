AM_CPPFLAGS = \
	-I@top_srcdir@/src/mapgen/ \
	-I@top_srcdir@/src/mapgen/filter \
	-I@top_srcdir@/src/mapgen/generator \
	-I@top_srcdir@/src


noinst_LIBRARIES = libmapgen.a


libmapgen_a_SOURCES = \
map.cpp \
mapmaker.cpp \
filter/choper.cpp \
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
generator/generator.cpp


noinst_HEADERS = \
map.h \
mapmaker.h \
filter/choper.h \
filter/filter.h \
filter/flattern.h \
filter/gaussblur.h \
filter/normalize.h \
filter/shaper.h \
filter/shape.h \
filter/shapeBubble.h \
filter/shapeTopRound.h \
filter/shapeVolcano.cpp \
generator/diamon.h \
generator/generator.h


