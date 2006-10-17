// -*- C++ -*-

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _CANVAS_H_
#define _CANVAS_H_


#if HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#define _(x) x

#include <string>
#include <list>
#include <gui/acb_gui.h>

/*
 * Clase:      Canvas
 * Hereda:     Widget 
 * Version:    1.0
 * Creada:     Mon Dec 30 17:09:03 CET 2002
 * Modificada: Mon Dec 30 17:09:03 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Widget que representa un canvas.
 */

typedef GdkPoint     Point;
typedef GdkRectangle Rectangle;

class Sentence;
class Canvas_item;
class Text_item;
class Tagger_view;

/*!
 * \class Canvas
 * \brief Widget que representa un canvas.
 */
class Canvas : public acb_gui::Widget
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  typedef std::list<Canvas_item*> Itemlist;

  /*! Constructor por defecto.
   * \param wwindow La ventana 'madre' donde se muestra el canvas
   * \param cname El nombre del widget de gtk que es el canvas.
   */
  Canvas(Tagger_view* tv, const std::string& cname);
  //! Destructor.
  ~Canvas();

  //! Dibuja un punto en las coordenadas x, y y anchura w.
  void draw_point (int x, int y, int w = 1) const;
  //! Dibuja un punto 'p' y anchura w.
  void draw_point (const Point& p, int w = 1) const;
  //! Dibuja una linea desde x1,y1 hasta x2,y2.
  void draw_line(int x1, int y1, int x2, int y2) const;
  //! Cambia el modo de dibujo de las lineas a XOR.
  void set_line_mode_xor() const;
  //! Cambia el modo de dibujo de las lineas a COPY.
  void set_line_mode_copy() const;
  //! Cambia el trazo de linea a discontinuo
  void set_line_mode_on_off() const;
  //! Cambia el trazo de linea a continuo
  void set_line_mode_on() const;

  //! Dibuja un rectangulo no relleno.
  void draw_rectangle (const Rectangle& r, bool filled = false) const;
  //! Dibuja un texto str en coordenadas x, y.
  void draw_text(int  x, int y, const std::string& str) const;
  //! Dibuja un Text_item sobre el canvas.
  void draw_text_item(const Text_item* ti) const;
  //! Dibuja un arco inmerso en el rectangulo r y con angulos a1 y a2.
  void draw_arc (const Rectangle& r, gint a1, gint a2, bool filled = false) const;

  //! Reorganiza el arbol de la frase actual.
  void layout_tree();
  //! Fuerza el redibujado del canvas completo.
  void force_redraw() const;
  //! Fuerza el redibujado de un rectangulo del canvas.
  void force_redraw (const Rectangle& r);
  //! Fuerza el redibujado del rectangulo del Canvas_item 'ci'.
  void force_redraw(const Canvas_item* ci);
  //! Borra el contenido del canvas.
  void clear ();
  //! Deja el canvas sin frase y 'limpio'.
  void empty();

  //! Devuelve el Tagger_view al que pertenece el Canvas.
  Tagger_view* get_tagger_view() { return ctv; }

  /*! Recorre el arbol de items a partir de este y devuelve el item que
   * tiene dentro el punto de coordenadas 'p', si no, devuelve NULL.
   *
   * \param not_this Si es distinto de NULL indica que el item buscado
   * es distinto de este.*/
  Canvas_item* get_item_at(int x, int y, const Canvas_item* not_this = NULL);

  //! \sa Canvas_item* get_item_at(const Point &p, const Canvas_item* not_this = NULL)
  Canvas_item*
  get_item_at(const Point &p, 
	      const Canvas_item* not_this = NULL)
  {
    return get_item_at(p.x, p.y, not_this);
  }

  //! Inserta este canvas_item en la lista de items del canvas.
  void add_item(Canvas_item* child);
  //! Elimina este canvas_item de la lista de items del canvas.
  void remove_item(Canvas_item* child);
  //! Cambia la frase visualizada en el canvas.
  void set_sentence(Sentence* s);
  //! Devuelve la frase visualizada en el canvas.
  Sentence* get_sentence() { return cs; }
  //! Selecciona todos los items del canvas.
  void select_all_items();
  //! Deselecciona todos los items del canvas.
  void deselect_all_items();
  //! Invierte la seleccion de todos los items
  void invert_selection();
  //! Devuelve la cantidad de items seleccionados.
  int count_selected_items();
  //! Desplaza todos los items 'dx' y 'dy' pixels en el canvas
  void move_all_items(int dx, int dy);
  /*! Desplaza solo los items seleccionados 'dx' y 'dy' pixels en el
   * canvas. */
  void move_selected_items(int dx, int dy);
  /*! Ajusta la posicion de cada item respecto al ultimo click del
   * raton. */
  void set_xy_click_all_items(Point p);

  //! Devuelve la lista de items directamente  seleccionados
  Itemlist& get_selected_items();

  void set_gcs(GdkGC* lgc, GdkGC* rgc, GdkGC* tgc)
  { 
    line_gc = lgc; text_gc = tgc; rect_gc = rgc;
  }

  /*! Permite crear una traza entre dos items del canvas.  \param dest
   * El item destino. \param orig El item q arrastra el usuario.*/
  void create_trace(Text_item* dest, Text_item* orig);

  // Parte protegida.
