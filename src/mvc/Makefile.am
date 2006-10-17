#
# Makefile.am para la parte de interfaz.
#
current=1
revision=0
age=0

DEPDIR = @DEPDIR@
OBJEXT = @OBJEXT@

CXXFLAGS =
AM_CXXFLAGS = -foperator-names -Wall -g -pipe

##LDFLAGS = @GTK_LIBS@
#LIBTOOL = @LIBTOOL@ --tag CXX

lib_LTLIBRARIES = libacb_mvc.la

#
# Esta variable me permite generar automaticamente la cabecera acb_mvc.h
#
THE_DOT_H = model.h view.h

include_HEADERS = acb_mvc.h $(THE_DOT_H)

libacb_mvc_la_SOURCES = model.cc view.cc

libacb_mvc_la_LDFLAGS = -version-info $(current):$(revision):$(age)

all: libacb_mvc.la acb_mvc.h

acb_mvc.h : $(THE_DOT_H)
	@echo Regenerating acb_mvc.h
	@echo -e "// -*- C++ -*-" >acb_mvc.h
	@echo -e "// Created on: `date`\n" >>acb_mvc.h
	@echo -e "#ifndef _ACB_MVC_H_\n#define _ACB_MVC_H_\n" >>acb_mvc.h
	@echo -e "#ifdef HAVE_CONFIG_H\n  #include <sishitra_config.h>\n#endif\n" >> acb_mvc.h

	@for f in $(THE_DOT_H);\
	do\
	  echo -e "#include <mvc/$$f>" >>acb_mvc.h ;\
	done
	@echo -e "\n#endif\n" >>acb_mvc.h

CLEANFILES = *~
