// -*- C++ -*-

#ifndef _PROGRESS_DIALOG_H_
#define _PROGRESS_DIALOG_H_


#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <string>
#include "dialog.h"

#define _(x) x

namespace acb_gui {
  /*
   * Clase:      Progress_dialog
   * Hereda:     Dialog 
   * Version:    1.0
   * Creada:     Mon Jun 30 12:46:10 CEST 2003
   * Modificada: Mon Jun 30 12:46:10 CEST 2003
   * Autor:      Antonio-Miguel Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Representa una barra de progreso.
   */

  /*!
   * \class Progress_dialog
   * \brief Representa una barra de progreso.
   */
  class Progress_dialog : public Dialog
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! Constructor por defecto.
    Progress_dialog(const std::string& t);
    //! Destructor.
    ~Progress_dialog();

    //! De momento solo podemos 'pulsar' la barra.
    void pulse();
    //! Metodo invocado al 'aceptar' el dialogo.
    void accept() {}
    //! Metodo invocado al 'cancelar' el dialogo.
    void cancel() {}

    // Parte protegida.
  protected:
    //! Contiene el codigo generado por Glade.
    GtkWidget* create_gui();
    //! Guarda el widget barra_de_progreso del dialogo.
    GtkWidget* progressbar;

    // Parte privada.
  private:

  };

} // Fin namespace acb_gui
#endif
