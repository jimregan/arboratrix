// -*- C++ -*-

#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <gdk/gdkkeysyms.h>
#include "window.h"
#include "glade2.h"

namespace acb_gui 
{
  /*
   * Clase:      Dialog
   * Hereda:     Window 
   * Version:    1.0
   * Creada:     Wed Jan 30 14:20:04 CET 2002
   * Modificada: Wed Jan 30 14:20:04 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Clase que representa un dialogo.
   */

  /*!
   * \class Dialog
   * \brief Clase que representa un dialogo.
   */
  class Dialog : public Window
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! Constructor por defecto.
    Dialog();

    //! Destructor.
    ~Dialog();

    //! Atributo modal.
    void set_modal(bool m) { gtk_window_set_modal (GTK_WINDOW(the_widget), m); }

    //! Metodo invocado al 'aceptar' el dialogo.
    virtual void accept() ABSTRACT;
    //! Metodo invocado al 'cancelar' el dialogo.
    virtual void cancel() ABSTRACT;

    /*! Permite asociar las teclas ENTER y ESC a los botones de
     * aceptar y cancelar de un dialogo cuando el foco lo tiene un
     * widget de texto editable.  \param wn Es el nombre del widget de
     * texto editable dentro del dialogo.*/
    virtual void catch_enter_esc_keys (const std::string& wn);

    //! Asocia el callback de pulsar ESC al dialogo
    void catch_esc_key ();
   
    // Parte protegida.
  protected:

    //-------CALLBACKS--------
    //! Metodo de clase llamado para salida 'OK'.
    static void dialog_accepted (GtkWidget *widget, gpointer data);
    //! Metodo de clase llamado para salida 'CANCEL'.
    static void dialog_cancelled (GtkWidget *widget, gpointer data);
    //! Metodo de clase util para capturar Enter o Esc en widgets entry
    static bool key_pressed(GtkWidget *w, GdkEventKey* e, gpointer p);
    //! Captura solo la pulsacion de ESC sobre el dialogo para cancelar
    static bool esc_key_pressed(GtkWidget *w, GdkEventKey* e, gpointer p);
    //-------CALLBACKS--------

    // Parte privada.
  private:
  };

} // Fin namespace gui
#endif
