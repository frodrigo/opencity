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


#==============================================================================
# The binary executables.
# It's necessary to compile the other programs after the main binary.
#==============================================================================
bin_PROGRAMS = opencity oczen


#==============================================================================
# Main OpenCity executable
#==============================================================================
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
else
    AUDIO_CPP = audiomanager.cpp
endif


opencity_SOURCES = \
$(AUDIO_CPP) \
buildinglayer.cpp \
city.cpp \
conf.cpp \
destination.cpp \
graphicmanager.cpp \
gui3dview.cpp \
guibar.cpp \
guibutton.cpp \
guicontainer.cpp \
guilabel.cpp \
guimain.cpp \
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
gui3dview.h \
guibar.h \
guibutton.h \
guicontainer.h \
guilabel.h \
guimain.h \
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
vehicle.h \
zen.h


#==============================================================================
# Zen server executable
#==============================================================================
oczen_LDADD = \
binreloc/libbinreloc.a \
mapgen/libmapgen.a \
mas/libmas.a \
model/libmodel.a \
networking/libnet.a \
simulator/libsim.a \
structure/libstruct.a \
tinyxml/libtinyxml.a \
triangulation/libtriangulation.a


oczen_SOURCES = \
audiomanager.cpp \
buildinglayer.cpp \
city.cpp \
conf.cpp \
destination.cpp \
graphicmanager.cpp \
gui3dview.cpp \
guibar.cpp \
guibutton.cpp \
guicontainer.cpp \
guilabel.cpp \
guimain.cpp \
layer.cpp \
map.cpp \
movement.cpp \
movementmanager.cpp \
renderer.cpp \
ui.cpp \
pathfinder.cpp \
persistence.cpp \
propertymanager.cpp \
texture.cpp \
vehicle.cpp \
zen.cpp

