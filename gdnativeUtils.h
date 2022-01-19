#include <gdnative_api_struct.gen.h>

void init_gdnative_utils(const godot_gdnative_core_api_struct* api_struct);
void finish_gdnative_utils();

const char* godot_string_to_c_str(const godot_string* gstr); 

const char* godot_string_name_to_c_str(const godot_string_name *name); 

const char* godot_variant_string_to_c_str(const godot_variant *var);

void godot_string_new_with_value(godot_string* gs, const char* s);

void godot_variant_new_string_with_value(godot_variant *var, const char* s); 

void godot_dictionary_set_strings(godot_dictionary *dict, const char *key, const char *val); 

const char* godot_globalize_path(const char *path);

void godot_print_variant(const godot_variant* message);

bool godot_is_special_method(const char *method_name);

const char *get_node_notification_name(int notification);
