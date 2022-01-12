#include <stdio.h>
#include <sys/time.h>
#include "lfqueue/lfqueue.h"
#include "sqMessage.h"

static lfqueue_t queue;
static int is_init = 0;

typedef struct {
	enum MessageType type;
	message_data_t *data;
	sem_t signal;
} message_t;

static int init_messages() {
	// TODO not technically thread-safe
	is_init = 1;
	if (lfqueue_init(&queue) == -1) {
		fprintf(stderr, "failed to init queue\n");
		return 0;
	}
	return 1;
}

// we send a message to squeak and wait for it to be handled
void send_message(enum MessageType type, message_data_t *data) {
	if (!is_init) {
		init_messages();
	}

	message_t *m = (message_t *) calloc(1, sizeof(message_t));
	m->type = type;
	m->data = data;
	sem_init(&m->signal, 1, 0);

	if (lfqueue_enq(&queue, m) != 0) {
		fprintf(stderr, "failed to enqueue");
	}

	// wait for our message to be handled
	sem_wait(&m->signal);

	// the response is in the separately-allocated data pointer
	free(m);
}

// squeak signals us that the message has been processed
void signal_message(message_t *message) {
	sem_post(&message->signal);
}

// next message or NULL if empty
message_t *read_message() {
	if (!is_init) {
		init_messages();
	}

	return (message_t *) lfqueue_single_deq(&queue);
}

/***************
 * Some helpers for testing, remove once no longer needed
 */

void timed_call() {
	struct timeval start, end;
	gettimeofday(&start, NULL);
	/* send_message(SIGNAL, NULL); */
	gettimeofday(&end, NULL);

	printf("delta microseconds: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
}

void complicated_timed_call() {
  message_data_t *data = malloc(sizeof(message_data_t));
  /* data->function_call.a = 1; */
  send_message(SQP_NEW_SCRIPT, data);
  send_message(SQP_NEW_INSTANCE, data);
  send_message(SQP_FUNCTION_CALL, data);
  /* send_message(234, data); */
  free(data);
	/* struct timeval start, end; */

	/* instance_message_data_t *data = malloc(sizeof(instance_message_data_t)); */
	/* data->request.a = 4; */
	/* data->request.b = 2; */
	/* data->request.str = "hello"; */

	/* gettimeofday(&start, NULL); */
	/* send_message(INSTANCE, data); */
	/* gettimeofday(&end, NULL); */
	/* printf("[complicated] delta microseconds: %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))); */

    /* printf("Result: %ld\n", data->response.result); */
	/* free(data); */
}

