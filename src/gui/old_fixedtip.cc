/* 
 * fixedtip.c
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
 * USA */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtkmain.h>
#include <gtk/gtkstyle.h>
#include <gtk/gtksignal.h>
#include "fixedtip.h"

static void gtk_fixedtip_class_init (GtkFixedtipClass *klass);
static void gtk_fixedtip_init (GtkFixedtip      *fixedtip);
static void gtk_fixedtip_destroy (GtkObject        *object);
static gint gtk_fixedtip_expose (GtkWidget *fixedtip, 
				 GdkEventExpose *event);
static void gtk_fixedtip_draw (GtkWidget* widget, GdkRectangle* area);
static GtkWindowClass *parent_class;

GtkType
gtk_fixedtip_get_type (void)
{
  static GtkType fixedtip_type = 0;
  if (!fixedtip_type)
    {
      static const GtkTypeInfo fixedtip_info =
	{
	  "GtkFixedtip",
	  sizeof (GtkFixedtip),
	  sizeof (GtkFixedtipClass),
	  (GtkClassInitFunc) gtk_fixedtip_class_init,
	  (GtkObjectInitFunc) gtk_fixedtip_init,
	  /* reserved_1 */ NULL,
	  /* reserved_2 */ NULL,
	  (GtkClassInitFunc) NULL,
	};
      fixedtip_type = gtk_type_unique (GTK_TYPE_WINDOW, &fixedtip_info);
    }
  return fixedtip_type;
}

static void
gtk_fixedtip_class_init (GtkFixedtipClass *klass)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  parent_class = reinterpret_cast<GtkWindowClass *>(gtk_type_class (GTK_TYPE_WINDOW));
  object_class = (GtkObjectClass*) klass;
  widget_class = (GtkWidgetClass*) klass;
  object_class->destroy = gtk_fixedtip_destroy;
  widget_class->expose_event = gtk_fixedtip_expose;
  widget_class->draw = gtk_fixedtip_draw;
}

static void
gtk_fixedtip_init (GtkFixedtip *fixedtip)
{
  GtkWindow* window = GTK_WINDOW(fixedtip);
  GtkWidget* widget = GTK_WIDGET(fixedtip);

  fixedtip->tip_text = NULL;
  fixedtip->width = 0;
  fixedtip->row = NULL;
  window->type = GTK_WINDOW_POPUP;
  gtk_widget_set_app_paintable(widget, TRUE);
  gtk_window_set_policy (window, FALSE, FALSE, TRUE);
  gtk_widget_set_name (widget, "gtk-fixedtip");
}

GtkWidget *
gtk_fixedtip_new (void)
{
  GtkFixedtip* fixedtip;
  GtkWindow* window;

  fixedtip = reinterpret_cast<GtkFixedtip*>(gtk_type_new (GTK_TYPE_FIXEDTIP));
  window = GTK_WINDOW(fixedtip);
  return GTK_WIDGET(fixedtip);
}

static void
gtk_fixedtip_free_string (gpointer data, gpointer user_data)
{
  if (data)
    g_free (data);
}

static void
gtk_fixedtip_cleanup_strings (GtkFixedtip *fixedtip)
{
  g_free (fixedtip->tip_text);
  fixedtip->tip_text = NULL;
  if (fixedtip->row)
    {
      g_list_foreach (fixedtip->row, gtk_fixedtip_free_string, 0);
      g_list_free (fixedtip->row);
      fixedtip->row = NULL;
    }
}

static void
gtk_fixedtip_destroy (GtkObject *object)
{
  GtkFixedtip *fixedtip = GTK_FIXEDTIP (object);

  g_return_if_fail (fixedtip != NULL);
  gtk_fixedtip_cleanup_strings(fixedtip);
  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    (* GTK_OBJECT_CLASS(parent_class)->destroy)(object);
}

static void
gtk_fixedtip_layout_text (GtkFixedtip *fixedtip)
{
  GtkWidget* widget;
  gchar *row_end, *text, *row_text, *break_pos;
  gint i, row_width, window_width = 0;
  size_t len;

  widget = GTK_WIDGET(fixedtip);
  if (fixedtip->row)
    {
      g_list_foreach (fixedtip->row, gtk_fixedtip_free_string, 0);
      g_list_free (fixedtip->row);
    }
  fixedtip->row = NULL;
  fixedtip->width = 0;
  text = fixedtip->tip_text;
  if (text == NULL)
    return;
  while (*text)
    {
      row_end = strchr (text, '\n');
      if (!row_end)
	row_end = strchr (text, '\0');
      len = row_end - text + 1;
      row_text = g_new(gchar, len);
      memcpy (row_text, text, len - 1);
      row_text[len - 1] = '\0';
      /* now either adjust the window's width or shorten the row until
	 it fits in the window */
      while (TRUE)
	{
	  row_width = gdk_string_width (widget->style->font, row_text);
	  if (!window_width)
	    {
	      /* make an initial guess at window's width: */
	      if (row_width > gdk_screen_width () / 4)
		window_width = gdk_screen_width () / 4;
	      else
		window_width = row_width;
	    }
	  if (row_width <= window_width)
	    break;
#if 0 // I want to only break on newlines for now
	  if (strchr (row_text, ' '))
	    {
	      /* the row is currently too wide, but we have blanks in
		 the row so we can break it into smaller pieces */
	      gint avg_width = row_width / strlen (row_text);
	      i = window_width;
	      if (avg_width)
		i /= avg_width;
	      if ((size_t) i >= len)
		i = len - 1;
	      break_pos = strchr (row_text + i, ' ');
	      if (!break_pos)
		{
		  break_pos = row_text + i;
		  while (*--break_pos != ' ');
		}
	      *break_pos = '\0';
	    }
	  else
#endif
	    {
	      /* we can't break this row into any smaller pieces, so
		 we have no choice but to widen the window: */
	      window_width = row_width;
	      break;
	    }
	}
      if (row_width > fixedtip->width)
	fixedtip->width = row_width;
      fixedtip->row = g_list_append (fixedtip->row, row_text);
      text += strlen (row_text);
      if (!*text)
	break;
      if (text[0] == '\n' && text[1])
	/* end of paragraph and there is more text to come */
	fixedtip->row = g_list_append (fixedtip->row, 0);
      ++text;  /* skip blank or newline */
    }
  fixedtip->width += 8;    /* leave some border */
}

