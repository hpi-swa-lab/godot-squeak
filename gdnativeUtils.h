#include <gdnative_api_struct.gen.h>

void init_gdnative_utils(const godot_gdnative_core_api_struct* api_struct);

const char* godot_string_to_c_str(const godot_string* gstr); 

const char* godot_string_name_to_c_str(const godot_string_name *name); 

void godot_string_new_with_value(godot_string* gs, const char* s);

void godot_variant_new_string_with_value(godot_variant *var, const char* s); 

void godot_dictionary_set_strings(godot_dictionary *dict, const char *key, const char *val); 

const char *get_node_notification_name(int notification);
