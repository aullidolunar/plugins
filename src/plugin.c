#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <geanyplugin.h>

GeanyPlugin     *geany_plugin;
GeanyData       *geany_data;
GeanyFunctions  *geany_functions;

PLUGIN_VERSION_CHECK( 211 )

PLUGIN_SET_TRANSLATABLE_INFO(
	LOCALEDIR,
	GETTEXT_PACKAGE,
	PACKAGE,
	_("Lost chars in your laptop"),
	PACKAGE_VERSION,
	SELFIE
);

GtkWidget *m_item = NULL;

void cb_any_item_activated (GtkMenuItem *menuitem, gpointer gdata) {
	GeanyDocument *doc = NULL;
	doc = document_get_current();
	if (doc != NULL) {
		const gchar* _char;
		gint pos;
		gint len;
		_char = gtk_menu_item_get_label (menuitem);
		len = g_utf8_strlen (_char, -1);
		pos = sci_get_current_position (doc->editor->sci);
		sci_insert_text (doc->editor->sci, -1, _char);
		sci_set_current_position (doc->editor->sci, pos+len, TRUE);
	}
}

void plugin_init( GeanyData *data )
{
	const gchar *_chars[] = {
		"<", ">", ">=", "<=", "->", "=>", NULL
	};
	gint index = 0;
	GtkWidget *hbox;
	GtkWidget *spacer;
	GtkWidget *subMenu;

#if (GTK_MAJOR_VERSION == 2)
	hbox = gtk_hbox_new (FALSE, 0);
	spacer = gtk_hbox_new (FALSE, 0);
#else
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	spacer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
#endif
/* looking for a portable way (gtk2 <-> gtk3) to add icons to menus */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	m_item = gtk_image_menu_item_new_with_label (_("Your lost chars"));
	ui_add_document_sensitive (m_item);
	if (g_file_test (ICON_PATH, G_FILE_TEST_IS_REGULAR)) {
		GdkPixbuf *nice_image;
		GtkWidget *widget_nice_image;
		nice_image = gdk_pixbuf_new_from_file_at_size (ICON_PATH, 16, 16, NULL);
		widget_nice_image = gtk_image_new_from_pixbuf (nice_image);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (m_item), widget_nice_image);
		g_object_unref (nice_image);
	}
#pragma GCC diagnostic pop	
	subMenu = gtk_menu_new ();
	
	while (_chars[index] != NULL) {
		GtkWidget *item;
		item = gtk_menu_item_new_with_label (_chars[index]);
		g_signal_connect (item, "activate", G_CALLBACK(cb_any_item_activated), NULL);
		gtk_menu_shell_append (GTK_MENU_SHELL (subMenu), item);
		index++;
	}
	
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (m_item), subMenu);
	gtk_container_add (GTK_CONTAINER(geany->main_widgets->tools_menu), m_item);
	gtk_widget_show_all (m_item);
	
}

void plugin_cleanup( void )
{
	gtk_widget_destroy (m_item);
}
