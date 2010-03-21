#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(arboratrix, 2.0.0-pre20, acorbi@dlsi.ua.es)

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([src/gui/widget.h])
AC_CONFIG_HEADER(sishitra_config.h)
#AC_CONFIG_HEADERS(sishitra_config.h)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

# Set the test language to C++.
##AC_LANG([C++])
#AC_LANG_PUSH([C++])
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
#AC_LANG_POP
dnl AC_REPLACE_FUNCS

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT

AC_ISC_POSIX
dnl AC_ARG_ENABLE(g3, [  --enable-g3             Usa el nuevo compilador.])
dnl if test $enable_g3; then
dnl   CXX=g++-3.2
dnl else
dnl   CXX=g++
dnl fi

dnl Compilador por defecto, necesitamos un 3.x.y
CXX=g++

if test ! -x /usr/bin/g++-3.3
then
  if test -x /usr/bin/g++-3.2
  then
    CXX=g++-3.2
  else
    CXX=g++
  fi
fi
AC_SUBST(CXX)

##AM_PROG_CC_STDC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

AM_PATH_GTK_2_0(2.0.0,,AC_MSG_ERROR(Cannot find GTK-2: Is gtk-config inpath?))
#AM_PATH_GTK(1.2.10,,AC_MSG_ERROR(Cannot find GTK: Is gtk-config inpath?))


# Checks for libraries.
AC_CHECK_LIB(xml2, xmlNewDoc,,)
XML2_LIBS=`xml2-config --libs`
AC_SUBST(XML2_LIBS)

XML2_CFLAGS=`xml2-config --cflags`
AC_SUBST(XML2_CFLAGS)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_STRUCT_TM
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr])

## Chequeos mios
dnl Conversiones de tipo nuevas
acb_CXX_NEW_CASTS
dnl Soporte de STL
acb_CXX_STL

dnl ***************************************************
dnl Necesitaremos compilador de Latex
dnl ***************************************************
AC_PATH_PROG(LATEX,latex)
if test x$ac_cv_path_LATEX = x
then
   AC_MSG_ERROR([No tienes LaTeX instalado, no podre generar Postscript.])
fi

dnl Compilacion latex
acb_LATEX_IS_GOOD

dnl Buscamos dvipdfm
dnl AC_MSG_CHECKING(Si esta instalado dvipdfm)
AC_PATH_PROG(DVIPDFM,dvipdfm)
if test x$ac_cv_path_DVIPDFM = x
then
   AC_MSG_ERROR([No tienes dvipdfm instalado, no podre generar PDFs.])
fi


## Defines mios
AC_DEFINE_UNQUOTED(DATE,"`date`",[Fecha de compilacion])
AC_DEFINE_UNQUOTED(ABSTRACT,= 0,[Declaracion de funciones virtuales puras])

## Ficheros emitidos
AC_CONFIG_FILES([Makefile \
		 src/Makefile \
                 src/gui/Makefile \
                 src/mvc/Makefile \
                 src/tagger/Makefile])


AC_CONFIG_COMMANDS_PRE([
echo
echo "·-------------------------· ·------------------------·"
echo -n "| Configuracion terminada | | AC_PACKAGE_NAME-AC_PACKAGE_VERSION | "
echo " CXX = $CXX"
echo "·-------------------------· ·------------------------·"
echo
])


AC_OUTPUT
