#include <gdnative_api_struct.gen.h>
#include "gdnativeUtils.h"

enum MessageType
{
    SQP_SQUEAK_NEW_SCRIPT = 0,
    SQP_SQUEAK_SCRIPT_RELOAD = 1,
    SQP_SQUEAK_NEW_INSTANCE = 2,
    SQP_SQUEAK_FUNCTION_CALL = 3,
    SQP_SQUEAK_FINISH_PROCESSING = 4,
    SQP_SQUEAK_SET_PROPERTY = 5,
    SQP_SQUEAK_GET_PROPERTY = 6,
    SQP_INITIALIZE = 7,

    SQP_GODOT_FINISH_PROCESSING = 100,
    SQP_GODOT_FUNCTION_CALL = 101,

    SQP_INVALID_MESSAGE = -1,
};

bool init_sqmessage();
void send_message(enum MessageType type, godot_variant *data, godot_variant *response);
