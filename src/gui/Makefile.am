#
# Makefile.am para la parte de interfaz.
#
current=1
revision=0
age=0

DEPDIR = @DEPDIR@
OBJEXT = @OBJEXT@

EXTRA_DIST = old_fixedtip.cc old_fixedtip.h

AM_CXXFLAGS = -foperator-names \
              -Wall -g @GTK_CFLAGS@ \
	      -DG_DISABLE_DEPRECATED \
	      -DGDK_DISABLE_DEPRECATED \
	      -DGTK_DISABLE_DEPRECATED \
	      -pipe

##LDFLAGS = @GTK_LIBS@
#LIBTOOL = @LIBTOOL@ --tag CXX

lib_LTLIBRARIES = libacb_gui.la

#
# Esta variable me permite generar automaticamente la cabecera acb_gui.h
#
THE_DOT_H = glade2.h object.h application.h \
	    widget.h window.h dialog.h yes_no_dialog.h \
	    fixed_tip.h info_dialog.h progress_dialog.h

include_HEADERS = acb_gui.h $(THE_DOT_H)

libacb_gui_la_SOURCES = object.cc application.cc \
			widget.cc window.cc dialog.cc \
			yes_no_dialog.cc fixed_tip.cc \
			info_dialog.cc progress_dialog.cc

libacb_gui_la_LDFLAGS = -version-info $(current):$(revision):$(age)

all: libacb_gui.la acb_gui.h

acb_gui.h : $(THE_DOT_H)
	@echo Regenerating acb_gui.h
	@echo -e "// -*- C++ -*-" >acb_gui.h
	@echo -e "// Created on: `date`\n" >>acb_gui.h
	@echo -e "#ifndef _ACB_GUI_H_\n#define _ACB_GUI_H_\n" >>acb_gui.h
	@echo -e "#ifdef HAVE_CONFIG_H\n  #include <sishitra_config.h>\n#endif\n" >> acb_gui.h

	@for f in $(THE_DOT_H);\
	do\
	  echo -e "#include <gui/$$f>" >>acb_gui.h ;\
	done
	@echo -e "\n#endif\n" >>acb_gui.h


CLEANFILES = *~
