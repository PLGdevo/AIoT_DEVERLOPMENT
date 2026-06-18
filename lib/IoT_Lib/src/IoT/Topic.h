#ifndef INC_IoT_TopiC
#define INC_IoT_TopiC
/*
Topic format:
    DEVICE/<MAC ESP>/<prefix topic>
*/
#define BASE_TOPIC "DEVICE/"

/* Subscribe topic */
#define SUB_PREFIX_TELEMETRY_TOPIC "/TELEMETRY"
#define SUB_PREFIX_CONTROL_TOPIC "/CONTROL"
#define SUB_PREFIX_VIRTUAL_TOPIC "/VIRTUAL_PIN/"

/* Publish topic */
#define PUB_PREFIX_TELEMETRY_TOPIC "TELEMETRY"
#define PUB_PREFIX_CONTROL_TOPIC "CONTROL"
#define PUB_PREFIX_MODBUS_DATA_TOPIC "/DATA/"

#endif /*INC_IoT_TopiC*/