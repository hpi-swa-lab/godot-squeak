void init_squeak(const char* lib_path);
void finish_squeak();

void squeak_new_script(const char* script_name);
void squeak_reload_script(const char* script_path, const char* script_source);
void squeak_call_method(const char* method_name);
