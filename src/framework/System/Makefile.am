#==============================================================================
# Makefile.am for libSystem.la
# $Id$
#==============================================================================
SUBDIRS = Information Reflection

AM_CPPFLAGS = -I@top_srcdir@/src/framework

noinst_LTLIBRARIES = libSystem.la

libSystem_la_SOURCES = \
	CConsole.cpp \
	CNullValue.cpp \
	CObject.cpp \
	CString.cpp \
	CType.cpp \
	CValueType.cpp

noinst_HEADERS = \
	CConsole.h \
	CNullValue.h \
	CObject.h \
	CString.h \
	CType.h \
	CValueType.h

libSystem_la_LIBADD = \
	Information/libInformation.la \
	Reflection/libReflection.la

