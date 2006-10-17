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

#ifndef _CANVAS_ITEM_H_
#define _CANVAS_ITEM_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <list>
#include <gdk/gdk.h>
#include <mvc/view.h>

/*
 * Clase:      Canvas_item
 * Hereda:     View 
 * Version:    1.0
 * Creada:     Thu Jan  9 18:11:13 CET 2003
 * Modificada: Thu Jan  9 18:11:13 CET 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  La clase base de todos los items del canvas.
 */

class Canvas;
typedef GdkRectangle Rectangle;
typedef GdkPoint     Point;

/*!
 * \class Canvas_item
 * \brief La clase base de todos los items del canvas.
 * Se trata de una clase abstracta de la que derivan las clases de los
 * elementos que se pueden pintar en el canvas.
 */
class Canvas_item : public acb_mvc::View
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  /*! Constructor por defecto.
   * \param m El modelo para esta vista.
   * \param c El canvas donde se muestra esta vista.
   * \param auto_add Si es cierto este item se 'auto-añade' a la lista
   * de canvas-items q tiene el canvas 'c'.
   */
  Canvas_item(acb_mvc::Model& m, const Canvas* c, bool auto_add = false);
  //! Destructor.
  ~Canvas_item();

  void set_x(int px) { r.x = px; }
  void set_y(int py) { r.y = py; }
  void set_w(int pw) { r.width = pw; }
  void set_h(int ph) { r.height = ph; }
  int  get_x() const { return r.x; }
  int  get_y() const { return r.y; }
  int  get_h() const { return r.height; }
  int  get_w() const { return r.width; }
  void set_rect(Rectangle pr) { r = pr; }
  Rectangle get_rect() const { return r; }
  int get_shadow_dimension() { return shadow_delta; }

  /* Guarda la coordenada 'x' actual del item. Se emplea antes de
   * comenzar el arrastre del item para darle un nuevo padre y asi
   * mantener su posicion 'x' relativa al resto de sus 'nuevos
   * hermanos'.*/
  void save_x_coord() { save_x = r.x; }
  /* Restaura la coordenada 'x' guardada del item. Se emplea despues
   * de realizar el arrastre del item para darle un nuevo padre y asi
   * mantener su posicion 'x' relativa al resto de sus 'nuevos
   * hermanos'.*/
  void restore_x_coord() { r.x = save_x; }

  //! Actualizacion de un Canvas_item pedida desde el modelo.
  virtual void update() ABSTRACT;
  //! Visualización de un Canvas_item.
  virtual void draw() ABSTRACT;

  /*! Muestra las caracteristicas del item en un tooltip.  Este
   * tooltip se visualizará en las coordenadas globales 'x' e 'y'.*/
  virtual void show_properties(int x, int y) ABSTRACT;
  /*! Indica al usuario caracteristicas relevantes cuando se intenta
    'dejar' otro item sobre el.*/
  virtual void show_feedback_for(Canvas_item* down_item, 
				 bool shift, bool control) ABSTRACT;

  //! Aplica un offset-x al rectangulo de este item.
  void offset_by_x(int ox) { r.x += ox; }

  void set_hidden(bool b) { hidden = b; }
  bool is_hidden() { return hidden; }
  void set_selected(bool b) { selected = b; clear_from_canvas(); draw(); }
  void toggle_selected() { selected = not selected; clear_from_canvas(); draw(); }
  bool is_selected() { return selected; }

  //! Nos dice si este item contiene un punto 'p'.
  bool is_point_inside(const Point& p) { return is_point_inside(p.x, p.y); }
  //! Nos dice si este item contiene un punto 'p'.
  bool is_point_inside(int x, int y);
  //! Nos dice si intersecta con este otro canvas item.
  bool intersects_with(const Canvas_item* ci);

  //! Cambio del ultimo click del raton sobre este item.
  void set_xy_click(int x, int y) { dxc = x-r.x; dyc = y-r.y; }

  //! Nos dice si este item se puede mover con el ratón
  bool is_draggable() { return draggable; }
  //! Mueve a las coordenadas (x,y) este item.
  virtual void mouse_drag(int x, int y, bool force_drag = false);

  //! Borra este item solo del canvas.
  virtual void clear_from_canvas(bool generate_event = false);
  // Parte protegida.

  /*! Se usa para ordenar dos nodos por su coordenada 'x' de menor 'x'
   * a mayor. Se emplea en la ordenacion de una lista de
   * Canvas_item. */
  static bool citem_less(const Canvas_item* c1, const Canvas_item* c2);

  /*! Comparacion 'menor' con otro canvas_item respecto a su
   * coordenada 'x'.*/
  bool operator< (const Canvas_item &ci) const;

  //! Suministra al canvas los valores de sus GCs
  void set_canvas_gcs();

  //! Indica si el item se puede borrar.
  virtual bool is_erasable() ABSTRACT;

 /*! Ayuda en la creacion de la lista de items seleccionados,
   * incluyendo a sus hijos (si los tiene) de manera recursiva aunque
   * no esten seleccionados.*/
  virtual void compute_selected_items(std::list<Canvas_item*>& cisl) ABSTRACT;

protected:
  //! Delta empleado en el calculo de la sombra del rectangulo.
  const int shadow_delta;
  //! El canvas al q pertenece este item.
  const Canvas* canvas;
  //! El rectangulo q engloba al item.
  Rectangle r;
  //! Rectangulo sombra 1
  Rectangle rs1;
  //! Rectangulo sombra 2
  Rectangle rs2;
  //! Contexto grafico del texto.
  GdkGC* text_gc;
  //! Contexto grafico del rectangulo.
  GdkGC* rect_gc;
  //! Contexto grafico de las lineas y puntos.
  GdkGC* line_gc;

  //! Se puede mover este item?
  bool draggable;
  //! Indica si el item esta seleccionado o no.
  bool selected;
  //! Si queremos que esté oculto este item, por defecto NO lo esta.
  bool hidden;
  //! Delta ultimo x_click del raton, origen
  int dxc;
  //! Delta ultimo y_click del raton, origen
  int dyc;
  //! Anterior coordenada x antes de comenzar a moverse.
  int save_x;

  //! Crea los contextos gráficos. Solo le reserva la memoria.
  void create_pens();
  //! Asigna los colores a los GCs. Cada subclase debe redefinirla.
  virtual void set_colors() ABSTRACT;
  //! Crea la sombra de este item cuando esta seleccionado.
  void compute_shadow();
  //! Calcula y dibuja la sombra de este item.
  void draw_shadow();
  // Parte privada.
private:

};

#endif
