#include <semaphore.h>
#include <gdnative_api_struct.gen.h>

enum MessageType {
  SQP_NEW_SCRIPT = 0,
  SQP_SCRIPT_RELOAD = 1,
  SQP_NEW_INSTANCE = 2,
  SQP_FUNCTION_CALL = 3,
};

typedef union {
  struct {
    const char* script_name;
    const char* parent_name;
  } new_script;

  struct {
    const char* script_path;
    const char* script_source;
  } script_reload;

  struct {
    const char* script_path;
    const godot_object* owner;
  } new_instance;

  struct {
    const char* method_name;
    const godot_object* owner;
    const godot_variant** args;
    int arg_count;
    godot_variant* result;
  } method_call;
} message_data_t;

void* send_message(enum MessageType type, message_data_t *data);
