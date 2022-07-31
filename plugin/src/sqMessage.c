#include "sqMessage.h"

#include "gdnativeUtils.h"
#include <lfqueue/lfqueue.h>

#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>
#define __USE_XOPEN_EXTENDED
#include <string.h>

static lfqueue_t squeak_queue;
static lfqueue_t godot_queue;

typedef struct
{
  int32_t type; // enum MessageType
  void *data;
} message_t;

#if SOCKETS
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
int socket_fd;
#else
// TODO: volatile?
static message_t *godot_inbox;
static message_t *squeak_inbox;

static sem_t godot_message_signal;
static sem_t squeak_message_signal;
#endif

// Squeak FFI API functions
#if !SOCKETS
void squeak_send_message(message_t *message)
{
  godot_inbox = message;
  sem_post(&godot_message_signal);
}

message_t *squeak_read_message_blocking()
{
  sem_wait(&squeak_message_signal);
  return squeak_inbox;
}

// next message or NULL if empty
message_t *read_message()
{
  return (message_t *)lfqueue_single_deq(&squeak_queue);
}
#endif

void godot_send_message(message_t *message)
{
#if SOCKETS
  char *data = "test";
  printf("SENDING\n");
  if (send(socket_fd, data, strlen(data), 0) < 0)
  {
    perror("Send failed");
  }
  printf("SENT\n");
#else
  squeak_inbox = message;
  sem_post(&squeak_message_signal);
#endif
}

static bool init_queue(lfqueue_t *queue)
{
  // TODO not technically thread-safe
  if (lfqueue_init(queue) == -1)
  {
    fprintf(stderr, "failed to init queue\n");
    return false;
  }
  return true;
}

bool init_sqmessage()
{
#if SOCKETS
  void *sym = dlsym(0, "encode_variant");
  printf("%p\n", sym);
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1)
  {
    perror("creating socket");
    return false;
  }

  struct sockaddr_in server;
  char *host = getenv("SQ_HOST");
  char *port = getenv("SQ_PORT");
  server.sin_addr.s_addr = inet_addr(host ? host : "127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(port ? port : "8000"));
  if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("connect");
    return false;
  }
#else
  // TODO handle failure cases
  sem_init(&godot_message_signal, 0, 0);
  sem_init(&squeak_message_signal, 0, 0);
#endif
  return init_queue(&squeak_queue) && init_queue(&godot_queue);
}

void finish_sqmessage()
{
#if SOCKETS
  close(socket_fd);
#else
  sem_destroy(&godot_message_signal);
  sem_destroy(&squeak_message_signal);
#endif
  lfqueue_destroy(&squeak_queue);
  lfqueue_destroy(&godot_queue);
}

typedef struct
{
  godot_string *method_name;
  godot_variant *self;
  const godot_variant **args;
  int arg_count;
  godot_variant_call_error *error;
  godot_variant *result;
} godot_method_call_t;

static int processing_stack_count = 0;

