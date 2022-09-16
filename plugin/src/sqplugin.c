#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

#define SOCKETS 1

#include "apiStructDecl.h"
#if SOCKETS
#include "sqMessageSockets.h"
#else
#include "sqMessage.h"
#endif
#include "gdnativeUtils.h"
#include "squeakUtils.h"
#include <sys/time.h>

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_core_1_1_api_struct *api_1_1 = NULL;
const godot_gdnative_core_1_2_api_struct *api_1_2 = NULL;
const godot_gdnative_ext_pluginscript_api_struct *pluginscript_api = NULL;
const char *lib_path = NULL;
bool in_editor;

godot_pluginscript_language_data *smalltalk_lang_init()
{
  printf("smalltalk_lang_init\n");
  if (!init_sqmessage())
  {
    fprintf(stderr, "Language initialization failed!");
    exit(1);
  }

  godot_variant data, response;
  godot_variant_new_nil(&data);

  send_message(SQP_INITIALIZE, &data, &response);

  godot_variant_destroy(&data);
  godot_variant_destroy(&response);
#if 0
  // FIXME experiment to test latency
  struct timeval t1, t2;
  for (int i = 0; i < 20; i++) {
  gettimeofday(&t1, NULL);
  gettimeofday(&t2, NULL);

  char buffer[2048];
  sprintf(buffer, "squeak_initialize_environment took %f seconds\n", (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0);
  godot_string s;
  godot_string_new_with_value(&s, buffer);
  godot_print(&s);
  printf("squeak_initialize_environment took %f seconds\n", (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0);
  }
#endif

  return NULL;
}

void smalltalk_lang_finish()
{
  printf("smalltalk_lang_finish\n");
#if !SOCKETS
  finish_squeak();
#endif
}

godot_string smalltalk_get_template_source_code(godot_pluginscript_language_data *p_data, const godot_string *p_class_name, const godot_string *p_base_class_name)
{
  godot_dictionary dict;
  godot_dictionary_new(&dict);
  godot_dictionary_set_godot_string(&dict, "script_name", *p_class_name);
  godot_dictionary_set_godot_string(&dict, "parent_name", *p_base_class_name);

  godot_variant data;
  godot_variant_new_dictionary(&data, &dict);

  godot_variant response;
  send_message(SQP_SQUEAK_SCRIPT_RELOAD, &data, &response);

  godot_string string = godot_variant_as_string(&response);
  godot_variant_destroy(&response);

  return string;
}

void smalltalk_add_global_constant(godot_pluginscript_language_data *p_data, const godot_string *p_variable, const godot_variant *p_value)
{
  printf("add_global_constant");
}

typedef struct
{
  char *path;
} smalltalk_script_data_t;

godot_pluginscript_script_manifest smalltalk_script_init(godot_pluginscript_language_data *p_data, const godot_string *p_path, const godot_string *p_source, godot_error *r_error)
{
  printf("smalltalk_script_init\n");
  printf("\tpath: %s\n", godot_string_to_c_str(p_path));

  godot_string script_path = godot_string_globalize_path(*p_path);
  godot_dictionary dict;
  godot_dictionary_new(&dict);
  godot_dictionary_set_godot_string(&dict, "script_path", script_path);
  godot_dictionary_set_godot_string(&dict, "script_source", *p_source);

  godot_variant data;
  godot_variant_new_dictionary(&data, &dict);

  godot_variant response;
  send_message(SQP_SQUEAK_NEW_SCRIPT, &data, &response);
  godot_dictionary dict_response = godot_variant_as_dictionary(&response);

  godot_pluginscript_script_manifest manifest = {
      .data = NULL,
      .is_tool = false,
  };

  godot_string_name_new_data(&manifest.name, "");
  godot_string_name_new_data(&manifest.base, "");
  godot_dictionary_new(&manifest.member_lines);

  godot_variant field_var;
  field_var = godot_dictionary_get_strings(&dict_response, "methods");
  godot_array methods = godot_variant_as_array(&field_var);
  godot_array_new_copy(&manifest.methods, &methods);
  godot_array_destroy(&methods);
  godot_variant_destroy(&field_var);

  field_var = godot_dictionary_get_strings(&dict_response, "signals");
  godot_array signals = godot_variant_as_array(&field_var);
  godot_array_new_copy(&manifest.signals, &signals);
  godot_array_destroy(&signals);
  godot_variant_destroy(&field_var);

  field_var = godot_dictionary_get_strings(&dict_response, "properties");
  godot_array properties = godot_variant_as_array(&field_var);
  godot_array_new_copy(&manifest.properties, &properties);
  godot_array_destroy(&properties);
  godot_variant_destroy(&field_var);

  godot_variant_destroy(&response);
  godot_variant_destroy(&data);
  godot_dictionary_destroy(&dict);

  *r_error = GODOT_OK;

  return manifest;
}

void smalltalk_script_finish(godot_pluginscript_script_data *p_data)
{
  printf("smalltalk_script_finish\n");
  /* smalltalk_script_data_t* data = ((smalltalk_script_data_t*) p_data); */
  /* if (data->path != NULL) { */
  /*   free(data->path); */
  /*   data->path = NULL; */
  /* } */
  /* if (data->functions != NULL) { */
  /*   destroy_script_functions(data->functions); */
  /*   data->functions = NULL; */
  /* } */
  /* free(p_data); */
  /* p_data = NULL; */
}

typedef struct
{
  godot_object *owner;
} smalltalk_instance_data_t;

// if this function returns NULL, Godot considers the initialization failed
godot_pluginscript_instance_data *smalltalk_instance_init(godot_pluginscript_script_data *p_data, godot_object *p_owner)
{
  printf("smalltalk_instance_init\n");
  printf("\tscript path: %s\n", ((smalltalk_script_data_t *)p_data)->path);
  smalltalk_instance_data_t *data = malloc(sizeof(smalltalk_instance_data_t));
  data->owner = p_owner;

  godot_variant owner_var;
  godot_variant_new_object(&owner_var, p_owner);

  godot_dictionary dict;
  godot_dictionary_new(&dict);
  godot_dictionary_set_strings(&dict, "script_path", ((smalltalk_script_data_t *)p_data)->path);
  godot_dictionary_set_variant(&dict, "owner", &owner_var);

  godot_variant request;
  godot_variant_new_dictionary(&request, &dict);

  godot_variant response;
  send_message(SQP_SQUEAK_NEW_INSTANCE, &request, &response);

  godot_variant_destroy(&response);
  godot_variant_destroy(&request);
  godot_variant_destroy(&owner_var);

  return data;
}

void smalltalk_instance_finish(godot_pluginscript_instance_data *p_data)
{
  printf("smalltalk_instance_finish\n");
  // TODO: destroy smalltalk object
  free(p_data);
}

godot_bool smalltalk_set_prop(godot_pluginscript_instance_data *p_data, const godot_string *p_name, const godot_variant *p_value)
{
  godot_variant owner;
  godot_variant_new_object(&owner, ((smalltalk_instance_data_t *)p_data)->owner);

  godot_variant data;
  godot_dictionary request;
  godot_dictionary_new(&request);
  godot_dictionary_set_godot_string(&request, "name", *p_name);
  godot_dictionary_set_variant(&request, "value", p_value);
  godot_dictionary_set_variant(&request, "object", &owner);

  godot_variant response;
  send_message(SQP_SQUEAK_SET_PROPERTY, &data, &response);

  godot_bool res = godot_variant_as_bool(&response);
  godot_variant_destroy(&response);
  godot_variant_destroy(&owner);
  godot_variant_destroy(&data);
  godot_dictionary_destroy(&request);

  return res;
}

godot_bool smalltalk_get_prop(godot_pluginscript_instance_data *p_data, const godot_string *p_name, godot_variant *r_ret)
{
  godot_variant owner;
  godot_variant_new_object(&owner, ((smalltalk_instance_data_t *)p_data)->owner);

  godot_variant data;
  godot_dictionary request;
  godot_dictionary_new(&request);
  godot_dictionary_set_godot_string(&request, "name", *p_name);
  godot_dictionary_set_variant(&request, "object", &owner);

  godot_variant response;
  send_message(SQP_SQUEAK_GET_PROPERTY, &data, &response);

  godot_bool res = godot_variant_as_bool(&response);
  godot_variant_destroy(&response);
  godot_variant_destroy(&owner);
  godot_variant_destroy(&data);
  godot_dictionary_destroy(&request);
  return res;
}

int call_count = 0;
int notification_count = 0;

godot_variant smalltalk_call_method(godot_pluginscript_instance_data *p_data,
                                    const godot_string_name *p_method, const godot_variant **p_args,
                                    int argcount, godot_variant_call_error *r_error)
{
  smalltalk_instance_data_t *data = (smalltalk_instance_data_t *)p_data;

  godot_variant owner;
  godot_variant_new_object(&owner, data->owner);

  godot_array args;
  godot_array_new(&args);
  godot_array_resize(&args, argcount);
  for (int i = 0; i < argcount; ++i)
  {
    godot_array_append(&args, p_args[i]);
  }
  godot_variant args_var;
  godot_variant_new_array(&args_var, &args);

  godot_variant method_name_var;
  const char *method_name = remap_method_name(godot_string_name_to_c_str(p_method));
  godot_variant_new_string_with_value(&method_name_var, method_name);

  godot_dictionary request;
  godot_dictionary_set_variant(&request, "object", &owner);
  godot_dictionary_set_variant(&request, "method_name", &method_name_var);
  godot_dictionary_set_variant(&request, "arguments", &args_var);

  godot_variant request_var;
  godot_variant_new_dictionary(&request_var, &request);
  godot_variant response;
  send_message(SQP_SQUEAK_FUNCTION_CALL, &request_var, &response);

  godot_dictionary res = godot_variant_as_dictionary(&response);
  // result should be nil on error
  godot_variant result = godot_dictionary_get_strings(&res, "result");
  godot_variant success = godot_dictionary_get_strings(&res, "success");

  r_error->error = godot_variant_as_bool(&success) ? GODOT_CALL_ERROR_CALL_OK : GODOT_CALL_ERROR_CALL_ERROR_INVALID_METHOD;

  return result;
}

void smalltalk_notification(godot_pluginscript_instance_data *p_data, int p_notification)
{
  // char *notification_name = get_node_notification_name(p_notification);
}

// required fields are in modules/gdnative/pluginscript/register_types.cpp:_check_language_desc
static godot_pluginscript_language_desc smalltalk_language_desc = {
    .name = "Smalltalk",
    .type = "Smalltalk",
    // default extension, used for creating new files
    .extension = "st",
    // used in places where different extensions are possible, like validating file names
    .recognized_extensions = (const char *[]){"st", NULL},
    // called when the ScriptServer is initialized, once in Main::setup
    .init = &smalltalk_lang_init,
    // called when the ScriptServer is deinitialzed, once in Main::cleanup
    .finish = &smalltalk_lang_finish,
    // this enables naming classes Godot's script creation dialog.
    // the name entered there will be passed to the template creation function
    .has_named_classes = false,
    // builtin scripts are saved in the scene, but we want to save them separately
    .supports_builtin_mode = false,
    .get_template_source_code = &smalltalk_get_template_source_code,
    .add_global_constant = &smalltalk_add_global_constant,

    .script_desc = {
        // called when PluginScript resource is reloaded (e.g. when the file has changed and the editor is refocused)
        .init = &smalltalk_script_init,
        // called in PluginScript destructor
        .finish = &smalltalk_script_finish,

        .instance_desc = {
            // called when PluginScriptInstance is created
            .init = &smalltalk_instance_init,
            // called in PluginScriptInstance destructor
            .finish = &smalltalk_instance_finish,
            // these methods are how outside code calls the script
            // Gdscript function equivalents like _process will be called correctly if call_method exposes them
            .set_prop = &smalltalk_set_prop,
            .get_prop = &smalltalk_get_prop,
            .call_method = &smalltalk_call_method,
            .notification = &smalltalk_notification,
        }}};

void GDN_EXPORT sqplug_gdnative_init(godot_gdnative_init_options *p_options)
{
  printf("Initializing sqplugin library\n");

  api = p_options->api_struct;
  api_1_1 = (const godot_gdnative_core_1_1_api_struct *)api->next;
  api_1_2 = (const godot_gdnative_core_1_2_api_struct *)api_1_1->next;
  init_gdnative_utils(api);

  printf(p_options->in_editor ? "Starting in editor\n" : "Starting in running game\n");

  lib_path = strdup(godot_string_to_c_str(p_options->active_library_path));
  printf("Active library path: %s\n", lib_path);

  in_editor = p_options->in_editor;
  printf("In editor: %s\n", in_editor ? "true" : "false");

  for (int i = 0; i < api->num_extensions; ++i)
  {
    switch (api->extensions[i]->type)
    {
    case GDNATIVE_EXT_PLUGINSCRIPT:
      pluginscript_api = (godot_gdnative_ext_pluginscript_api_struct *)api->extensions[i];
      break;
    }
  }

  if (pluginscript_api == NULL)
  {
    printf("PluginScript API is missing!\n");
    return;
  }

  printf("Found PluginScript API v%i.%i\n", pluginscript_api->version.major, pluginscript_api->version.minor);

  pluginscript_api->godot_pluginscript_register_language(&smalltalk_language_desc);
}

void GDN_EXPORT sqplug_gdnative_terminate(godot_gdnative_terminate_options *p_options)
{
  printf("library term\n");
  finish_gdnative_utils();
}

void GDN_EXPORT sqplug_nativescript_init(void *p_handle)
{
  printf("library nativescript init\n");
}

void GDN_EXPORT sqplug_gdnative_singleton()
{
  printf("library singleton\n");
}
