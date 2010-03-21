// -*- C++ -*-

#ifndef _FIXED_TIP_H_
#define _FIXED_TIP_H_

#include "widget.h"

namespace acb_gui 
{
  /*
   * Clase:      Fixed_tip
   * Hereda:     Widget 
   * Version:    1.0
   * Creada:     Mon Dec  2 16:28:03 CET 2002
   * Modificada: Mon Dec  2 16:28:03 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Crea un tooltip en la posición solicitada.
   */

  /*!
   * \class Fixed_tip
   * \brief Crea un tooltip en la posición solicitada.
   */
  class Fixed_tip : public Widget
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! Constructor por defecto.
    Fixed_tip();

    //! Destructor.
    ~Fixed_tip();

    /*! Muestra el tooltip 't' en (x,y).  \param tout Indica si es >0
     * el tiempo para un timeout.*/
    void show(int x, int y, const std::string& t, int tout = 0);

    // Parte protegida.
  protected:

    //! Recoge el código generado por glade en 'create_...'
    virtual GtkWidget* create_gui();
  
    //! Pinta el tool-tip.
    static bool expose_handler(GtkTooltip *tooltips, GdkEvent *event,
			       gpointer data);
    //! Auto-elimina el tool-tip.
    static gint tip_remover(gpointer data);

    // Parte privada.
  private:
    GtkWidget *label;
    int screen_width;
    int screen_height;
    int screen_number;
  };

} // Fin namespace gui

#endif
