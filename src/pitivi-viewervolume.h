/* 
 * PiTiVi
 * Copyright (C) <2004> Edward G. Hervey <hervey_e@epita.fr>
 *                      Guillaume Casanova <casano_g@epita.fr>
 *
 * This software has been written in EPITECH <http://www.epitech.net>
 * EPITECH is a computer science school in Paris - FRANCE -
 * under the direction of Flavien Astraud and Jerome Landrieu.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef PITIVI_VIEWERVOLUME_H
#define PITIVI_VIEWERVOLUME_H

/*
 * Potentially, include other headers on which this header depends.
 */

#include <gtk/gtk.h>
#include "pitivi-stockicons.h"

/*
 * Type macros.
 */

#define PITIVI_VIEWERVOLUME_TYPE (pitivi_viewervolume_get_type ())
#define PITIVI_VIEWERVOLUME(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PITIVI_VIEWERVOLUME_TYPE, PitiviViewerVolume))
#define PITIVI_VIEWERVOLUME_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PITIVI_VIEWERVOLUME_TYPE, PitiviViewerVolumeClass))
#define PITIVI_IS_VIEWERVOLUME(obj) (G_TYPE_CHECK_TYPE ((obj), PITIVI_VIEWERVOLUME_TYPE))
#define PITIVI_IS_VIEWERVOLUME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PITIVI_VIEWERVOLUME_TYPE))
#define PITIVI_VIEWERVOLUME_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PITIVI_VIEWERVOLUME_TYPE, PitiviViewerVolumeClass))

typedef struct _PitiviViewerVolume PitiviViewerVolume;
typedef struct _PitiviViewerVolumeClass PitiviViewerVolumeClass;
typedef struct _PitiviViewerVolumePrivate PitiviViewerVolumePrivate;
typedef struct _PitiviViewerVolumePublic PitiviViewerVolumePublic;

enum {
  PITIVI_STATE_VOLIMG_MAX = 0,
  PITIVI_STATE_VOLIMG_MEDIUM,
  PITIVI_STATE_VOLIMG_MIN,
  PITIVI_STATE_VOLIMG_ZERO,
  PITIVI_STATE_VOLIMG_ALL
};

struct _PitiviViewerVolumePublic
{
  GtkWidget		*state_vol[PITIVI_STATE_VOLIMG_ALL];
  GtkWidget		*adjust_vol;
};

struct _PitiviViewerVolume
{
  GtkWindow parent;

  /* instance public members */
  PitiviViewerVolumePublic *public;
  /* private */
  PitiviViewerVolumePrivate *private;
};

struct _PitiviViewerVolumeClass
{
  GtkWindowClass parent;
  /* class members */
};

/* used by PITIVI_VIEWERVOLUME_TYPE */
GType pitivi_viewervolume_get_type (void);

/*
 * Method definitions.
 */

PitiviViewerVolume	*pitivi_viewervolume_new(void);

int
pitivi_viewervolume_cb_button_released          (GtkWidget *toggle_button,
						 GdkEventButton * event,
						 gpointer  user_data);
void
pitivi_viewervolume_cb_button_clicked (GtkWidget *volume_button, gpointer user_data);

#endif