void
gtk_fixedtip_set_text (GtkFixedtip *fixedtip,
		       const gchar *tip_text)
{
  g_return_if_fail (fixedtip != NULL);
  g_return_if_fail (GTK_IS_FIXEDTIP (fixedtip));
  gtk_fixedtip_cleanup_strings(fixedtip);
  if (tip_text != NULL)
    fixedtip->tip_text = g_strdup(tip_text);
  gtk_fixedtip_layout_text(fixedtip);
  if (GTK_WIDGET_VISIBLE(fixedtip))
    gtk_widget_queue_draw(GTK_WIDGET(fixedtip));
}

static void
gtk_fixedtip_paint (GtkWidget* widget)
{
  GtkStyle *style;
  gint y, baseline_skip, gap;
  GList *el = NULL;
  GtkFixedtip* fixedtip;

  fixedtip = GTK_FIXEDTIP(widget);
  style = widget->style;
  gap = (style->font->ascent + style->font->descent) / 4;
  if (gap < 2)
    gap = 2;
  baseline_skip = style->font->ascent + style->font->descent + gap;
  gtk_paint_flat_box(style, widget->window,
		     GTK_STATE_NORMAL, GTK_SHADOW_OUT, 
		     NULL, widget, "tooltip",
		     0, 0, -1, -1);
  y = style->font->ascent + 4;
  for (el = fixedtip->row; el; el = el->next)
    {
      if (el->data)
	{
	  gtk_paint_string (style, widget->window,
			    GTK_STATE_NORMAL, 
			    NULL, widget, "tooltip",
			    4, y, reinterpret_cast<const gchar*>(el->data));
	  y += baseline_skip;
	}
      else
	y += baseline_skip / 2;
    }
}

static gint
gtk_fixedtip_expose (GtkWidget *widget, GdkEventExpose *event)
{
  gtk_fixedtip_paint(widget);
  return FALSE;
}

static void
gtk_fixedtip_draw (GtkWidget* widget, GdkRectangle* area)
{
  gtk_fixedtip_paint(widget);
}

/* Should be OK to call this if the widget is already visible. */
void             
gtk_fixedtip_popup (GtkFixedtip* fixedtip, gint x, gint y, 
		    GdkWindow* relative_to)
{
  GtkStyle *style;
  gint gap, w, h, scr_w, scr_h, baseline_skip, ox, oy;
  GList *el;

  g_return_if_fail(relative_to != NULL);
  style = GTK_WIDGET(fixedtip)->style;
  scr_w = gdk_screen_width ();
  scr_h = gdk_screen_height ();
  gap = (style->font->ascent + style->font->descent) / 4;
  if (gap < 2)
    gap = 2;
  baseline_skip = style->font->ascent + style->font->descent + gap;
  w = fixedtip->width;
  h = 8 - gap;
  for (el = fixedtip->row; el; el = el->next)
    if (el->data)
      h += baseline_skip;
    else
      h += baseline_skip / 2;
  if (h < 8)
    h = 8;
  gdk_window_get_origin (relative_to, &ox, &oy);
  gtk_widget_set_usize (GTK_WIDGET(fixedtip), w, h);
  if (!GTK_WIDGET_VISIBLE(GTK_WIDGET(fixedtip)))
    gtk_widget_popup (GTK_WIDGET(fixedtip), ox + x, oy + y);
  else 
    gtk_widget_set_uposition(GTK_WIDGET(fixedtip), ox + x, oy + y);
}

void             
gtk_fixedtip_popup_widget (GtkFixedtip* ft, gint x, gint y, 
			   GtkWidget* relative_to)
{
  if (GTK_WIDGET_NO_WINDOW (relative_to))
    {
      g_return_if_fail(relative_to->parent != NULL);
      g_return_if_fail(GTK_WIDGET_REALIZED(relative_to->parent));
      y += relative_to->allocation.y;
      gtk_fixedtip_popup (ft, x, y, relative_to->parent->window);
    }
  else
    {
      g_return_if_fail(GTK_WIDGET_REALIZED(relative_to));
      gtk_fixedtip_popup (ft, x, y, relative_to->window);
    }
}
