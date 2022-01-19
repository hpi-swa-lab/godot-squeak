#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

#include "gdnativeUtils.h"
#include "squeakUtils.h"

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_pluginscript_api_struct *pluginscript_api = NULL;
const char* lib_path = NULL;

godot_pluginscript_language_data* smalltalk_lang_init() {
  printf("smalltalk_lang_init\n");
  init_squeak(lib_path);
  return NULL;
}

void smalltalk_lang_finish() {
  printf("smalltalk_lang_finish\n");
  finish_squeak();
}

godot_string smalltalk_get_template_source_code(godot_pluginscript_language_data *p_data, const godot_string *p_class_name, const godot_string *p_base_class_name) {
  godot_string template;
  //
  char* class_name = strdup(godot_string_to_c_str(p_class_name));
  char* template_str = squeak_new_script(class_name);

  godot_string_new_with_value(&template, template_str);

  free(class_name);
  free(template_str);

  return template;
}

void smalltalk_add_global_constant(godot_pluginscript_language_data *p_data, const godot_string *p_variable, const godot_variant *p_value) {
  printf("add_global_constant");
}

typedef struct {
  char* path;
} smalltalk_script_data_t;

godot_pluginscript_script_manifest smalltalk_script_init(godot_pluginscript_language_data *p_data, const godot_string *p_path, const godot_string *p_source, godot_error *r_error) {
  printf("smalltalk_script_init\n");
  printf("\tpath: %s\n", godot_string_to_c_str(p_path));

  smalltalk_script_data_t *data = malloc(sizeof(smalltalk_script_data_t));
  data->path = strdup(godot_globalize_path(godot_string_to_c_str(p_path)));

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

  godot_dictionary process_fake;

  api->godot_dictionary_new(&process_fake);
  godot_dictionary_set_strings(&process_fake, "name", "_process");

  godot_variant process_fake_var;
  api->godot_variant_new_dictionary(&process_fake_var, &process_fake);
  api->godot_array_push_back(&manifest.methods, &process_fake_var);

  api->godot_dictionary_destroy(&process_fake);

  char* script_source = strdup(godot_string_to_c_str(p_source));
  squeak_reload_script(data->path, script_source);
  free(script_source);

  return manifest;
}

void smalltalk_script_finish(godot_pluginscript_script_data *p_data) {
  printf("smalltalk_script_finish\n");
  free(((smalltalk_script_data_t*) p_data)->path);
  free(p_data);
}

typedef struct {
  godot_object *owner;
} smalltalk_instance_data_t;

// if this function returns NULL, Godot considers the initialization failed
godot_pluginscript_instance_data *smalltalk_instance_init(godot_pluginscript_script_data *p_data, godot_object *p_owner) {
  printf("smalltalk_instance_init\n");
  printf("\tscript path: %s\n", ((smalltalk_script_data_t*) p_data)->path);
  smalltalk_instance_data_t* data = malloc(sizeof(smalltalk_instance_data_t));
  data->owner = p_owner;
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
  printf("smalltalk_set_prop\n");
  return false;
}

godot_bool smalltalk_get_prop(godot_pluginscript_instance_data *p_data, const godot_string *p_name, godot_variant *r_ret) {
  printf("smalltalk_get_prop\n");
  return false;
}

int call_count = 0;
int notification_count = 0; 

godot_variant smalltalk_call_method(godot_pluginscript_instance_data *p_data,
    const godot_string_name *p_method, const godot_variant **p_args,
    int p_argcount, godot_variant_call_error *r_error) {
  const char* method_name = godot_string_name_to_c_str(p_method);

  if (strcmp(method_name, "_process") == 0) {
    if (++call_count % 100 == 0) {
      printf("_process has been called %i times\n", call_count);
    }
  } else {
    printf("smalltalk_call_method %s\n", method_name);
    squeak_call_method(
        godot_is_special_method(method_name) ? &method_name[1] : method_name,
        ((smalltalk_instance_data_t*)p_data)->owner, p_args, p_argcount);
  }

  godot_variant var;
  api->godot_variant_new_nil(&var);
  return var;
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
  // optional
  /* .reserved_words = (const char *[]){NULL}, */
  // optional
  /* .comment_delimiters = (const char *[]){NULL}, */
  // optional
  /* .string_delimiters = (const char *[]){NULL}, */
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
  init_gdnative_utils(api);

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
