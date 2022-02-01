#include "sqMessage.h"

#include "gdnativeUtils.h"
#include "lfqueue/lfqueue.h"

#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

static lfqueue_t squeak_queue;
static lfqueue_t godot_queue;

typedef struct {
  int32_t type; // enum MessageType
  void* data;
} message_t;

// TODO: volatile?
static message_t* godot_inbox;
static message_t* squeak_inbox;

static sem_t godot_message_signal;
static sem_t squeak_message_signal;

void squeak_send_message(message_t* message) {
  godot_inbox = message;
  sem_post(&godot_message_signal);
}

void godot_send_message(message_t* message) {
  squeak_inbox = message;
  sem_post(&squeak_message_signal);
}

message_t* squeak_read_message_blocking() {
  sem_wait(&squeak_message_signal);
  return squeak_inbox;
}

static bool init_queue(lfqueue_t* queue) {
  // TODO not technically thread-safe
  if (lfqueue_init(queue) == -1) {
    fprintf(stderr, "failed to init queue\n");
    return false;
  }
  return true;
}

bool init_sqmessage() {
  // TODO handle failure cases
  sem_init(&godot_message_signal, 0, 0);
  sem_init(&squeak_message_signal, 0, 0);
  return init_queue(&squeak_queue) && init_queue(&godot_queue);
}

void finish_sqmessage() {
  sem_destroy(&godot_message_signal);
  sem_destroy(&squeak_message_signal);
  lfqueue_destroy(&squeak_queue);
  lfqueue_destroy(&godot_queue);
}

typedef struct {
  godot_string* method_name;
  godot_variant* self;
  const godot_variant** args;
  int arg_count;
  godot_variant_call_error* error;
  godot_variant* result;
} godot_method_call_t;

static int processing_stack_count = 0;

void* process_responses(message_t* message) {
  ++processing_stack_count;

  while (true) {
    printf("Waiting for response to message of type %i...\n", message->type);
    sem_wait(&godot_message_signal);

    message_t* response = godot_inbox;

    switch (response->type) {
      case SQP_GODOT_FINISH_PROCESSING:
        printf("SQP_GODOT_FINISH_PROCESSING\n");
        void* result = response->data;
        free(response);
        --processing_stack_count;
        return result;
        break;
      case SQP_GODOT_FUNCTION_CALL:
        printf("SQP_GODOT_FUNCTION_CALL\n");
        godot_method_call_t* data = (godot_method_call_t*) response->data;
        printf("Squeak called Godot function: %s\n", godot_string_to_c_str(data->method_name));
        *data->result = godot_call_variant(data->self, data->method_name, data->args, data->arg_count, data->error);
        message_t* r = (message_t*) malloc(sizeof(message_t));
        r->type = SQP_SQUEAK_FINISH_PROCESSING;
        godot_send_message(r);
        break;
      default:
        fprintf(stderr, "Godot received message that it can't handle! %i\n", response->type);
        break;
    }

    /* free(response); */
    // TODO: do this only if debug enabled
    response = NULL;
  }
}

bool is_currently_processing() {
  return processing_stack_count > 0;
}

// we send a message to squeak and wait for it to be handled
void* send_message(enum MessageType type, void* data) {
  message_t m;
  m.type = type;
  m.data = data;

  if (is_currently_processing()) {
    printf("Currently processing, using signalling mechanism\n");
    godot_send_message(&m);
  } else {
    printf("Not currently processing, using busy wait mechanism\n");
    if (lfqueue_enq(&squeak_queue, &m) != 0) {
      fprintf(stderr, "failed to enqueue");
    }
  }

  return process_responses(&m);
}

// next message or NULL if empty
message_t *read_message() {
  return (message_t *) lfqueue_single_deq(&squeak_queue);
}

void destroy_script_functions(script_functions_t* script_functions) {
  for (int i = 0; i < script_functions->num_names; ++i) {
    free(script_functions->names[i]);
  }
  free(script_functions);
}

typedef struct {
  const char* script_name;
  const char* parent_name;
} new_script_t;

char* squeak_new_script(const char* script_name, const char* parent_name) {
  new_script_t data = {
    script_name,
    parent_name
  };
  return send_message(SQP_SQUEAK_NEW_SCRIPT, &data);
}

typedef struct {
  const char* script_path;
  const char* script_source;
} script_reload_t;

script_functions_t* squeak_reload_script(const char* script_path, const char* script_source) {
  script_reload_t data = {
    script_path,
    script_source
  };
  return send_message(SQP_SQUEAK_SCRIPT_RELOAD, &data);
}

typedef struct {
  const char* script_path;
  const godot_object* owner;
} new_instance_t;

void squeak_new_instance(const char* script_path, const godot_object* owner) {
  new_instance_t data = {
    script_path,
    owner
  };
  send_message(SQP_SQUEAK_NEW_INSTANCE, &data);
}

typedef struct {
  const char* method_name;
  const godot_object* owner;
  const godot_variant** args;
  int arg_count;
  godot_variant_call_error* error;
  godot_variant* result;
} method_call_t;

void squeak_call_method(const char* method_name, const godot_object* owner, const godot_variant** args, int arg_count, godot_variant_call_error* error, godot_variant* result) {
  method_call_t data = {
    // is this strdup really necessary here?
    strdup(method_name),
    owner,
    args,
    arg_count,
    error,
    result
  };
  send_message(SQP_SQUEAK_FUNCTION_CALL, &data);
  printf("method call result: ");
  free((void*) data.method_name);
}
