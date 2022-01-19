#include <gdnative_api_struct.gen.h>

void init_squeak(const char* lib_path);
void finish_squeak();

// TODO: move these to sqMessage
char* squeak_new_script(const char* script_name);
void squeak_reload_script(const char* script_path, const char* script_source);
void squeak_new_instance(const char* script_path, const godot_object* owner);
void squeak_call_method(const char* method_name, const godot_object* owner, const godot_variant** args, int arg_count);
