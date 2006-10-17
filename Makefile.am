# -*- makefile -*-
# Makefile general para el proyecto etiquetador
# (c) A. Corbi. Elda, diciembre 2001
#

PACKAGE = @PACKAGE@
VERSION = @VERSION@
AMTAR = tar

.PHONY : doc

#
# Generamos documentacion
#
doc :
	doxygen

SUBDIRS = src

EXTRA_DIST = bootstrap arboratrix.glade arboratrix2.glade arboratrix2.gladep \
	     Doxyfile config/acinclude.m4 TODO

CLEANFILES = *~ doc

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
