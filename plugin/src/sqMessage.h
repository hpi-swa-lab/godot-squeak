#include <gdnative_api_struct.gen.h>
#define SOCKETS 0

enum MessageType
{
  SQP_SQUEAK_NEW_SCRIPT = 0,
  SQP_SQUEAK_SCRIPT_RELOAD = 1,
  SQP_SQUEAK_NEW_INSTANCE = 2,
  SQP_SQUEAK_FUNCTION_CALL = 3,
  SQP_SQUEAK_FINISH_PROCESSING = 4,
  SQP_SQUEAK_SET_PROPERTY = 5,
  SQP_SQUEAK_GET_PROPERTY = 6,
  SQP_INITIALIZE = 7,

  SQP_GODOT_FINISH_PROCESSING = 100,
  SQP_GODOT_FUNCTION_CALL = 101,

  SQP_INVALID_MESSAGE = -1,
};

bool init_sqmessage();
void finish_sqmessage();

typedef struct
{
  char **names;
  int32_t num_names;
} script_functions_t;

typedef struct
{
  char **names;
  int32_t num_names;
} script_signals_t;

typedef struct
{
  char *name;
  godot_variant *default_value;
} script_property_t;

typedef struct
{
  int32_t num;
  script_property_t *properties;
} script_property_array;

typedef struct
{
  script_functions_t functions;
  script_signals_t signals;
  script_property_array properties;
} script_description_t;

void destroy_script_description(script_description_t *description);

char *squeak_new_script(const char *script_name, const char *parent_name);
script_description_t *squeak_reload_script(const char *script_path, const char *script_source);
void squeak_new_instance(const char *script_path, const godot_object *owner);
void squeak_call_method(const char *method_name, const godot_object *owner, const godot_variant **args, int arg_count, godot_variant_call_error *error, godot_variant *result);
// TODO: consider returning a bool directly instead of a pointer
bool *squeak_set_property(const char *property_name, const godot_object *owner, const godot_variant *value);
bool *squeak_get_property(const char *property_name, const godot_object *owner, godot_variant *out);
void squeak_initialize_environment(bool in_editor);
