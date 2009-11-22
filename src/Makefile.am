#==============================================================================
# Makefile.am for the source root.
# $Id$
#==============================================================================
SUBDIRS = binreloc enum framework mapgen mas model networking pngfuncs \
	simulator structure tinyxml tinyxpath triangulation

AM_CXXFLAGS = \
	-I@top_srcdir@/src \
	-I@top_srcdir@/src/binreloc \
	-I@top_srcdir@/src/enum \
	-I@top_srcdir@/src/framework \
	-I@top_srcdir@/src/mas \
	-I@top_srcdir@/src/mapgen \
	-I@top_srcdir@/src/model \
	-I@top_srcdir@/src/networking \
	-I@top_srcdir@/src/pngfuncs \
	-I@top_srcdir@/src/simulator \
	-I@top_srcdir@/src/structure \
	-I@top_srcdir@/src/tinyxml \
	-I@top_srcdir@/src/tinyxpath \
	-I@top_srcdir@/src/triangulation \
	-DDATADIR=\"@datadir@\" \
	-DSYSCONFDIR=\"@sysconfdir@\"


#==============================================================================
# The main binary executable.
# It's necessary to compile the other programs after the main binary.
#==============================================================================
bin_PROGRAMS = opencity


#==============================================================================
# The OpenCity Zen server.
# We compile it but we do not install it yet
#==============================================================================
noinst_PROGRAMS = oczen


#==============================================================================
# Main OpenCity executable
#==============================================================================
opencity_LDADD = \
	binreloc/libbinreloc.a \
	framework/libsharpplus.la \
	mapgen/libmapgen.a \
	mas/libmas.a \
	model/libmodel.a \
	networking/libnet.a \
	pngfuncs/libpngfuncs.a \
	simulator/libsim.a \
	structure/libstruct.a \
	tinyxml/libtinyxml.a \
	tinyxpath/libtinyxpath.a \
	triangulation/libtriangulation.a


if OPENCITY_SDL_MIXER
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
	extensionmanager.cpp \
	globalvar.cpp \
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
	property.cpp \
	propertymanager.cpp \
	propertymanager2.cpp \
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
	extensionmanager.h \
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
	property.h \
	propertymanager.h \
	propertymanager2.h \
	renderer.h \
	SimpleOpt.h \
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
	pngfuncs/libpngfuncs.a \
	simulator/libsim.a \
	structure/libstruct.a \
	tinyxml/libtinyxml.a \
	tinyxpath/libtinyxpath.a \
	triangulation/libtriangulation.a


oczen_SOURCES = \
	audiomanager.cpp \
	buildinglayer.cpp \
	city.cpp \
	conf.cpp \
	destination.cpp \
	globalvar.cpp \
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
	property.cpp \
	propertymanager.cpp \
	propertymanager2.cpp \
	texture.cpp \
	vehicle.cpp \
	zen.cpp

