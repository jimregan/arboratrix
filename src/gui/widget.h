// -*- C++ -*-

#ifndef _WIDGET_H_
#define _WIDGET_H_

#if HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <iostream>
#include <gtk/gtk.h>
#include <gdk/gdkcursor.h>
#include <string>

#include "object.h"

namespace acb_gui 
{
  /*
   * Clase:      Widget
   * Hereda:     De Object.
   * Version:    1.1
   * Creada:     Tue Jan 29 18:56:19 CET 2002
   * Modificada: Thu Jan 31 16:56:19 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Clase base de los elementos de i.u.
   */

  typedef GdkCursorType CursorType;
  typedef GdkColor  Color;

  /*!
   * \class Widget \brief Clase base de los elementos de i.u.
   *
   *  Clases derivadas de ella deben definir el metodo create_qui.
   */
  class Widget : public Object
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! \brief Constructor por defecto.
    Widget();
    //! Wrapper sobre un gtkwidget.
    Widget(GtkWidget* w) { the_widget = w; build(); }
    //! Destructor.
    virtual ~Widget();

    //! Destruye el widget de gtk interno
    virtual void destroy_widget();
    //! Muestra el widget
    virtual void show() { gtk_widget_show(the_widget); }
    //! Oculta el widget
    virtual void hide() { gtk_widget_hide(the_widget); }
    //! Desactivar
    virtual void disable() { gtk_widget_set_sensitive(the_widget, false); }
    //! Activar
    virtual void enable() { gtk_widget_set_sensitive(the_widget, true); }
    //! Borde interno
    virtual void set_border(int b)
    { 
      gtk_container_set_border_width(GTK_CONTAINER(the_widget), b);
    }

    //! Conversion al tipo de GtkWidget*
    operator GtkWidget*() { return the_widget; }
    //! Devuelve la ventana de GDk de este widget.
    GdkWindow* get_window() const { return the_widget->window; }
    //! Conversion explícita al tipo GtkWidget*
    GtkWidget* get_widget() const { return the_widget; }

    //! Busca un widget entre los campos de usuario.
    GtkWidget* lookup_widget(const std::string& n);

    //! Busca un widget entre los campos de usuario de un gtkwidget.
    static GtkWidget* lookup_widget(GtkWidget* widget,
				    const std::string& n);

    //! Devuelve el puntero this guardado en un gtkwidget.
    static Widget* get_this(GtkWidget* w);

    /*! Cambia el aspecto del cursor del raton sobre este widget.
     *
     * La forma del cursor se proporciona como un mapa de bits.
     */
    void change_mouse_shape(int w, int h, char mask[], char bits[]);

    /*! Cambia el aspecto del cursor del raton sobre este widget.
     *
     * La forma del cursor es una de las predefinidas.
     */
    void change_mouse_shape(CursorType ct);

    //! Cambia el color de fondo del widget.
    void set_background(Color& c)
    { 
      gdk_window_set_background(the_widget->window, &c);
    }

    // Parte protegida.
  protected:
    //! El widget interno de GTK usado.
    GtkWidget* the_widget;

    //! Recoge el código generado por glade en 'create_...'
    virtual GtkWidget* create_gui() { return the_widget; };

    /*! \brief Nos dice si el widget ha sido 'registrado'.  
     *
     * Registrar un objeto de clase Widget significa guardar el
     * puntero this en un campo de usuario del gtk-widget usado a bajo
     * nivel.
     */
    bool is_registered() { return registered; }

    //! Crea el interfaz y guarda puntero this en gtkobject.
    void build() { the_widget = create_gui(); save_this(); }
    //! Guarda el puntero this en el objeto gtk 'the_widget'
    void save_this();

    /*! Conecta callbacks para los siguientes eventos:
     * expose, button_press, button_release, motion.
     */
    virtual void connect_callbacks();

    //! Manejador del evento expose de alto nivel.
    virtual void expose(GdkEventExpose* event) {}
    //! Manejador del evento button_press de alto nivel.
    virtual void button_press(GdkEventButton* event) {}
    //! Manejador del evento button_release de alto nivel.
    virtual void button_release(GdkEventButton* event) {}
    //! Manejador del evento motion_notify de alto nivel.
    virtual void motion_notify(GdkEventMotion* event) {}
    //! Manejador del evento configure de alto nivel.
    virtual void configure(GdkEventConfigure* event) {}

    // Parte privada.
  private:
    //! Indica si el objeto gtk guarda el puntero this.
    bool registered;

    //! Manejador del evento expose de bajo nivel.
    static gboolean expose_event(GtkWidget* widget, GdkEventExpose* event,
				 gpointer data);
    //! Manejador del evento button_press de bajo nivel.
    static gboolean button_press_event(GtkWidget* widget, GdkEventButton* event,
				       gpointer data);
    //! Manejador del evento button_release de bajo nivel.
    static gboolean button_release_event(GtkWidget* widget, GdkEventButton* event,
					 gpointer data);
    //! Manejador del evento motion de bajo nivel.
    static gboolean motion_notify_event(GtkWidget* widget, GdkEventMotion* event,
					gpointer data);
    //! Manejador del evento configure de bajo nivel.
    static gboolean configure_event(GtkWidget* widget, GdkEventConfigure* event,
				    gpointer data);
  };

} // Fin namespace gui
#endif
