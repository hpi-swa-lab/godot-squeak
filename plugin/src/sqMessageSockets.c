#define SOCKETS 1
#if SOCKETS

#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#include "sqMessageSockets.h"
#include "marshalls.h"

int socket_fd;

void read_response(godot_variant *data)
{
    uint32_t type, total;
    read_header(&type, &total);
    int len = total;

    uint8_t *buffer = calloc(total, sizeof(uint8_t));
    size_t received = 0;
    do
    {
        size_t bytes = read(socket_fd, buffer + received, total - received);
        if (bytes < 0)
        {
            fprintf(stderr, "error reading data from socket\n");
        }
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    if (len > 0)
    {
        decode_variant(&data, buffer, len, NULL, false, 0);
    }
    else
    {
        godot_variant_new_nil(&data);
    }
    free(buffer);

    return data;
}

void read_header(uint32_t *type, uint32_t *data_len)
{
    uint8_t buffer[8];
    size_t total = 8;
    size_t received = 0;
    do
    {
        size_t bytes = read(socket_fd, buffer + received, total - received);
        if (bytes < 0)
        {
            fprintf(stderr, "error reading data from socket\n");
        }
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    *type = buffer[0];
    *data_len = buffer[sizeof(uint32_t)];
}

void send_message(enum MessageType type, godot_variant *data, godot_variant *response)
{
    int data_len;
    encode_variant(data, NULL, &data_len, false, 0);
    uint32_t message_size = sizeof(uint32_t) * 2 + sizeof(uint8_t) * data_len;
    uint8_t *buffer = calloc(sizeof(uint8_t), message_size);
    buffer[0] = type;
    // TODO assert ranges
    buffer[sizeof(uint32_t)] = data_len;
    encode_variant(data, buffer + sizeof(uint32_t) * 2, &data_len, false, 0);

    printf("SENDING %d %d %d\n", type, data_len, message_size);
    if (send(socket_fd, buffer, message_size, 0) < 0)
    {
        perror("Send failed");
    }

    free(buffer);

    read_response(response);
}

bool init_sqmessage()
{
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
    server.sin_port = htons(atoi(port ? port : "9966"));
    if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return false;
    }
    return true;
}

char *squeak_new_script(const char *script_name, const char *parent_name)
{
    printf("NEW SCRIPT\n");
    return NULL;
}

script_description_t *squeak_reload_script(const char *script_path, const char *script_source)
{
    script_description_t *script = calloc(1, sizeof(script_description_t));
    return script;
}

void squeak_new_instance(const char *script_path, const godot_object *owner)
{
    printf("NEW INSTANCE\n");
}
void squeak_call_method(const char *method_name, const godot_object *owner, const godot_variant **args, int arg_count, godot_variant_call_error *error, godot_variant *result)
{
    printf("CALL METHOD\n");
}
// TODO: consider returning a bool directly instead of a pointer
bool *squeak_set_property(const char *property_name, const godot_object *owner, const godot_variant *value)
{
    printf("SET PROPERTY\n");
    return NULL;
}
bool *squeak_get_property(const char *property_name, const godot_object *owner, godot_variant *out)
{
    printf("GET PROPERTY\n");
    return NULL;
}

void squeak_initialize_environment(bool in_editor)
{
    printf("INITIALIZE ENVIRONMENT\n");
    godot_variant data, response;
    godot_variant_new_nil(&data);

    send_message(SQP_INITIALIZE, &data, &response);
}

#endif