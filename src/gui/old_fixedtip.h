/* 
 * fixedtip.h
 *
 * Copyright (C) 1998 EMC Capital Management, Inc. and the Gtk Team
 *
 * Developed by Havoc Pennington <hp@pobox.com> based on GtkTooltips
 * in the Gtk distribution.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */
/* This is a tooltip-like popup window that isn't on a timer; you position 
 * it manually with respect to some GdkWindow or GtkWidget. 
 * For example, you might want to display information about the point under the 
 * pointer in a plot.
 */
#ifndef __GTK_FIXEDTIP_H__
#define __GTK_FIXEDTIP_H__
#include <gdk/gdk.h>
#include <gtk/gtkwindow.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_TYPE_FIXEDTIP (gtk_fixedtip_get_type ())
#define GTK_FIXEDTIP(obj) (GTK_CHECK_CAST ((obj), GTK_TYPE_FIXEDTIP, GtkFixedtip))
#define GTK_FIXEDTIP_CLASS(klass) (GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_FIXEDTIP, GtkFixedtipClass))
#define GTK_IS_FIXEDTIP(obj) (GTK_CHECK_TYPE ((obj), GTK_TYPE_FIXEDTIP))
#define GTK_IS_FIXEDTIP_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FIXEDTIP))

  typedef struct _GtkFixedtip      GtkFixedtip;
  typedef struct _GtkFixedtipClass GtkFixedtipClass;
  
  struct _GtkFixedtip
  {
    GtkWindow gtkwindow;
    /* Text to display */
    gchar *tip_text;
    /* Tip width */
    gint width;
    GList *row;
  };

  struct _GtkFixedtipClass
  {
    GtkWindowClass parent_class;
  };

  GtkType          gtk_fixedtip_get_type (void);
  GtkWidget*       gtk_fixedtip_new (void);
  void             gtk_fixedtip_set_text (GtkFixedtip* ft, const gchar* text);
  void             gtk_fixedtip_popup (GtkFixedtip* ft, gint x, 
				       gint y, GdkWindow* relative_to);
  void             gtk_fixedtip_popup_widget (GtkFixedtip* ft, gint x, gint y,
					      GtkWidget* relative_to);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __GTK_FIXEDTIP_H__ */
