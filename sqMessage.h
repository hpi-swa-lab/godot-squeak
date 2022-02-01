#include <gdnative_api_struct.gen.h>

enum MessageType {
  SQP_SQUEAK_NEW_SCRIPT = 0,
  SQP_SQUEAK_SCRIPT_RELOAD = 1,
  SQP_SQUEAK_NEW_INSTANCE = 2,
  SQP_SQUEAK_FUNCTION_CALL = 3,

  SQP_GODOT_FINISH_PROCESSING = 100,
  SQP_GODOT_FUNCTION_CALL = 101,

  SQP_INVALID_MESSAGE = -1,
};

bool init_sqmessage();
void finish_sqmessage();

typedef struct {
  char** names;
  int num_names;
} script_functions_t;

void destroy_script_functions(script_functions_t* script_functions);

char* squeak_new_script(const char* script_name, const char* parent_name);
script_functions_t* squeak_reload_script(const char* script_path, const char* script_source);
void squeak_new_instance(const char* script_path, const godot_object* owner);
void squeak_call_method(const char* method_name, const godot_object* owner, const godot_variant** args, int arg_count, godot_variant* result);
