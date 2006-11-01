SUBDIRS = binreloc mapgen mas model networking simulator structure tinyxml triangulation
AM_CPPFLAGS = \
	-I@top_srcdir@/src/ \
	-I@top_srcdir@/src/binreloc/ \
	-I@top_srcdir@/src/mas/ \
	-I@top_srcdir@/src/mapgen/ \
	-I@top_srcdir@/src/model/ \
	-I@top_srcdir@/src/networking/ \
	-I@top_srcdir@/src/simulator/ \
	-I@top_srcdir@/src/structure/ \
	-I@top_srcdir@/src/triangulation/ \
	-DPREFIX=\"@prefix@/\"


bin_PROGRAMS = opencity
opencity_LDADD = \
binreloc/libbinreloc.a \
mapgen/libmapgen.a \
mas/libmas.a \
model/libmodel.a \
networking/libnet.a \
simulator/libsim.a \
structure/libstruct.a \
tinyxml/libtinyxml.a \
triangulation/libtriangulation.a


if AUDIO_SDL
        AUDIO_CPP = audiomanagersdl.cpp
        AUDIO_H = audiomanagersdl.h
else
        AUDIO_CPP = audiomanager.cpp
        AUDIO_H = audiomanager.h
endif


opencity_SOURCES = \
$(AUDIO_CPP) \
buildinglayer.cpp \
city.cpp \
conf.cpp \
destination.cpp \
graphicmanager.cpp \
guimain.cpp \
guicontainer.cpp \
guibutton.cpp \
gui3dview.cpp \
layer.cpp \
main.cpp \
map.cpp \
movement.cpp \
movementmanager.cpp \
renderer.cpp \
ui.cpp \
pathfinder.cpp \
persistence.cpp \
propertymanager.cpp \
texture.cpp \
vehicle.cpp


noinst_HEADERS = \
audiomanager.h \
audiomanagersdl.h \
buildinglayer.h \
city.h \
conf.h \
destination.h \
enum.h \
font_8x8.h \
globalvar.h \
graphicmanager.h \
guimain.h \
guicontainer.h \
guibutton.h \
gui3dview.h \
layer.h \
macros.h \
main.h \
map.h \
movement.h \
movementmanager.h \
pathfinder.h \
persistence.h \
propertymanager.h \
renderer.h \
texture.h \
ui.h \
vehicle.h


