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

#include "pitivi.h"
#include "pitivi-menu.h"
#include "pitivi-menu-actions.h"

enum {
  ES_MENUBAR_ACTIVATE_SIGNAL = 1,
  ES_MENUBAR_LAST_SIGNAL
 };

enum {
  PITIVI_WINDOW_PROPERTY = 1,
  PITIVI_FILE_MENU_PROPERTY,
  PITIVI_ACTIONS_MENU_PROPERTY,
  PITIVI_MENU_DESCRIPTION_PROPERTY,
  PITIVI_LAST_ENUM_MENU,
};


struct  _PitiviMenuPrivate
{    
  /* instance private members */

  GtkWidget		*window;
  GtkUIManager		*ui_manager;
  guint			merge_id;
  gchar			*filename;
  gchar			*ui_description;
  
  GList			*action_group;
  GtkActionEntry	*action_entries;
  GtkToggleActionEntry	*action_toggle;
  GtkRadioActionEntry	*action_radio;
  
  GtkAccelGroup		*accel_group;
  gboolean		dispose_has_run;
  
};


/*
 * forward definitions
 */

/*
 * Insert "added-value" functions here
 */

PitiviMenu *
pitivi_menu_new(GtkWidget *window, gchar *fname)
{
  PitiviMenu	*menu;

  menu = PITIVI_MENU (g_object_new(PITIVI_MENU_TYPE, "window"\
				   , GTK_WIDGET(window), "filename", fname, NULL));
  g_assert(menu != NULL);
  return menu;
}

static GObject *
pitivi_menu_constructor (GType type,
			     guint n_construct_properties,
			     GObjectConstructParam * construct_properties)
{
  GObject *obj;
  {
    /* Invoke parent constructor. */
    PitiviMenuClass *klass;
    GObjectClass *parent_class;
    klass = PITIVI_MENU_CLASS (g_type_class_peek (PITIVI_MENU_TYPE));
    parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));
    obj = parent_class->constructor (type, n_construct_properties,
				     construct_properties);
  }
  return obj;
}

void
pitivi_get_uimanager()
{
  
}

static void
pitivi_menu_instance_init (GTypeInstance * instance, gpointer g_class)
{
  PitiviMenu *self = (PitiviMenu *) instance;
  
  self->private = g_new0(PitiviMenuPrivate, 1);
  self->public  = g_new0(PitiviMenuPublic, 1);
  
  /* initialize all public and private members to reasonable default values. */ 
  
  self->private->dispose_has_run = FALSE;
  self->private->ui_manager = gtk_ui_manager_new ();
  self->public->ui = self->private->ui_manager;
  self->public->menu = gtk_ui_manager_get_widget (self->private->ui_manager, g_strdup (PITIVI_MAIN_MENUBAR_XML));
  gtk_ui_manager_set_add_tearoffs (self->private->ui_manager, TRUE);
  self->private->action_group = gtk_ui_manager_get_action_groups (self->private->ui_manager);
  self->private->accel_group = gtk_ui_manager_get_accel_group (self->private->ui_manager);
  
  /* If you need specific consruction properties to complete initialization, 
   * delay initialization completion until the property is set. 
   */
  
  gtk_ui_manager_ensure_update (self->private->ui_manager);
}

static void
pitivi_menu_dispose (GObject *object)
{
  PitiviMenu	*self = PITIVI_MENU(object);

  /* If dispose did already run, return. */
  if (self->private->dispose_has_run)
    return;
  
  /* Make sure dispose does not run twice. */
  self->private->dispose_has_run = TRUE;	
  
  /* 
   * In dispose, you are supposed to free all types referenced from this 
   * object which might themselves hold a reference to self. Generally, 
   * the most simple solution is to unref all members on which you own a 
   * reference. 
   */

}

static void
pitivi_menu_finalize (GObject *object)
{
  PitiviMenu	*self = PITIVI_MENU(object);

  /* 
   * Here, complete object destruction. 
   * You might not need to do much... 
   */

  g_free (self->private);
}

static void
pitivi_set_menu_window (PitiviMenu *menu, GtkWidget *widget)
{
  if (GTK_IS_WINDOW(widget))
    { menu->private->window = widget;}
}


