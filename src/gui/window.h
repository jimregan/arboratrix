// -*- C++ -*-

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include "widget.h"

namespace acb_gui 
{
  /*
   * Clase:      Window
   * Hereda:     Widget 
   * Version:    1.0
   * Creada:     Wed Jan 30 10:55:29 CET 2002
   * Modificada: Wed Jan 30 10:55:29 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Clase que representa una ventana.
   */

  /*!
   * \class Window
   * \brief Clase que representa una ventana.
   */
  class Window : public Widget
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    enum Close_action {HIDE, DESTROY, IGNORE};

    //! Constructor sólo para ventanas (no dialogos).
    Window (GtkWindowType t=GTK_WINDOW_TOPLEVEL);
    //! Destructor.
    ~Window ();

    //! Cambia el titulo de la ventana.
    void set_title (const std::string& t);
    //! Icono para mostrar el gestor de ventanas.
    void set_icon (const char** xpm);

    //! Es relativa a...
    void set_transient_for (Window* w);
    //! Es relativa a...
    void set_transient_for (const Window& w);
    //! Impide que la ventana se pueda cerrar desde el WM
    void set_wm_close(Close_action a = IGNORE);
    //! Es visible?
    bool is_visible() { return GTK_WIDGET_VISIBLE(the_widget); }

    // Parte protegida.
  protected:
    //! Tipo de GtkWindow
    GtkWindowType wtype;

    /*________CALLBACKS_________*/
    /*! Callback a asociar si no queremos que la ventana se cierre
     * desde WM. */
    static gint wm_no_close(GtkWidget *widget, GdkEvent *event, gpointer data);

    /*! Callback a asociar si queremos que la ventana se destruya
     * desde WM. */
    static gint wm_do_close(GtkWidget *widget, GdkEvent *event, gpointer data);

    /*! Callback a asociar si queremos que la ventana se oculte
     * desde WM. */
    static gint wm_do_hide(GtkWidget *widget, GdkEvent *event, gpointer data);
    /*________CALLBACKS_________*/

    // Parte privada.
  private:
    /*! Signal handler para activar/desactivar botones del
     *  window-manager*/
    gulong sh;

  };

} // Fin namespace gui
#endif
