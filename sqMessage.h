#include <semaphore.h>

enum MessageType {
  SQP_NEW_SCRIPT = 0,
  SQP_SCRIPT_RELOAD = 1,
  SQP_NEW_INSTANCE = 2,
  SQP_FUNCTION_CALL = 3,
};

typedef union {
  struct {
    const char* script_name;
  } new_script;

  struct {
    const char* script_path;
    const char* script_source;
  } script_reload;

  struct {
    int a;
  } new_instance;

  struct {
    const char* method_name;
  } function_call;
} message_data_t;

void* send_message(enum MessageType type, message_data_t *data);
