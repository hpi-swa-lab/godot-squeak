#include "apiStructDecl.h"
#include "gdnativeUtils.h"

#include <string.h>

static godot_variant project_settings;
static godot_string globalize_path_method_name;

void init_gdnative_utils(const godot_gdnative_core_api_struct* api_struct) {
  godot_object *obj = api->godot_global_get_singleton("ProjectSettings");
  api->godot_variant_new_object(&project_settings, obj);
  godot_string_new_with_value(&globalize_path_method_name, "globalize_path");
}

void finish_gdnative_utils() { api->godot_variant_destroy(&project_settings);
  api->godot_string_destroy(&globalize_path_method_name);
}

// TODO: change all of these godot_ prefixes, bit confusing
const char* godot_string_to_c_str(const godot_string* gstr) {
  godot_char_string gcs = api->godot_string_ascii(gstr);
  const char* s = api->godot_char_string_get_data(&gcs);
  // getting weird behaviour if the following line is included.
  // unclear if it necessary
  /* api->godot_char_string_destroy(&gcs); */
  return s;
}

const char* godot_string_name_to_c_str(const godot_string_name *name) {
  godot_string gs = api->godot_string_name_get_name(name);
  const char* s = godot_string_to_c_str(&gs);
  api->godot_string_destroy(&gs);
  return s;
}

const char* godot_variant_string_to_c_str(const godot_variant *var) {
  godot_string gs = api->godot_variant_as_string(var);
  const char* s = godot_string_to_c_str(&gs);
  api->godot_string_destroy(&gs);
  return s;
}

void godot_string_new_with_value(godot_string* gs, const char* s) {
  api->godot_string_new(gs);
  api->godot_string_parse_utf8(gs, s);
}

void godot_variant_new_string_with_value(godot_variant *var, const char* s) {
  godot_string gs;
  godot_string_new_with_value(&gs, s);
  api->godot_variant_new_string(var, &gs);
  api->godot_string_destroy(&gs);
}

void godot_dictionary_set_variant(godot_dictionary *dict, const char *key, const godot_variant *val) {
  godot_variant key_var;
  godot_variant_new_string_with_value(&key_var, key);

  api->godot_dictionary_set(dict, &key_var, val);

  api->godot_variant_destroy(&key_var);
}

void godot_dictionary_set_strings(godot_dictionary *dict, const char *key, const char *val) {
  godot_variant val_var;
  godot_variant_new_string_with_value(&val_var, val);

  godot_dictionary_set_variant(dict, key, &val_var);

  api->godot_variant_destroy(&val_var);
}

void godot_dictionary_set_int(godot_dictionary *dict, const char *key, godot_int val) {
  godot_variant val_var;
  api->godot_variant_new_int(&val_var, val);

  godot_dictionary_set_variant(dict, key, &val_var);

  api->godot_variant_destroy(&val_var);
}

void godot_array_push_single_entry_dictionary(godot_array *arr, const char *key, const char *val) {
    godot_dictionary entry;
    api->godot_dictionary_new(&entry);

    godot_dictionary_set_strings(&entry, key, val);

    godot_variant entry_var;
    api->godot_variant_new_dictionary(&entry_var, &entry);
    api->godot_array_push_back(arr, &entry_var);

    api->godot_dictionary_destroy(&entry);
}

const char* godot_globalize_path(const char *path) {
  godot_variant arg;
  const godot_variant* argp = &arg;
  godot_variant_new_string_with_value(&arg, path);

  godot_variant_call_error err;
  godot_variant result = api->godot_variant_call(&project_settings, &globalize_path_method_name, &argp, 1, &err);
  return godot_variant_string_to_c_str(&result);
}

const char* godot_get_project_root() {
  return godot_globalize_path("res://");
}

void godot_print_variant(const godot_variant* message) {
  godot_string str = api->godot_variant_as_string(message);
  api->godot_print(&str);
}

void godot_print_string(const char* str) {
  godot_string gstr;
  godot_string_new_with_value(&gstr, str);
  api->godot_print(&gstr);
  api->godot_string_destroy(&gstr);
}

godot_variant godot_call_variant(godot_variant *p_self, const godot_string *p_method, const godot_variant **p_args, const godot_int p_argcount, godot_variant_call_error *r_error) {
  return api->godot_variant_call(p_self, p_method, p_args, p_argcount, r_error);
}

const char *get_node_notification_name(int notification) {
  // copied from godot-headers/api.json (Node)
  const char *name;
  switch(notification) {
    case 1015:
      name = "NOTIFICATION_APP_PAUSED";
      break;
    case 1014:
      name = "NOTIFICATION_APP_RESUMED";
      break;
    case 1012:
      name = "NOTIFICATION_CRASH";
      break;
    case 21:
      name = "NOTIFICATION_DRAG_BEGIN";
      break;
    case 22:
      name = "NOTIFICATION_DRAG_END";
      break;
    case 10:
      name = "NOTIFICATION_ENTER_TREE";
      break;
    case 11:
      name = "NOTIFICATION_EXIT_TREE";
      break;
    case 20:
      name = "NOTIFICATION_INSTANCED";
      break;
    case 26:
      name = "NOTIFICATION_INTERNAL_PHYSICS_PROCESS";
      break;
    case 25:
      name = "NOTIFICATION_INTERNAL_PROCESS";
      break;
    case 12:
      name = "NOTIFICATION_MOVED_IN_PARENT";
      break;
    case 1013:
      name = "NOTIFICATION_OS_IME_UPDATE";
      break;
    case 1009:
      name = "NOTIFICATION_OS_MEMORY_WARNING";
      break;
    case 18:
      name = "NOTIFICATION_PARENTED";
      break;
    case 23:
      name = "NOTIFICATION_PATH_CHANGED";
      break;
    case 14:
      name = "NOTIFICATION_PAUSED";
      break;
    case 16:
      name = "NOTIFICATION_PHYSICS_PROCESS";
      break;
    case 27:
      name = "NOTIFICATION_POST_ENTER_TREE";
      break;
    case 17:
      name = "NOTIFICATION_PROCESS";
      break;
    case 13:
      name = "NOTIFICATION_READY";
      break;
    case 1010:
      name = "NOTIFICATION_TRANSLATION_CHANGED";
      break;
    case 19:
      name = "NOTIFICATION_UNPARENTED";
      break;
    case 15:
      name = "NOTIFICATION_UNPAUSED";
      break;
    case 1011:
      name = "NOTIFICATION_WM_ABOUT";
      break;
    case 1004:
      name = "NOTIFICATION_WM_FOCUS_IN";
      break;
    case 1005:
      name = "NOTIFICATION_WM_FOCUS_OUT";
      break;
    case 1007:
      name = "NOTIFICATION_WM_GO_BACK_REQUEST";
      break;
    case 1002:
      name = "NOTIFICATION_WM_MOUSE_ENTER";
      break;
    case 1003:
      name = "NOTIFICATION_WM_MOUSE_EXIT";
      break;
    case 1006:
      name = "NOTIFICATION_WM_QUIT_REQUEST";
      break;
    case 1008:
      name = "NOTIFICATION_WM_UNFOCUS_REQUEST";
      break;
    default:
      name = "UNKNOWN NOTIFICATION";
      break;
  }
  return name;
}
