#define SOCKETS 1
#if SOCKETS

#include <stdio.h>
#include "sqMessageSockets.h"

bool init_sqmessage()
{
    return true;
}

char *squeak_new_script(const char *script_name, const char *parent_name)
{
    printf("NEW SCRIPT\n");
    return NULL;
}

script_description_t *squeak_reload_script(const char *script_path, const char *script_source)
{
    script_description_t *script = calloc(1, sizeof(script_description_t));
    return script;
}

void squeak_new_instance(const char *script_path, const godot_object *owner)
{
    printf("NEW INSTANCE\n");
}
void squeak_call_method(const char *method_name, const godot_object *owner, const godot_variant **args, int arg_count, godot_variant_call_error *error, godot_variant *result)
{
    printf("CALL METHOD\n");
}
// TODO: consider returning a bool directly instead of a pointer
bool *squeak_set_property(const char *property_name, const godot_object *owner, const godot_variant *value)
{
    printf("SET PROPERTY\n");
    return NULL;
}
bool *squeak_get_property(const char *property_name, const godot_object *owner, godot_variant *out)
{
    printf("GET PROPERTY\n");
    return NULL;
}
void squeak_initialize_environment(bool in_editor)
{
    printf("INITIALIZE ENVIRONMENT\n");
}

#endif