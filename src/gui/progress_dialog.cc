// -*- C++ -*-

/*
 * Creado:     Mon Jun 30 12:46:10 CEST 2003
 * Modificado: Mon Jun 30 12:46:10 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "progress_dialog.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Progress_dialog::Progress_dialog(const std::string& t)
{
  build(); // crea interfaz y salva this

  progressbar = lookup_widget("progressbar");
  gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(progressbar), 0.01);

  set_title(t);
  //gtk_window_set_title(GTK_WINDOW(the_widget), utf8_t);
  set_wm_close(IGNORE);
}

/*
 * Destructor.
 */
Progress_dialog::~Progress_dialog()
{
  gtk_widget_destroy(the_widget);
}

void Progress_dialog::pulse() 
{ 
  for (int i = 0; i < 5; i++)
    {
      gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));
      while (gtk_main_iteration_do(FALSE));
    }
}

GtkWidget*
Progress_dialog::create_gui(void)
{
  GtkWidget *progress_dialog;
  GtkWidget *alignment1;
  GtkWidget *progressbar;

  progress_dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (progress_dialog), _("Activity in progress..."));
  gtk_window_set_position (GTK_WINDOW (progress_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_modal (GTK_WINDOW (progress_dialog), TRUE);
  gtk_window_set_resizable (GTK_WINDOW (progress_dialog), FALSE);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (progress_dialog), alignment1);

  progressbar = gtk_progress_bar_new ();
  gtk_widget_show (progressbar);
  gtk_container_add (GTK_CONTAINER (alignment1), progressbar);
  gtk_widget_set_size_request (progressbar, 260, -1);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (progress_dialog, progress_dialog, "progress_dialog");
  GLADE_HOOKUP_OBJECT (progress_dialog, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (progress_dialog, progressbar, "progressbar");

  return progress_dialog;
}
