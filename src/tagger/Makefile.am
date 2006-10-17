##
## Makefile para generar una aplicacion usando la libreria 'gui'.
## (c) A. Corbi, Alicante 2002.
##

EXTRA_DIST = db_new_delete.cc

XML2_CFLAGS = @XML2_CFLAGS@
XML2_LIBS = @XML2_LIBS@

##CXXFLAGS = -g -foperator-names -Wall -pedantic @GTK_CFLAGS@
#LIBTOOL = @LIBTOOL@ --tag CXX

## Usarla más adelante...: -DGTK_DISABLE_DEPRECATED
CXXFLAGS =
AM_CXXFLAGS = -g -foperator-names -Wall -pipe
#INCLUDES = -I$(top_srcdir)/src/gui @GTK_CFLAGS@ $(XML2_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/src @GTK_CFLAGS@ $(XML2_CFLAGS)
bin_PROGRAMS = arboratrix arboratrix.debug

##
## Para usar ElectricFence
##
#EXTRA_SOURCES =
#EXTRA_LIBS =
#EXTRA_SOURCES=db_new_delete.cc
#EXTRA_LIBS = -lefence
#EXTRA_LIBS = -lmudflap

DEPDIR = @DEPDIR@
OBJEXT = @OBJEXT@

arboratrix_SOURCES = $(EXTRA_SOURCES) \
		     arboratrix.cc app_icon.h pixmaps.h \
		     tagger_model.h tagger_model.cc \
		     tagger_view.h tagger_view.cc \
		     list_manager.h list_manager.cc \
		     sentence_list.h sentence_list.cc \
		     tag_list.h tag_list.cc \
		     canvas.h canvas.cc \
		     canvas_item.h canvas_item.cc \
		     text_model.h text_model.cc \
		     text_item.h text_item.cc \
		     sentence.h sentence.cc \
		     exporter.h \
		     tag_item.h tag_item.cc \
		     tag_model.h tag_model.cc \
		     tag_info.h tag_info.cc \
		     index_dialog.h index_dialog.cc \
		     utils.h utils.cc

arboratrix_LDADD = $(top_builddir)/src/gui/libacb_gui.la \
		   $(top_builddir)/src/mvc/libacb_mvc.la \
		   @GTK_LIBS@  $(XML2_LIBS) $(EXTRA_LIBS)

## Para depuracion
arboratrix_debug_SOURCES = $(arboratrix_SOURCES)
arboratrix_debug_LDADD = $(arboratrix_LDADD)
arboratrix_debug_LDFLAGS = -static

##
## Que mas borramos.
##
CLEANFILES = *~
#DISTCLEANFILES  = .deps/*.P
