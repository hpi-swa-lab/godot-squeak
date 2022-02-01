#include "sqMessage.h"

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
  sem_t signal;
} message_t;

static message_t* godot_response;

static bool init_queue(lfqueue_t* queue) {
  // TODO not technically thread-safe
  if (lfqueue_init(queue) == -1) {
    fprintf(stderr, "failed to init queue\n");
    return false;
  }
  return true;
}

bool init_sqmessage() {
  return init_queue(&squeak_queue) && init_queue(&godot_queue);
}

void finish_sqmessage() {
  lfqueue_destroy(&squeak_queue);
  lfqueue_destroy(&godot_queue);
}

void* process_responses(message_t* message) {
  while (true) {
    printf("Waiting for response to message of type %i...\n", message->type);
    printf("message at \t\t%p\n", message);
    fflush(stdout);
    sem_wait(&message->signal);

    /* message_t* message; */
    /* if ((message = lfqueue_single_deq(&godot_queue)) == NULL) { */
    /*   fprintf(stderr, "Godot signalled without message!"); */
    /*   exit(1); */
    /* } */

    printf("Received response at \t%p\n", godot_response);

    switch (godot_response->type) {
      case SQP_GODOT_FINISH_PROCESSING:
        printf("SQP_GODOT_FINISH_PROCESSING\n");
        /* sem_destroy(&message->response->signal); */
        void* result = godot_response->data;
        printf("result pointer: %p\n", result);
        free(godot_response);
        return result;
        break;
      case SQP_GODOT_FUNCTION_CALL:
        printf("SQP_GODOT_FUNCTION_CALL\n");
        // call method
        break;
      default:
        fprintf(stderr, "Godot received message that it can't handle! %i\n", godot_response->type);
        break;
    }

    // TODO: do this only if debug enabled
    godot_response->type = SQP_INVALID_MESSAGE;
  }
}

// we send a message to squeak and wait for it to be handled
void* send_message(enum MessageType type, void* data) {
  message_t m;
  m.type = type;
  m.data = data;
  sem_init(&m.signal, 1, 0);

  if (lfqueue_enq(&squeak_queue, &m) != 0) {
    fprintf(stderr, "failed to enqueue");
  }

  void* response = process_responses(&m);
  sem_destroy(&m.signal);
  return response;
}

// squeak signals us that the message has been processed
void signal_message(message_t *message) {
  sem_post(&message->signal);
}

// next message or NULL if empty
message_t *read_message() {
  return (message_t *) lfqueue_single_deq(&squeak_queue);
}

void squeak_send_response(message_t* message, message_t* response) {
  godot_response = response;
  sem_post(&message->signal);
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
  godot_variant* result;
} method_call_t;

void squeak_call_method(const char* method_name, const godot_object* owner, const godot_variant** args, int arg_count, godot_variant* result) {
  method_call_t data = {
    // is this strdup really necessary here?
    strdup(method_name),
    owner,
    args,
    arg_count,
    result
  };
  send_message(SQP_SQUEAK_FUNCTION_CALL, &data);
  printf("method call result: ");
  free((void*) data.method_name);
}