void *process_responses(message_t *message)
{
  ++processing_stack_count;

  while (true)
  {
    /* printf("Waiting for response to message of type %i...\n", message->type); */
#if SOCKETS
    char response_data[4096];
    memset(response_data, 0, sizeof(response_data));
    size_t total = sizeof(response_data) - 1;
    size_t received = 0;
    do
    {
      size_t bytes = read(socket_fd, response_data + received, total - received);
      if (bytes < 0)
      {
        fprintf(stderr, "error reading data from socket\n");
      }
      if (bytes == 0)
        break;
      received += bytes;
    } while (received < total);
    if (received == total)
    {
      fprintf(stderr, "message was too long to receive\n");
    }
    message_t *response = NULL;
#else
    sem_wait(&godot_message_signal);

    message_t *response = godot_inbox;
#endif

    switch (response->type)
    {
    case SQP_GODOT_FINISH_PROCESSING:;
      /* printf("SQP_GODOT_FINISH_PROCESSING\n"); */
      void *result = response->data;
      /* free(response); */
      --processing_stack_count;
      return result;
      break;
    case SQP_GODOT_FUNCTION_CALL:;
      /* printf("SQP_GODOT_FUNCTION_CALL\n"); */
      godot_method_call_t *data = (godot_method_call_t *)response->data;
      /* printf("Squeak called Godot function: %s\n", godot_string_to_c_str(data->method_name)); */
      *data->result = godot_call_variant(data->self, data->method_name, data->args, data->arg_count, data->error);
      message_t *r = (message_t *)malloc(sizeof(message_t));
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

bool is_currently_processing()
{
  return processing_stack_count > 0;
}

// we send a message to squeak and wait for it to be handled
void *send_message(enum MessageType type, void *data)
{
  message_t m;
  m.type = type;
  m.data = data;

  if (is_currently_processing())
  {
    /* printf("Currently processing, using signalling mechanism\n"); */
    printf("GO\n");
    godot_send_message(&m);
  }
  else
  {
    /* printf("Not currently processing, using busy wait mechanism\n"); */
    if (lfqueue_enq(&squeak_queue, &m) != 0)
    {
      fprintf(stderr, "failed to enqueue");
    }
  }

  return process_responses(&m);
}

void destroy_script_description(script_description_t *description)
{
  for (int i = 0; i < description->functions.num_names; ++i)
  {
    free(description->functions.names[i]);
  }
  for (int i = 0; i < description->signals.num_names; ++i)
  {
    free(description->signals.names[i]);
  }
  for (int i = 0; i < description->properties.num; ++i)
  {
    free(description->properties.properties[i].name);
  }
  free(description->properties.properties);
}

typedef struct
{
  const char *script_name;
  const char *parent_name;
} new_script_t;

char *squeak_new_script(const char *script_name, const char *parent_name)
{
  new_script_t data = {
      script_name,
      parent_name};
  return send_message(SQP_SQUEAK_NEW_SCRIPT, &data);
}

typedef struct
{
  const char *script_path;
  const char *script_source;
} script_reload_t;

script_description_t *squeak_reload_script(const char *script_path, const char *script_source)
{
  script_reload_t data = {
      script_path,
      script_source};
  return send_message(SQP_SQUEAK_SCRIPT_RELOAD, &data);
}

typedef struct
{
  const char *script_path;
  const godot_object *owner;
} new_instance_t;

void squeak_new_instance(const char *script_path, const godot_object *owner)
{
  new_instance_t data = {
      script_path,
      owner};
  send_message(SQP_SQUEAK_NEW_INSTANCE, &data);
}

typedef struct
{
  const char *method_name;
  const godot_object *owner;
  const godot_variant **args;
  int arg_count;
  godot_variant_call_error *error;
  godot_variant *result;
} method_call_t;

void squeak_call_method(const char *method_name, const godot_object *owner, const godot_variant **args, int arg_count, godot_variant_call_error *error, godot_variant *result)
{
  method_call_t data = {
      // is this strdup really necessary here?
      strdup(method_name),
      owner,
      args,
      arg_count,
      error,
      result};
  send_message(SQP_SQUEAK_FUNCTION_CALL, &data);
  /* printf("method call result: "); */
  free((void *)data.method_name);
}

typedef struct
{
  const char *property_name;
  const godot_object *owner;
  const godot_variant *value;
} set_property_t;

bool *squeak_set_property(const char *property_name, const godot_object *owner, const godot_variant *value)
{
  set_property_t data = {
      property_name,
      owner,
      value};
  return send_message(SQP_SQUEAK_SET_PROPERTY, &data);
}

typedef struct
{
  const char *property_name;
  const godot_object *owner;
  godot_variant *out;
} get_property_t;

bool *squeak_get_property(const char *property_name, const godot_object *owner, godot_variant *out)
{
  get_property_t data = {
      property_name,
      owner,
      out};
  return send_message(SQP_SQUEAK_GET_PROPERTY, &data);
}

typedef struct
{
  bool in_editor;
} initialize_environment_t;

void squeak_initialize_environment(bool in_editor)
{
#if SOCKETS
  godot_variant data = godot_variant_new_bool(in_editor);
#else
  initialize_environment_t data = {
      in_editor};
#endif
  send_message(SQP_INITIALIZE, &data);
}
