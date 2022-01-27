#include "squeakUtils.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include "sqMessage.h"
#include "gdnativeUtils.h"

int squeak_main(int argc, char **argv, char **envp);
int osCogStackPageHeadroom();

typedef struct {
  char* plugin_dir;
} SqueakThreadArgs;

void* run_squeak(void* a) {
  SqueakThreadArgs* args = (SqueakThreadArgs*) a;

  const char* image_name = "image/squeak.image";
  const int path_len = strlen(args->plugin_dir) + 1 + strlen(image_name);
  char* image_path = (char*) malloc(path_len + 1);
  snprintf(image_path, path_len + 1, "%s/%s", args->plugin_dir, image_name);

  printf("Squeak image path: %s\n", image_path);

  printf("Running squeak in separate thread\n");
  char* fake_argv[] = {"fake_name", image_path};
  char* fake_envp[] = {NULL};
  squeak_main(2, fake_argv, fake_envp);


  free(image_path);
  free(args->plugin_dir);
  free(a);
  return 0;
}

void start_squeak_thread(const char* lib_path) {
  osCogStackPageHeadroom();

  pthread_t thread;
  pthread_attr_t tattr;
  size_t size;
  pthread_attr_init(&tattr);
  pthread_attr_getstacksize(&tattr, &size);
  pthread_attr_setstacksize(&tattr, size * 4);

  SqueakThreadArgs *args = (SqueakThreadArgs* )malloc(sizeof(SqueakThreadArgs));
  char* lib_path_copy = strdup(lib_path);
  char* plugin_dir = dirname(lib_path_copy);
  args->plugin_dir = strdup(plugin_dir);

  pthread_create(&thread, &tattr, run_squeak, (void*) args);
  printf("Detaching thread\n");
  pthread_detach(thread);

  free(lib_path_copy);
}

void init_squeak(const char* lib_path) {
  start_squeak_thread(lib_path);
}

void finish_squeak() {
}

void destroy_script_functions(script_functions_t* script_functions) {
  for (int i = 0; i < script_functions->num_names; ++i) {
    free(script_functions->names[i]);
  }
  free(script_functions);
}

// TODO: complete this list
// TODO: consider remapping every method to the proper smalltalk selector
//       (colons instead of underscores)
#define NUM_SPECIAL_METHODS 3
static const char* special_methods[NUM_SPECIAL_METHODS][2] = {
  {"_enter_tree", "enterTree_"},
  {"_ready", "ready"},
  {"_process", "process_"},
};

const char* remap_method_name(const char* method_name) {
  for (int i = 0; i < NUM_SPECIAL_METHODS; ++i) {
    if (strcmp(method_name, special_methods[i][0]) == 0) {
      return special_methods[i][1];
    }
  }
  return method_name;
}

char* squeak_new_script(const char* script_name, const char* parent_name) {
  message_data_t data;
  data.new_script.script_name = script_name;
  data.new_script.parent_name = parent_name;
  return send_message(SQP_NEW_SCRIPT, &data);
}

script_functions_t* squeak_reload_script(const char* script_path, const char* script_source) {
  message_data_t data;
  data.script_reload.script_path = script_path;
  data.script_reload.script_source = script_source;
  return send_message(SQP_SCRIPT_RELOAD, &data);
}

void squeak_new_instance(const char* script_path, const godot_object* owner) {
  message_data_t data;
  data.new_instance.script_path = script_path;
  data.new_instance.owner = owner;
  send_message(SQP_NEW_INSTANCE, &data);
}

void squeak_call_method(const char* method_name, const godot_object* owner, const godot_variant** args, int arg_count, godot_variant* result) {
  message_data_t data;
  // is this strdup really necessary here?
  data.method_call.method_name = strdup(method_name);
  data.method_call.owner = owner;
  data.method_call.args = args;
  data.method_call.arg_count = arg_count;
  data.method_call.result = result;
  send_message(SQP_FUNCTION_CALL, &data);
  printf("method call result: ");
  if (result != NULL) {
    printf("received godot variant that is not NULL. type: %i\n", godot_get_variant_type(result));
  } else {
    printf("NULL\n");
  }
  free((void*) data.method_call.method_name);
}
