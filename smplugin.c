#include <gdnative_api_struct.gen.h>
/* #include <string.h> */
#include <stdio.h>
#include <pthread.h>

/* typedef struct user_data_struct { */
/* 	char data[256]; */
/* } user_data_struct; */

// GDNative supports a large collection of functions for calling back
// into the main Godot executable. In order for your module to have
// access to these functions, GDNative provides your application with
// a struct containing pointers to all these functions.
/* const godot_gdnative_core_api_struct *api = NULL; */
/* const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL; */

// These are forward declarations for the functions we'll be implementing
// for our object. A constructor and destructor are both necessary.
/* GDCALLINGCONV void *simple_constructor(godot_object *p_instance, void *p_method_data); */
/* GDCALLINGCONV void simple_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data); */
/* godot_variant simple_get_data(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args); */

// `gdnative_init` is a function that initializes our dynamic library.
// Godot will give it a pointer to a structure that contains various bits of
// information we may find useful among which the pointers to our API structures.
void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	/* api = p_options->api_struct; */

	/* // Find NativeScript extensions. */
	/* for (int i = 0; i < api->num_extensions; i++) { */
	/* 	switch (api->extensions[i]->type) { */
	/* 		case GDNATIVE_EXT_NATIVESCRIPT: { */
	/* 			nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i]; */
	/* 		}; break; */
	/* 		default: */
	/* 			break; */
	/* 	}; */
	/* }; */
  printf("library init\n");
}

// `gdnative_terminate` which is called before the library is unloaded.
// Godot will unload the library when no object uses it anymore.
void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	/* api = NULL; */
	/* nativescript_api = NULL; */
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
  printf("running squeak in separate thread\n");
  fflush(stdout);
  char* fake_argv[] = {"fake_name", "/home/leo/Uni/Master/PX/squeak/shared/Squeak6.0alpha-20582-64bit.image"};
  char* fake_envp[] = {NULL};
  squeak_main(2, fake_argv, fake_envp);
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

// `nativescript_init` is the most important function. Godot calls
// this function as part of loading a GDNative library and communicates
// back to the engine what objects we make available.
void GDN_EXPORT godot_nativescript_init(void *p_handle) {
  printf("library nativescript init\n");
  /* start_squeak(); */
	/* godot_instance_create_func create = { NULL, NULL, NULL }; */
	/* create.create_func = &simple_constructor; */

	/* godot_instance_destroy_func destroy = { NULL, NULL, NULL }; */
	/* destroy.destroy_func = &simple_destructor; */

	/* // We first tell the engine which classes are implemented by calling this. */
	/* // * The first parameter here is the handle pointer given to us. */
	/* // * The second is the name of our object class. */
	/* // * The third is the type of object in Godot that we 'inherit' from; */
	/* //   this is not true inheritance but it's close enough. */
	/* // * Finally, the fourth and fifth parameters are descriptions */
	/* //   for our constructor and destructor, respectively. */
	/* nativescript_api->godot_nativescript_register_class(p_handle, "SIMPLE", "Reference", create, destroy); */

	/* godot_instance_method get_data = { NULL, NULL, NULL }; */
	/* get_data.method = &simple_get_data; */

	/* godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED }; */

	/* // We then tell Godot about our methods by calling this for each */
	/* // method of our class. In our case, this is just `get_data`. */
	/* // * Our first parameter is yet again our handle pointer. */
	/* // * The second is again the name of the object class we're registering. */
	/* // * The third is the name of our function as it will be known to GDScript. */
	/* // * The fourth is our attributes setting (see godot_method_rpc_mode enum in */
	/* //   `godot_headers/nativescript/godot_nativescript.h` for possible values). */
	/* // * The fifth and final parameter is a description of which function */
	/* //   to call when the method gets called. */
	/* nativescript_api->godot_nativescript_register_method(p_handle, "SIMPLE", "get_data", attributes, get_data); */
}

// In our constructor, allocate memory for our structure and fill
// it with some data. Note that we use Godot's memory functions
// so the memory gets tracked and then return the pointer to
// our new structure. This pointer will act as our instance
// identifier in case multiple objects are instantiated.
/* GDCALLINGCONV void *simple_constructor(godot_object *p_instance, void *p_method_data) { */
/* 	user_data_struct *user_data = api->godot_alloc(sizeof(user_data_struct)); */
/* 	strcpy(user_data->data, "World from GDNative!"); */

/* 	return user_data; */
/* } */

// The destructor is called when Godot is done with our
// object and we free our instances' member data.
/* GDCALLINGCONV void simple_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data) { */
/* 	api->godot_free(p_user_data); */
/* } */

void GDN_EXPORT godot_gdnative_singleton() {
  printf("library singleton\n");
  start_squeak();
}
