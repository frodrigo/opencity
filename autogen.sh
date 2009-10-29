#!/bin/bash
# Note: we don't use "autothings" prior to 1.7
# $Id$

aclocal
libtoolize -c
autoconf
automake -a -c

