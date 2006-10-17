// -*- C++ -*-

#ifndef _INFO_DIALOG_H_
#define _INFO_DIALOG_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <string>
#include "dialog.h"

#define _(x) x

namespace acb_gui 
{
  /*
   * Clase:      Info_dialog
   * Hereda:     Gui::Dialog 
   * Version:    1.0
   * Creada:     Wed May 15 17:02:45 CEST 2002
   * Modificada: Wed May 15 17:02:45 CEST 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Representa un dialogo informativo.
   */

  /*!
   * \class Info_dialog
   * \brief Representa un dialogo informativo.
   */
  class Info_dialog : public Dialog
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    /*! Constructor por defecto.
     * \param t El titulo de la ventana.
     * \param q La pregunta que se hace al usuario. */
    Info_dialog(const std::string& t, const std::string& q);

    //! Destructor.
    ~Info_dialog();

    //! Antes de mostrar debemos crear un bucle local
    virtual void show();
    //! Metodo invocado al 'aceptar' el dialogo.
    virtual void accept();
    //! Metodo invocado al 'cancelar' el dialogo.
    virtual void cancel();

    //! Nos dice si el usuario respondio SI.
    bool is_accepted() { return accepted; }

    // Parte protegida.
  protected:
    //! Contiene el codigo generado por Glade.
    GtkWidget* create_gui();
    //! El widget que muestra la pregunta hecha al usuario.
    GtkWidget* text;
    //! Indica si se respondio SI a la pregunta del dialgo.
    bool accepted;

    //! Titulo en formato utf8
    gchar* utf8_t;
    //! Mensaje en formato utf8
    gchar* utf8_m;

    // Parte privada.
  private:

  };
} // Fin namespace gui
#endif
