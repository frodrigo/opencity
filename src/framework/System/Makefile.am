#==============================================================================
# Makefile.am for libSystem.la
# $Id$
#==============================================================================
SUBDIRS = Information Reflection

AM_CPPFLAGS = -I@top_srcdir@/src/framework

noinst_LTLIBRARIES = libSystem.la

libSystem_la_SOURCES = \
	AValueType.cpp \
	CConsole.cpp \
	CNullValue.cpp \
	CObject.cpp \
	CString.cpp \
	CType.cpp


noinst_HEADERS = \
	AValueType.h \
	CConsole.h \
	CNullValue.h \
	CObject.h \
	CString.h \
	CType.h

libSystem_la_LIBADD = \
	Information/libInformation.la \
	Reflection/libReflection.la