static void
pitivi_menu_set_property (GObject * object,
			      guint property_id,
			      const GValue * value, GParamSpec * pspec)
{
  PitiviMenu *self = (PitiviMenu *) object;

  switch (property_id)
    {
    case PITIVI_FILE_MENU_PROPERTY:
      pitivi_menu_set_filename (self, g_value_get_string (value));
      break;
    case PITIVI_WINDOW_PROPERTY:
      pitivi_set_menu_window (self, g_value_get_object (value));
      break;
    default:
      g_assert (FALSE);
      break;
    }
}

void
pitivi_menu_configure (PitiviMenu *self)
{
  GError *error;
  
  error = NULL;
  PitiviMenuPrivate *priv = self->private;
  if (priv->merge_id)
    gtk_ui_manager_remove_ui(priv->ui_manager, priv->merge_id);
  pitivi_menubar_configure (priv->ui_manager, priv);
  if ((priv->merge_id = gtk_ui_manager_add_ui_from_file (priv->ui_manager, \
							 priv->filename, &error)))
    {
      priv->ui_description = gtk_ui_manager_get_ui (priv->ui_manager);
      self->public->menu = gtk_ui_manager_get_widget (priv->ui_manager, PITIVI_MAIN_MENUBAR_XML);
      gtk_ui_manager_ensure_update (priv->ui_manager);
    }
  else
    {
      g_message ("building menus failed: %s", error->message);
      exit (0);
    }
}


void
pitivi_menu_set_filename (PitiviMenu *self, const gchar *filename)
{
  PitiviMenuPrivate *priv;
  
  priv = self->private;
  if (self)
    {
      if (!priv->filename)
	g_free (priv->filename);
      priv->filename = g_strdup(filename);
    }
}

static void
pitivi_menu_get_property (GObject * object,
			      guint property_id,
			      GValue * value, GParamSpec * pspec)
{
  PitiviMenu *self = (PitiviMenu *) object;

  switch (property_id)
    {
     case PITIVI_FILE_MENU_PROPERTY:
      g_value_set_object (value, self->private->filename);
      break;
    case PITIVI_MENU_DESCRIPTION_PROPERTY:
      g_value_set_object (value, self->private->ui_description);
      break;
    case PITIVI_ACTIONS_MENU_PROPERTY:
      g_value_set_object (value, self->private->action_entries);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
pitivi_menu_class_init (gpointer g_class, gpointer g_class_data)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);
  PitiviMenuClass *klass = PITIVI_MENU_CLASS (g_class);

  gobject_class->constructor = pitivi_menu_constructor;
  gobject_class->dispose = pitivi_menu_dispose;
  gobject_class->finalize = pitivi_menu_finalize;

  gobject_class->set_property = pitivi_menu_set_property;
  gobject_class->get_property = pitivi_menu_get_property;

  g_object_class_install_property
    (gobject_class,
     PITIVI_FILE_MENU_PROPERTY,
     g_param_spec_string ("filename",
			  "Filename",
			  "Filename xml description file ui",
			  NULL,
			  (G_PARAM_READABLE|G_PARAM_WRITABLE)));
  
  g_object_class_install_property (gobject_class,
				   PITIVI_WINDOW_PROPERTY,
				   g_param_spec_object ("window",
							"Window",
							"Window container for menubar.",
							gtk_window_get_type(), G_PARAM_READWRITE));
  klass->public = g_new0 (PitiviMenuClassPublic, 1);
  klass->public->configure = pitivi_menu_configure;
}

GType
pitivi_menu_get_type (void)
{
  static GType type = 0;
 
  if (type == 0)
    {
      static const GTypeInfo info = {
	sizeof (PitiviMenuClass),
	NULL,			/* base_init */
	NULL,			/* base_finalize */
	pitivi_menu_class_init,	/* class_init */
	NULL,			/* class_finalize */
	NULL,			/* class_data */
	sizeof (PitiviMenu),
	0,			/* n_preallocs */
	pitivi_menu_instance_init	/* instance_init */
      };
      type = g_type_register_static (GTK_TYPE_WIDGET,
				     "PitiviMenuType", &info, 0);
    }

  return type;
}
