##########################################################################
# Fichero con macros particulares.
# (c) Antonio Corbi 2000, 2001, 2002
# Iniciado para comprobaciones relacionadas con C++, y ampliado con cosas
# de Latex y demas...
##########################################################################

#
# Comprueba si LaTeX es bueno o no...
#
AC_DEFUN([acb_LATEX_IS_GOOD],
[
AC_MSG_CHECKING(Si LaTeX es lo bastante bueno)
changequote(`, ')
echo "-------------comprobando latex-----------" 1>&5
cat > conftest.tex <<EOF 
\documentclass[a4paper]{foils}
\usepackage[latin1]{inputenc}
\usepackage{fancyheadings}
\usepackage[dvips]{graphicx}
\usepackage[hyperindex]{hyperref}

% Babel y acentos
\usepackage[spanish]{babel}
\usepackage[latin1]{inputenc}
\newcommand{\imagen}[3]
{
  \begin{figure}[ht!]
    \begin{center}
      \fbox{\includegraphics[scale=#3]{#1}}
    \end{center}
  \end{figure}
}

\begin{document}
\end{document}
EOF
dnl
changequote([,])
if($LATEX '\nonstopmode\input{conftest.tex}' 2>&1 1>&5)
then
   echo "-------------latex test successful!--------" 1>&5
   AC_MSG_RESULT([eso parece])
else 
   AC_MSG_RESULT([no, consulta el fichero config.log])
   AC_MSG_ERROR([la version de LaTeX instalada no soporta todo lo necesario.])
fi
rm -f conftest.tex conftest.log conftest.aux texput.log
])

# ---------------------------------------------------

#
# Comprobamos las nuevas conversiones de tipo de C++
#

AC_DEFUN([acb_CXX_NEW_CASTS],
[
AC_MSG_CHECKING(Si el compilador de C++ tiene las nuevas conversiones de tipo)
AC_LANG_PUSH(C++)
AC_COMPILE_IFELSE([int i = static_cast<int>(2.5);],
  [AC_MSG_RESULT([si])],
  [AC_MSG_ERROR([Lo siento, necesitas un compilador de C++ mejor.])])
AC_LANG_POP(C++)
])

#
# Comprobamos el soporte de STL
#

AC_DEFUN([acb_CXX_STL],
[
AC_MSG_CHECKING(Si el compilador de C++ soporta STL)
AC_LANG_PUSH(C++)
AC_LINK_IFELSE(
[
#include <vector>

template <class T>
class Matriz
{
private:
  std::vector< std::vector<T> > la_matriz;
public:
  Matriz(int);
};

template <class T>
Matriz<T>::Matriz(int h) : la_matriz(h) {}

int main(void)
{
  Matriz<int> me = Matriz<int>(10);
}
],
[AC_MSG_RESULT([si])],
[AC_MSG_ERROR([Lo siento, necesitas un compilador de C++ mejor.])])
AC_LANG_POP(C++)
])
