// -*- C++ -*-

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <gtk/gtk.h>
#include "object.h"

namespace acb_gui 
{
  /*
   * Clase:      Application
   * Hereda:     Object 
   * Version:    1.0
   * Creada:     Thu Jan 31 16:30:17 CET 2002
   * Modificada: Thu Jan 31 16:30:17 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Clase que representa a la aplicacion.
   */

  /*!
   * \class Application
   * \brief Clase que representa a la aplicacion.
   */
  class Application : public Object
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! Constructor por defecto.
    Application(int& argc, char**& argv);

    //! Destructor.
    ~Application();

    virtual void run () { gtk_main (); }
    virtual void end_loop () { gtk_main_quit (); }
    virtual int  end () { return 0; } 

    // Parte protegida.
  protected:

    // Parte privada.
  private:

  };

} // Fin namespace gui
#endif
