/* geanypluginlegacy.c generated by valac 0.24.0, the Vala compiler
 * generated from geanypluginlegacy.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <geanyplugin2.h>
#include <gmodule.h>


#define GEANY_TYPE_PLUGIN_LEGACY (geany_plugin_legacy_get_type ())
#define GEANY_PLUGIN_LEGACY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN_LEGACY, GeanyPluginLegacy))
#define GEANY_PLUGIN_LEGACY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GEANY_TYPE_PLUGIN_LEGACY, GeanyPluginLegacyClass))
#define GEANY_IS_PLUGIN_LEGACY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEANY_TYPE_PLUGIN_LEGACY))
#define GEANY_IS_PLUGIN_LEGACY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GEANY_TYPE_PLUGIN_LEGACY))
#define GEANY_PLUGIN_LEGACY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GEANY_TYPE_PLUGIN_LEGACY, GeanyPluginLegacyClass))

typedef struct _GeanyPluginLegacy GeanyPluginLegacy;
typedef struct _GeanyPluginLegacyClass GeanyPluginLegacyClass;
typedef struct _GeanyPluginLegacyPrivate GeanyPluginLegacyPrivate;
#define _g_module_close0(var) ((var == NULL) ? NULL : (var = (g_module_close (var), NULL)))

typedef void (*GeanyDelegateInit) (void* geanydata);
typedef gint (*GeanyDelegateVersionCheck) (gint abi);
typedef GtkWidget* (*GeanyDelegateConfigure) (GtkDialog* parent);
typedef void (*GeanyDelegateConfigureSingle) (GtkWidget* parent);
typedef void (*GeanyDelegateHelp) ();
typedef void (*GeanyDelegateCleanup) ();
struct _GeanyPluginLegacy {
	GObject parent_instance;
	GeanyPluginLegacyPrivate * priv;
	GeanyDelegateInit _init;
	GeanyDelegateVersionCheck _version_check;
	GeanyDelegateConfigure _configure;
	GeanyDelegateConfigureSingle _configure_single;
	GeanyDelegateHelp _help;
	GeanyDelegateCleanup _cleanup;
	GModule* module;
};

struct _GeanyPluginLegacyClass {
	GObjectClass parent_class;
};


static gpointer geany_plugin_legacy_parent_class = NULL;
static GeanyPlugin2Iface* geany_plugin_legacy_geany_plugin2_parent_iface = NULL;

GType geany_plugin_legacy_get_type (void) G_GNUC_CONST;
enum  {
	GEANY_PLUGIN_LEGACY_DUMMY_PROPERTY,
	GEANY_PLUGIN_LEGACY_METHODS
};
static void geany_plugin_legacy_real_initialize (GeanyPlugin2* base);
static gint geany_plugin_legacy_real_version_check (GeanyPlugin2* base, gint abi_ver);
static GtkWidget* geany_plugin_legacy_real_configure (GeanyPlugin2* base, GtkDialog* parent);
static void geany_plugin_legacy_real_configure_single (GeanyPlugin2* base, GtkWidget* parent);
static void geany_plugin_legacy_real_help (GeanyPlugin2* base);
static void geany_plugin_legacy_real_cleanup (GeanyPlugin2* base);
GeanyPluginLegacy* geany_plugin_legacy_new (void);
GeanyPluginLegacy* geany_plugin_legacy_construct (GType object_type);
static void geany_plugin_legacy_finalize (GObject* obj);
static void _vala_geany_plugin_legacy_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);


static void geany_plugin_legacy_real_initialize (GeanyPlugin2* base) {
	GeanyPluginLegacy * self;
	void* geanydata = NULL;
	GModule* _tmp0_ = NULL;
	void* _tmp1_ = NULL;
	GeanyDelegateInit _tmp2_ = NULL;
	self = (GeanyPluginLegacy*) base;
	_tmp0_ = self->module;
	g_module_symbol (_tmp0_, "geany_data", &_tmp1_);
	geanydata = _tmp1_;
	_tmp2_ = self->_init;
	_tmp2_ (geanydata);
}


static gint geany_plugin_legacy_real_version_check (GeanyPlugin2* base, gint abi_ver) {
	GeanyPluginLegacy * self;
	gint result = 0;
	GeanyDelegateVersionCheck _tmp0_ = NULL;
	gint _tmp1_ = 0;
	gint _tmp2_ = 0;
	self = (GeanyPluginLegacy*) base;
	_tmp0_ = self->_version_check;
	_tmp1_ = abi_ver;
	_tmp2_ = _tmp0_ (_tmp1_);
	result = _tmp2_;
	return result;
}


static GtkWidget* geany_plugin_legacy_real_configure (GeanyPlugin2* base, GtkDialog* parent) {
	GeanyPluginLegacy * self;
	GtkWidget* result = NULL;
	GeanyDelegateConfigure _tmp0_ = NULL;
	GtkDialog* _tmp1_ = NULL;
	GtkWidget* _tmp2_ = NULL;
	self = (GeanyPluginLegacy*) base;
	g_return_val_if_fail (parent != NULL, NULL);
	_tmp0_ = self->_configure;
	_tmp1_ = parent;
	_tmp2_ = _tmp0_ (_tmp1_);
	result = _tmp2_;
	return result;
}


static void geany_plugin_legacy_real_configure_single (GeanyPlugin2* base, GtkWidget* parent) {
	GeanyPluginLegacy * self;
	GeanyDelegateConfigureSingle _tmp0_ = NULL;
	GtkWidget* _tmp1_ = NULL;
	self = (GeanyPluginLegacy*) base;
	g_return_if_fail (parent != NULL);
	_tmp0_ = self->_configure_single;
	_tmp1_ = parent;
	_tmp0_ (_tmp1_);
}


static void geany_plugin_legacy_real_help (GeanyPlugin2* base) {
	GeanyPluginLegacy * self;
	GeanyDelegateHelp _tmp0_ = NULL;
	self = (GeanyPluginLegacy*) base;
	_tmp0_ = self->_help;
	_tmp0_ ();
}


static void geany_plugin_legacy_real_cleanup (GeanyPlugin2* base) {
	GeanyPluginLegacy * self;
	GeanyDelegateCleanup _tmp0_ = NULL;
	self = (GeanyPluginLegacy*) base;
	_tmp0_ = self->_cleanup;
	_tmp0_ ();
}


GeanyPluginLegacy* geany_plugin_legacy_construct (GType object_type) {
	GeanyPluginLegacy * self = NULL;
	self = (GeanyPluginLegacy*) g_object_new (object_type, NULL);
	return self;
}


GeanyPluginLegacy* geany_plugin_legacy_new (void) {
	return geany_plugin_legacy_construct (GEANY_TYPE_PLUGIN_LEGACY);
}


static gint geany_plugin_legacy_real_get_methods (GeanyPlugin2* base) {
	gint result;
	GeanyPluginLegacy* self;
	gint ret = 0;
	GeanyDelegateConfigure _tmp0_ = NULL;
	GeanyDelegateConfigureSingle _tmp2_ = NULL;
	GeanyDelegateHelp _tmp4_ = NULL;
	GeanyDelegateCleanup _tmp6_ = NULL;
	gint _tmp8_ = 0;
	self = (GeanyPluginLegacy*) base;
	ret = 0;
	_tmp0_ = self->_configure;
	if (_tmp0_ != NULL) {
		gint _tmp1_ = 0;
		_tmp1_ = ret;
		ret = _tmp1_ | GEANY_METHODS_CONFIGURE;
	}
	_tmp2_ = self->_configure_single;
	if (_tmp2_ != NULL) {
		gint _tmp3_ = 0;
		_tmp3_ = ret;
		ret = _tmp3_ | GEANY_METHODS_CONFIGURE_SINGLE;
	}
	_tmp4_ = self->_help;
	if (_tmp4_ != NULL) {
		gint _tmp5_ = 0;
		_tmp5_ = ret;
		ret = _tmp5_ | GEANY_METHODS_HELP;
	}
	_tmp6_ = self->_cleanup;
	if (_tmp6_ != NULL) {
		gint _tmp7_ = 0;
		_tmp7_ = ret;
		ret = _tmp7_ | GEANY_METHODS_CLEANUP;
	}
	_tmp8_ = ret;
	result = _tmp8_;
	return result;
}


static void geany_plugin_legacy_class_init (GeanyPluginLegacyClass * klass) {
	geany_plugin_legacy_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = _vala_geany_plugin_legacy_get_property;
	G_OBJECT_CLASS (klass)->finalize = geany_plugin_legacy_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_PLUGIN_LEGACY_METHODS, g_param_spec_int ("methods", "methods", "methods", G_MININT, G_MAXINT, 0, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
}


static void geany_plugin_legacy_geany_plugin2_interface_init (GeanyPlugin2Iface * iface) {
	geany_plugin_legacy_geany_plugin2_parent_iface = g_type_interface_peek_parent (iface);
	iface->initialize = (void (*)(GeanyPlugin2*)) geany_plugin_legacy_real_initialize;
	iface->version_check = (gint (*)(GeanyPlugin2*, gint)) geany_plugin_legacy_real_version_check;
	iface->configure = (GtkWidget* (*)(GeanyPlugin2*, GtkDialog*)) geany_plugin_legacy_real_configure;
	iface->configure_single = (void (*)(GeanyPlugin2*, GtkWidget*)) geany_plugin_legacy_real_configure_single;
	iface->help = (void (*)(GeanyPlugin2*)) geany_plugin_legacy_real_help;
	iface->cleanup = (void (*)(GeanyPlugin2*)) geany_plugin_legacy_real_cleanup;
	iface->get_methods = geany_plugin_legacy_real_get_methods;
}


static void geany_plugin_legacy_instance_init (GeanyPluginLegacy * self) {
}


static void geany_plugin_legacy_finalize (GObject* obj) {
	GeanyPluginLegacy * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (obj, GEANY_TYPE_PLUGIN_LEGACY, GeanyPluginLegacy);
	_g_module_close0 (self->module);
	G_OBJECT_CLASS (geany_plugin_legacy_parent_class)->finalize (obj);
}


GType geany_plugin_legacy_get_type (void) {
	static volatile gsize geany_plugin_legacy_type_id__volatile = 0;
	if (g_once_init_enter (&geany_plugin_legacy_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (GeanyPluginLegacyClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) geany_plugin_legacy_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GeanyPluginLegacy), 0, (GInstanceInitFunc) geany_plugin_legacy_instance_init, NULL };
		static const GInterfaceInfo geany_plugin2_info = { (GInterfaceInitFunc) geany_plugin_legacy_geany_plugin2_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		GType geany_plugin_legacy_type_id;
		geany_plugin_legacy_type_id = g_type_register_static (G_TYPE_OBJECT, "GeanyPluginLegacy", &g_define_type_info, 0);
		g_type_add_interface_static (geany_plugin_legacy_type_id, GEANY_TYPE_PLUGIN2, &geany_plugin2_info);
		g_once_init_leave (&geany_plugin_legacy_type_id__volatile, geany_plugin_legacy_type_id);
	}
	return geany_plugin_legacy_type_id__volatile;
}


static void _vala_geany_plugin_legacy_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	GeanyPluginLegacy * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (object, GEANY_TYPE_PLUGIN_LEGACY, GeanyPluginLegacy);
	switch (property_id) {
		case GEANY_PLUGIN_LEGACY_METHODS:
		g_value_set_int (value, geany_plugin2_get_methods ((GeanyPlugin2*) self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


