#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>
#include <libgen.h>
#include "gdnativeUtils.h"

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_pluginscript_api_struct *pluginscript_api = NULL;
const char* lib_path = NULL;

godot_pluginscript_language_data* smalltalk_lang_init() {
  printf("smalltalk_lang_init\n");
  return NULL;
}

void smalltalk_lang_finish() {
  printf("smalltalk_lang_finish\n");
}

void smalltalk_add_global_constant(godot_pluginscript_language_data *p_data, const godot_string *p_variable, const godot_variant *p_value) {
  printf("add_global_constant");
}

godot_pluginscript_script_manifest smalltalk_script_init(godot_pluginscript_language_data *p_data, const godot_string *p_path, const godot_string *p_source, godot_error *r_error) {
  printf("smalltalk_script_init\n");

  godot_pluginscript_script_manifest manifest = {
    .data = NULL,
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

  return manifest;
}

void smalltalk_script_finish(godot_pluginscript_script_data *p_data) {
  printf("smalltalk_script_finish\n");
}

// if this function returns NULL, Godot considers the initialization failed
godot_pluginscript_instance_data *smalltalk_instance_init(godot_pluginscript_script_data *p_data, godot_object *p_owner) {
  printf("smalltalk_instance_init\n");
  void* temp = malloc(1);
  return temp;
}

void smalltalk_instance_finish(godot_pluginscript_instance_data *p_data) {
  free(p_data);
  printf("smalltalk_instance_finish\n");
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
    printf("smalltalk_call_method %s\n", notification_name);
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
  // appears to enable naming the resource. probably not something we need
  .has_named_classes = false,
  // builtin scripts are saved in the scene, but we want to save them separately
  .supports_builtin_mode = false,
  .add_global_constant = &smalltalk_add_global_constant,

  .script_desc = {
    // called when PluginScript resource is reloaded (e.g. on editor refocus)
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

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
  printf("library init\n");

  api = p_options->api_struct;
  init_gdnative_utils(api);

  for (int i = 0; i < api->num_extensions; ++i) {
    switch (api->extensions[i]->type) {
      case GDNATIVE_EXT_PLUGINSCRIPT:
        pluginscript_api = (godot_gdnative_ext_pluginscript_api_struct*) api->extensions[i];
        break;
    }
  }

  if (pluginscript_api == NULL) {
    printf("pluginscript api is missing!\n");
    return;
  }
  
  printf("found pluginscript api v%i.%i\n", pluginscript_api->version.major, pluginscript_api->version.minor);

  pluginscript_api->godot_pluginscript_register_language(&smalltalk_language_desc);

  lib_path = strdup(godot_string_to_c_str(p_options->active_library_path));
  printf("active library path: %s\n", lib_path);
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
  printf("library term\n");
}

int squeak_main(int argc, char **argv, char **envp);
int osCogStackPageHeadroom();

typedef struct {
  int argc;
  char **argv;
  char **envp;
} Args;

void* run_squeak(void* a) {
  if (lib_path == NULL) {
    printf("lib_path is not set\n");
    return 0;
  }

  const char* image_name = "image/squeak.image";
  char* lib_path_d = strdup(lib_path);
  char* lib_dir = dirname(lib_path_d);
  const int path_len = strlen(lib_dir) + 1 + strlen(image_name);
  char* image_path = (char*) malloc(path_len + 1);
  snprintf(image_path, path_len + 1, "%s/%s", lib_dir, image_name);
  printf("Image path: %s\n", image_path);

  printf("running squeak in separate thread\n");
  fflush(stdout);
  char* fake_argv[] = {"fake_name", image_path};
  char* fake_envp[] = {NULL};
  /* squeak_main(2, fake_argv, fake_envp); */

  free(lib_path_d);
  free(image_path);
  return 0;
}

void start_squeak() {
  osCogStackPageHeadroom();
  pthread_t thread;
  pthread_attr_t tattr;
  size_t size;
  pthread_attr_init(&tattr);
  pthread_attr_getstacksize(&tattr, &size);
  pthread_attr_setstacksize(&tattr, size * 4);
  /* printf("Calling from runner\n"); */
  /* Args args = { argc, argv, envp  }; */
  Args args = {0, 0, 0};
  printf("Starting thread\n");
  pthread_create(&thread, &tattr, run_squeak, (void*) &args);
  printf("Detaching thread\n");
  pthread_detach(thread);
  /* printf("Joining thread\n"); */
  /* pthread_join(thread, NULL); */
  /* printf("Thread joined\n"); */
}

void GDN_EXPORT godot_nativescript_init(void *p_handle) {
  printf("library nativescript init\n");
}

void GDN_EXPORT godot_gdnative_singleton() {
  printf("library singleton\n");
  start_squeak();
}
