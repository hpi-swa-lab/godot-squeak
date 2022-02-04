#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

#include "apiStructDecl.h"
#include "sqMessage.h"
#include "gdnativeUtils.h"
#include "squeakUtils.h"

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_core_1_1_api_struct *api_1_1 = NULL;
const godot_gdnative_core_1_2_api_struct *api_1_2 = NULL;
const godot_gdnative_ext_pluginscript_api_struct *pluginscript_api = NULL;
const char* lib_path = NULL;

godot_pluginscript_language_data* smalltalk_lang_init() {
  printf("smalltalk_lang_init\n");
  if (!init_sqmessage()) {
    fprintf(stderr, "Language initialization failed!");
    exit(1);
  }
  init_squeak(lib_path);
  return NULL;
}

void smalltalk_lang_finish() {
  printf("smalltalk_lang_finish\n");
  finish_squeak();
}

godot_string smalltalk_get_template_source_code(godot_pluginscript_language_data *p_data, const godot_string *p_class_name, const godot_string *p_base_class_name) {
  godot_string template;
  // TODO: are this strdups necessary?
  char* class_name = strdup(godot_string_to_c_str(p_class_name));
  char* parent_name = strdup(godot_string_to_c_str(p_base_class_name));
  char* template_str = squeak_new_script(class_name, parent_name);

  godot_string_new_with_value(&template, template_str);

  free(class_name);
  free(parent_name);
  free(template_str);

  return template;
}

void smalltalk_add_global_constant(godot_pluginscript_language_data *p_data, const godot_string *p_variable, const godot_variant *p_value) {
  printf("add_global_constant");
}

typedef struct {
  char* path;
  script_description_t* description;
} smalltalk_script_data_t;

godot_pluginscript_script_manifest smalltalk_script_init(godot_pluginscript_language_data *p_data, const godot_string *p_path, const godot_string *p_source, godot_error *r_error) {
  printf("smalltalk_script_init\n");
  printf("\tpath: %s\n", godot_string_to_c_str(p_path));

  smalltalk_script_data_t *data = malloc(sizeof(smalltalk_script_data_t));
  data->path = strdup(godot_globalize_path(godot_string_to_c_str(p_path)));

  char* script_source = strdup(godot_string_to_c_str(p_source));
  data->description = squeak_reload_script(data->path, script_source);
  free(script_source);

  godot_pluginscript_script_manifest manifest = {
    .data = data,
    .is_tool = false,
  };

  api->godot_string_name_new_data(&manifest.name, "");
  api->godot_string_name_new_data(&manifest.base, "");
  api->godot_dictionary_new(&manifest.member_lines);
  api->godot_array_new(&manifest.methods);
  api->godot_array_new(&manifest.signals);
  api->godot_array_new(&manifest.properties);

  printf("Script contains %i functions\n", data->description->functions.num_names);
  for (int i = 0; i < data->description->functions.num_names; ++i) {
    printf("\t-> %s\n", data->description->functions.names[i]);

    godot_array_push_single_entry_dictionary(
        &manifest.methods, "name", data->description->functions.names[i]);
    if (strcmp(data->description->functions.names[i], "process_") == 0) {
      godot_array_push_single_entry_dictionary(&manifest.methods, "name", "_process");
    }
  }

  printf("Script contains %i signals\n", data->description->signals.num_names);
  for (int i = 0; i < data->description->signals.num_names; ++i) {
    printf("\t-> %s\n", data->description->signals.names[i]);

    godot_array_push_single_entry_dictionary(
        &manifest.signals, "name", data->description->signals.names[i]);
  }

  printf("Script contains %i properties\n", data->description->properties.num);
  for (int i = 0; i < data->description->properties.num; ++i) {
    script_property_t* property = &data->description->properties.properties[i];
    godot_int type = api->godot_variant_get_type(property->default_value);
    printf("\t -> %s of type %i\n", property->name, type);

    godot_dictionary properties_dict;
    api->godot_dictionary_new(&properties_dict);

    godot_dictionary_set_strings(&properties_dict, "name", property->name);
    godot_dictionary_set_int(&properties_dict, "type", type);
    godot_dictionary_set_variant(&properties_dict, "default_value", property->default_value);

    godot_variant dict_var;
    api->godot_variant_new_dictionary(&dict_var, &properties_dict);
    api->godot_array_push_back(&manifest.properties, &dict_var);

    api->godot_dictionary_destroy(&properties_dict);
  }


  *r_error = GODOT_OK;

  return manifest;
}

