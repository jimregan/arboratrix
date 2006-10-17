// -*- C++ -*-

/*
 * Creado:     Thu Jan 31 16:30:17 CET 2002
 * Modificado: Thu Jan 31 16:30:17 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "application.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Application::Application(int& argc, char**& argv)
{
  gtk_set_locale ();
  gtk_init(&argc, &argv);
}

/*
 * Destructor.
 */
Application::~Application()
{

}