protected:
  //! La frase visualizada en el canvas.
  Sentence* cs;
  /*! Lista de items visualizados en el canvas. Coincide con la de la
   * frase actual o Canvas::cs.*/
  Itemlist* cil;
  /*! Lista de items seleccionados y visualizados en el canvas. Debe
   * ser accedida por medio de get_selected_items() ya q asi se
   * construye al 'vuelo'.*/
  Itemlist cisl;
  //! Item seleccionado en el canvas.
  Canvas_item* cis;

  //! La ventana donde reside el canvas
  GtkWidget* parent_window;
  //! Contexto grafico general de las lineas y puntos.
  GdkGC* line_gc;
  //! Contexto grafico de los rectangulos.
  GdkGC* rect_gc;
  //! Contexto grafico del texto.
  GdkGC* text_gc;

  /*! El punto usado para realizar el scroll de los items
   * seleccionados.  Se fija al recibir el evento de boton pulsado y
   * se actualiza cada vez que se recibe uno de raton
   * movido. (scrollpoint) */
  Point sp;

  /*! Usada para saber si tras pulsar un boton del raton el usuario
   * inicio un desplazamiento o simplemente lo solto. */
  bool has_moved;

  /*! Acciones disponibles al seleccionar un item:
   *              SHIFT   ITEM    ACCION
   * kNothing:    true    false   Nada
   * kAppend:     true    true    Anyadir 'is' a la lista si no estaba
   * kOnlyThis:   false   true    Vaciar lista, añadir solo 'is'
   * kRemoveThis: true    true    Eliminar 'is' de la lista si ya estaba
   * kClearList:  false   false   Vaciar lista de items seleccionados. */
  enum Selection_action {kNothing, kAppend, 
                         kOnlyThis, kRemoveThis, kClearList, kAllItems};

  //! Accion elegida para el item recien seleccionado.
  Selection_action action;

  /*! Genera al vuelo la lista interna de items seleccionados. Esta
   * lista contiene los hijos no-seleccionados de un padre caso q este
   * lo este y recursive sea cierto.*/
  void compute_selected_items(bool recursive = true);

  //! Cambia de golpe toda la lista de items del canvas.
  void set_item_list(Itemlist* il) { cil = il; force_redraw(); }

  //! Crea los contextos graficos de este canvas.
  void create_gcs ();

  //! Crea las estructuras de datos para PANGO.
  void create_font_data(const std::string& font_name);

  //! Manejador del evento expose de alto nivel.
  virtual void expose(GdkEventExpose* event);
  //! Manejador del evento button_press de alto nivel.
  virtual void button_press(GdkEventButton* event);
  //! Manejador del evento button_release de alto nivel.
  virtual void button_release(GdkEventButton* event);
  //! Manejador del evento motion_notify de alto nivel.
  virtual void motion_notify(GdkEventMotion* event);
  //! Manejador del evento configure de alto nivel.
  virtual void configure(GdkEventConfigure* event);

  //! Gestiona la pulsacion del boton derecho del raton.
  void right_button_press(GdkEventButton* event);
  //! Gestiona la pulsacion del boton izquierdo del raton.
  void left_button_press(GdkEventButton* event);
  /*! Muestra informacion en la barra de estado cuando el usuario
   * intenta poner un hijo a un padre. El item que muestra la
   * informacion es el posible padre. \param shift es cierto si la
   * tecla shift esta pulsada.*/
  void show_feedback(bool shift, bool control);

  // Parte privada.
private:
  //! El objeto PangoLayout para pintar texto.
  PangoLayout* pl;
  //! El objeto PangoFontDescription para pintar texto.
  PangoFontDescription* pfd;
  //! El tagger_view que crea este canvas
  Tagger_view* ctv;
};

#endif