void smalltalk_script_finish(godot_pluginscript_script_data *p_data) {
  printf("smalltalk_script_finish\n");
  printf("DON'T FORGET TO FREE THE STUFF HERE YO\n");
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

typedef struct {
  godot_object *owner;
  script_functions_t* functions;
} smalltalk_instance_data_t;

// if this function returns NULL, Godot considers the initialization failed
godot_pluginscript_instance_data *smalltalk_instance_init(godot_pluginscript_script_data *p_data, godot_object *p_owner) {
  printf("smalltalk_instance_init\n");
  printf("\tscript path: %s\n", ((smalltalk_script_data_t*) p_data)->path);
  smalltalk_instance_data_t* data = malloc(sizeof(smalltalk_instance_data_t));
  data->owner = p_owner;
  data->functions = &((smalltalk_script_data_t*) p_data)->description->functions;
  printf("owner: %p\n", p_owner);
  squeak_new_instance(((smalltalk_script_data_t*) p_data)->path, p_owner);
  return data;
}

void smalltalk_instance_finish(godot_pluginscript_instance_data *p_data) {
  printf("smalltalk_instance_finish\n");
  // TODO: destroy smalltalk object
  free(p_data);
}

godot_bool smalltalk_set_prop(godot_pluginscript_instance_data *p_data, const godot_string *p_name, const godot_variant *p_value) {
  bool* success_p = squeak_set_property(godot_string_to_c_str(p_name), ((smalltalk_instance_data_t*) p_data)->owner, p_value);
  bool success = *success_p;
  free(success_p);
  return success;
}

godot_bool smalltalk_get_prop(godot_pluginscript_instance_data *p_data, const godot_string *p_name, godot_variant *r_ret) {
  api->godot_variant_new_nil(r_ret);
  bool* success_p = squeak_get_property(godot_string_to_c_str(p_name), ((smalltalk_instance_data_t*) p_data)->owner, r_ret);
  bool success = *success_p;
  free(success_p);
  return success;
}

int call_count = 0;
int notification_count = 0; 

godot_variant smalltalk_call_method(godot_pluginscript_instance_data *p_data,
    const godot_string_name *p_method, const godot_variant **p_args,
    int p_argcount, godot_variant_call_error *r_error) {
  const char* method_name = remap_method_name(godot_string_name_to_c_str(p_method));

  smalltalk_instance_data_t* data = (smalltalk_instance_data_t*) p_data;

  godot_variant ret;

  // TODO allow handling of special godot functions like ready and process
  bool can_handle_method = false;
  for (int i = 0; i < data->functions->num_names; ++i) {
    if(strcmp(method_name, data->functions->names[i]) == 0) {
      can_handle_method = true;
      break;
    }
  }
  if (!can_handle_method) {
    printf("Cannot handle method %s\n", method_name);
    r_error->error = GODOT_CALL_ERROR_CALL_ERROR_INVALID_METHOD;
    api->godot_variant_new_nil(&ret);
    return ret;
  }

  bool is_process = strcmp(method_name, "process_") == 0;

  if (is_process) {
    if (++call_count % 100 == 0) {
      printf("_process has been called %i times\n", call_count);
    }
  } else {
      printf("smalltalk_call_method %s\n", method_name);
  }

  squeak_call_method(method_name, data->owner, p_args, p_argcount, r_error, &ret);
  
  if (!is_process) {
    fprintf(stderr, "method returned with %i\n", r_error->error);
  }

  // TODO: limited error messages are possible by modifying r_error

  /* api->godot_variant_new_nil(&var); */
  return ret;
}

void smalltalk_notification(godot_pluginscript_instance_data *p_data, int p_notification) {
  const char* notification_name = get_node_notification_name(p_notification);

  if (strcmp(notification_name, "NOTIFICATION_PROCESS") == 0) {
    if (++notification_count % 100 == 0) {
      printf("NOTIFICATION_PROCESS occurred %i times\n", call_count);
    }
  } else {
    printf("smalltalk_notification %s\n", notification_name);
  }
}

// required fields are in modules/gdnative/pluginscript/register_types.cpp:_check_language_desc 
static godot_pluginscript_language_desc smalltalk_language_desc = {
  .name = "Smalltalk",
  .type = "Smalltalk",
  // default extension, used for creating new files
  .extension = "st",
  // used in places where different extensions are possible, like validating file names
  .recognized_extensions = (const char*[]){"st", NULL},
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
    }
  }
};

void GDN_EXPORT sqplug_gdnative_init(godot_gdnative_init_options *p_options) {
  printf("Initializing sqplugin library\n");

  api = p_options->api_struct;
  /* api_1_1 = (const godot_gdnative_core_1_1_api_struct*) api->next; */
  /* api_1_2 = (const godot_gdnative_core_1_2_api_struct*) api_1_1->next; */
  init_gdnative_utils(api);

  printf(p_options->in_editor ? "Starting in editor\n" : "Starting in running game\n");

  lib_path = strdup(godot_string_to_c_str(p_options->active_library_path));
  printf("Active library path: %s\n", lib_path);

  for (int i = 0; i < api->num_extensions; ++i) {
    switch (api->extensions[i]->type) {
      case GDNATIVE_EXT_PLUGINSCRIPT:
        pluginscript_api = (godot_gdnative_ext_pluginscript_api_struct*) api->extensions[i];
        break;
    }
  }

  if (pluginscript_api == NULL) {
    printf("PluginScript API is missing!\n");
    return;
  }
  
  printf("Found PluginScript API v%i.%i\n", pluginscript_api->version.major, pluginscript_api->version.minor);

  pluginscript_api->godot_pluginscript_register_language(&smalltalk_language_desc);
}

void GDN_EXPORT sqplug_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
  printf("library term\n");
  finish_gdnative_utils();
}

void GDN_EXPORT sqplug_nativescript_init(void *p_handle) {
  printf("library nativescript init\n");
}

void GDN_EXPORT sqplug_gdnative_singleton() {
  printf("library singleton\n");
}
