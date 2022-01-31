#include "squeakUtils.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

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

