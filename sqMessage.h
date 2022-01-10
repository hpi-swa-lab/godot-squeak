#include <semaphore.h>

enum MessageType {
	SIGNAL,
	INSTANCE,
};

typedef union {
	struct {
		long a;
		long b;
		char *str;
	} request;
	struct {
		long result;
	} response;
} instance_message_data_t;

void send_message(enum MessageType type, void *data);
