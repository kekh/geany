/* filetype.c generated by valac 0.22.1, the Vala compiler
 * generated from filetype.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include "geanyapi.h"
#include <filetypes.h>
#include <stdlib.h>
#include <string.h>

#define _g_hash_table_unref0(var) ((var == NULL) ? NULL : (var = (g_hash_table_unref (var), NULL)))

struct _GeanyFileTypePrivate {
	struct GeanyFiletype* ft;
};

struct _GeanyFileTypeManagerPrivate {
	GHashTable* fts;
};


static gpointer geany_file_type_parent_class = NULL;
static gpointer geany_file_type_manager_parent_class = NULL;
static GeanyFileTypeManager* geany_file_type_manager_inst;
static GeanyFileTypeManager* geany_file_type_manager_inst = NULL;

#define GEANY_FILE_TYPE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GEANY_TYPE_FILE_TYPE, GeanyFileTypePrivate))
enum  {
	GEANY_FILE_TYPE_DUMMY_PROPERTY,
	GEANY_FILE_TYPE_EXTENSION,
	GEANY_FILE_TYPE_ID,
	GEANY_FILE_TYPE_LANGUAGE,
	GEANY_FILE_TYPE_NAME,
	GEANY_FILE_TYPE_PATTERN,
	GEANY_FILE_TYPE_TITLE
};
GeanyFileType* geany_file_type_new (struct GeanyFiletype* ftype);
GeanyFileType* geany_file_type_construct (GType object_type, struct GeanyFiletype* ftype);
static void geany_file_type_finalize (GObject* obj);
static void _vala_geany_file_type_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);
#define GEANY_FILE_TYPE_MANAGER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GEANY_TYPE_FILE_TYPE_MANAGER, GeanyFileTypeManagerPrivate))
enum  {
	GEANY_FILE_TYPE_MANAGER_DUMMY_PROPERTY
};
static void _g_object_unref0_ (gpointer var);
static GeanyFileTypeManager* geany_file_type_manager_new (void);
static GeanyFileTypeManager* geany_file_type_manager_construct (GType object_type);
GeanyFileType* geany_file_type_manager_lookup_ft (GeanyFileTypeManager* self, struct GeanyFiletype* gft);
static void geany_file_type_manager_finalize (GObject* obj);
static gint _vala_array_length (gpointer array);


GeanyFileType* geany_file_type_construct (GType object_type, struct GeanyFiletype* ftype) {
	GeanyFileType * self = NULL;
	struct GeanyFiletype* _tmp0_ = NULL;
	self = (GeanyFileType*) g_object_new (object_type, NULL);
	_tmp0_ = ftype;
	if (_tmp0_ == NULL) {
		struct GeanyFiletype* _tmp1_ = NULL;
		_tmp1_ = filetypes_index ((gint) GEANY_FILETYPES_NONE);
		self->priv->ft = _tmp1_;
	} else {
		struct GeanyFiletype* _tmp2_ = NULL;
		_tmp2_ = ftype;
		self->priv->ft = _tmp2_;
	}
	return self;
}


GeanyFileType* geany_file_type_new (struct GeanyFiletype* ftype) {
	return geany_file_type_construct (GEANY_TYPE_FILE_TYPE, ftype);
}


const gchar* geany_file_type_get_extension (GeanyFileType* self) {
	const gchar* result;
	struct GeanyFiletype* _tmp0_ = NULL;
	const gchar* _tmp1_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->extension;
	result = _tmp1_;
	return result;
}


gint geany_file_type_get_id (GeanyFileType* self) {
	gint result;
	struct GeanyFiletype* _tmp0_ = NULL;
	filetype_id _tmp1_ = 0;
	g_return_val_if_fail (self != NULL, 0);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->id;
	result = (gint) _tmp1_;
	return result;
}


gint geany_file_type_get_language (GeanyFileType* self) {
	gint result;
	struct GeanyFiletype* _tmp0_ = NULL;
	langType _tmp1_ = 0;
	g_return_val_if_fail (self != NULL, 0);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->lang;
	result = (gint) _tmp1_;
	return result;
}


const gchar* geany_file_type_get_name (GeanyFileType* self) {
	const gchar* result;
	struct GeanyFiletype* _tmp0_ = NULL;
	const gchar* _tmp1_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->name;
	result = _tmp1_;
	return result;
}


gchar** geany_file_type_get_pattern (GeanyFileType* self, int* result_length1) {
	gchar** result;
	struct GeanyFiletype* _tmp0_ = NULL;
	gchar** _tmp1_ = NULL;
	gint _tmp1__length1 = 0;
	gchar** _tmp2_ = NULL;
	gint _tmp2__length1 = 0;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->pattern;
	_tmp1__length1 = _vala_array_length (_tmp0_->pattern);
	_tmp2_ = _tmp1_;
	_tmp2__length1 = _tmp1__length1;
	if (result_length1) {
		*result_length1 = _tmp2__length1;
	}
	result = _tmp2_;
	return result;
}


const gchar* geany_file_type_get_title (GeanyFileType* self) {
	const gchar* result;
	struct GeanyFiletype* _tmp0_ = NULL;
	const gchar* _tmp1_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = self->priv->ft;
	_tmp1_ = _tmp0_->title;
	result = _tmp1_;
	return result;
}


static void geany_file_type_class_init (GeanyFileTypeClass * klass) {
	geany_file_type_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GeanyFileTypePrivate));
	G_OBJECT_CLASS (klass)->get_property = _vala_geany_file_type_get_property;
	G_OBJECT_CLASS (klass)->finalize = geany_file_type_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_EXTENSION, g_param_spec_string ("extension", "extension", "extension", NULL, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_ID, g_param_spec_int ("id", "id", "id", G_MININT, G_MAXINT, 0, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_LANGUAGE, g_param_spec_int ("language", "language", "language", G_MININT, G_MAXINT, 0, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_NAME, g_param_spec_string ("name", "name", "name", NULL, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_PATTERN, g_param_spec_boxed ("pattern", "pattern", "pattern", G_TYPE_STRV, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_object_class_install_property (G_OBJECT_CLASS (klass), GEANY_FILE_TYPE_TITLE, g_param_spec_string ("title", "title", "title", NULL, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
}


static void geany_file_type_instance_init (GeanyFileType * self) {
	self->priv = GEANY_FILE_TYPE_GET_PRIVATE (self);
}


static void geany_file_type_finalize (GObject* obj) {
	GeanyFileType * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (obj, GEANY_TYPE_FILE_TYPE, GeanyFileType);
	G_OBJECT_CLASS (geany_file_type_parent_class)->finalize (obj);
}


GType geany_file_type_get_type (void) {
	static volatile gsize geany_file_type_type_id__volatile = 0;
	if (g_once_init_enter (&geany_file_type_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (GeanyFileTypeClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) geany_file_type_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GeanyFileType), 0, (GInstanceInitFunc) geany_file_type_instance_init, NULL };
		GType geany_file_type_type_id;
		geany_file_type_type_id = g_type_register_static (G_TYPE_OBJECT, "GeanyFileType", &g_define_type_info, 0);
		g_once_init_leave (&geany_file_type_type_id__volatile, geany_file_type_type_id);
	}
	return geany_file_type_type_id__volatile;
}


static void _vala_geany_file_type_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	GeanyFileType * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (object, GEANY_TYPE_FILE_TYPE, GeanyFileType);
	switch (property_id) {
		case GEANY_FILE_TYPE_EXTENSION:
		g_value_set_string (value, geany_file_type_get_extension (self));
		break;
		case GEANY_FILE_TYPE_ID:
		g_value_set_int (value, geany_file_type_get_id (self));
		break;
		case GEANY_FILE_TYPE_LANGUAGE:
		g_value_set_int (value, geany_file_type_get_language (self));
		break;
		case GEANY_FILE_TYPE_NAME:
		g_value_set_string (value, geany_file_type_get_name (self));
		break;
		case GEANY_FILE_TYPE_PATTERN:
		{
			int length;
			g_value_set_boxed (value, geany_file_type_get_pattern (self, &length));
		}
		break;
		case GEANY_FILE_TYPE_TITLE:
		g_value_set_string (value, geany_file_type_get_title (self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


static void _g_object_unref0_ (gpointer var) {
	(var == NULL) ? NULL : (var = (g_object_unref (var), NULL));
}


GeanyFileType* geany_file_type_manager_lookup_ft (GeanyFileTypeManager* self, struct GeanyFiletype* gft) {
	GeanyFileType* result = NULL;
	GHashTable* _tmp0_ = NULL;
	struct GeanyFiletype* _tmp1_ = NULL;
	gconstpointer _tmp2_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (gft != NULL, NULL);
	_tmp0_ = self->priv->fts;
	_tmp1_ = gft;
	_tmp2_ = g_hash_table_lookup (_tmp0_, _tmp1_);
	result = (GeanyFileType*) _tmp2_;
	return result;
}


GeanyFileType* geany_file_type_manager_detect_from_file (GeanyFileTypeManager* self, const gchar* utf8_fn) {
	GeanyFileType* result = NULL;
	struct GeanyFiletype* gft = NULL;
	const gchar* _tmp0_ = NULL;
	struct GeanyFiletype* _tmp1_ = NULL;
	GHashTable* _tmp2_ = NULL;
	gconstpointer _tmp3_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (utf8_fn != NULL, NULL);
	_tmp0_ = utf8_fn;
	_tmp1_ = filetypes_detect_from_file (_tmp0_);
	gft = _tmp1_;
	_tmp2_ = self->priv->fts;
	_tmp3_ = g_hash_table_lookup (_tmp2_, gft);
	result = (GeanyFileType*) _tmp3_;
	return result;
}


GeanyFileType* geany_file_type_manager_lookup_by_name (GeanyFileTypeManager* self, const gchar* name) {
	GeanyFileType* result = NULL;
	struct GeanyFiletype* gft = NULL;
	const gchar* _tmp0_ = NULL;
	struct GeanyFiletype* _tmp1_ = NULL;
	GHashTable* _tmp2_ = NULL;
	gconstpointer _tmp3_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (name != NULL, NULL);
	_tmp0_ = name;
	_tmp1_ = filetypes_lookup_by_name (_tmp0_);
	gft = _tmp1_;
	_tmp2_ = self->priv->fts;
	_tmp3_ = g_hash_table_lookup (_tmp2_, gft);
	result = (GeanyFileType*) _tmp3_;
	return result;
}


static GeanyFileTypeManager* geany_file_type_manager_construct (GType object_type) {
	GeanyFileTypeManager * self = NULL;
	self = (GeanyFileTypeManager*) g_object_new (object_type, NULL);
	{
		gsize i = 0UL;
		i = (gsize) 0;
		{
			gboolean _tmp0_ = FALSE;
			_tmp0_ = TRUE;
			while (TRUE) {
				gboolean _tmp1_ = FALSE;
				gsize _tmp3_ = 0UL;
				GPtrArray* _tmp4_ = NULL;
				guint _tmp5_ = 0U;
				struct GeanyFiletype* ft = NULL;
				GPtrArray* _tmp6_ = NULL;
				void** _tmp7_ = NULL;
				gsize _tmp8_ = 0UL;
				void* _tmp9_ = NULL;
				GHashTable* _tmp10_ = NULL;
				struct GeanyFiletype* _tmp11_ = NULL;
				struct GeanyFiletype* _tmp12_ = NULL;
				GeanyFileType* _tmp13_ = NULL;
				_tmp1_ = _tmp0_;
				if (!_tmp1_) {
					gsize _tmp2_ = 0UL;
					_tmp2_ = i;
					i = _tmp2_ + 1;
				}
				_tmp0_ = FALSE;
				_tmp3_ = i;
				_tmp4_ = filetypes_array;
				_tmp5_ = _tmp4_->len;
				if (!(_tmp3_ < ((gsize) _tmp5_))) {
					break;
				}
				_tmp6_ = filetypes_array;
				_tmp7_ = _tmp6_->pdata;
				_tmp8_ = i;
				_tmp9_ = _tmp7_[_tmp8_];
				ft = (struct GeanyFiletype*) _tmp9_;
				_tmp10_ = self->priv->fts;
				_tmp11_ = ft;
				_tmp12_ = ft;
				_tmp13_ = geany_file_type_new (_tmp12_);
				g_hash_table_insert (_tmp10_, _tmp11_, _tmp13_);
			}
		}
	}
	return self;
}


static GeanyFileTypeManager* geany_file_type_manager_new (void) {
	return geany_file_type_manager_construct (GEANY_TYPE_FILE_TYPE_MANAGER);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


GeanyFileTypeManager* geany_file_type_manager_get_default (void) {
	GeanyFileTypeManager* result = NULL;
	GeanyFileTypeManager* _tmp0_ = NULL;
	GeanyFileTypeManager* _tmp1_ = NULL;
	_tmp0_ = geany_file_type_manager_inst;
	_tmp1_ = _g_object_ref0 (_tmp0_);
	result = _tmp1_;
	return result;
}


static void geany_file_type_manager_class_init (GeanyFileTypeManagerClass * klass) {
	GeanyFileTypeManager* _tmp0_ = NULL;
	geany_file_type_manager_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GeanyFileTypeManagerPrivate));
	G_OBJECT_CLASS (klass)->finalize = geany_file_type_manager_finalize;
	_tmp0_ = geany_file_type_manager_new ();
	geany_file_type_manager_inst = _tmp0_;
}


static void geany_file_type_manager_instance_init (GeanyFileTypeManager * self) {
	GHashFunc _tmp0_ = NULL;
	GEqualFunc _tmp1_ = NULL;
	GHashTable* _tmp2_ = NULL;
	self->priv = GEANY_FILE_TYPE_MANAGER_GET_PRIVATE (self);
	_tmp0_ = g_direct_hash;
	_tmp1_ = g_direct_equal;
	_tmp2_ = g_hash_table_new_full (_tmp0_, _tmp1_, NULL, _g_object_unref0_);
	self->priv->fts = _tmp2_;
}


static void geany_file_type_manager_finalize (GObject* obj) {
	GeanyFileTypeManager * self;
	self = G_TYPE_CHECK_INSTANCE_CAST (obj, GEANY_TYPE_FILE_TYPE_MANAGER, GeanyFileTypeManager);
	_g_hash_table_unref0 (self->priv->fts);
	G_OBJECT_CLASS (geany_file_type_manager_parent_class)->finalize (obj);
}


GType geany_file_type_manager_get_type (void) {
	static volatile gsize geany_file_type_manager_type_id__volatile = 0;
	if (g_once_init_enter (&geany_file_type_manager_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (GeanyFileTypeManagerClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) geany_file_type_manager_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GeanyFileTypeManager), 0, (GInstanceInitFunc) geany_file_type_manager_instance_init, NULL };
		GType geany_file_type_manager_type_id;
		geany_file_type_manager_type_id = g_type_register_static (G_TYPE_OBJECT, "GeanyFileTypeManager", &g_define_type_info, 0);
		g_once_init_leave (&geany_file_type_manager_type_id__volatile, geany_file_type_manager_type_id);
	}
	return geany_file_type_manager_type_id__volatile;
}


static gint _vala_array_length (gpointer array) {
	int length;
	length = 0;
	if (array) {
		while (((gpointer*) array)[length]) {
			length++;
		}
	}
	return length;
}



