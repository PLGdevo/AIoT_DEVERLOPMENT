#ifndef INC_IoT_TopiC
#define INC_IoT_TopiC
/*
Topic format:
    device/<MAC ESP>/<prefix topic>
*/
#define BASE_TOPIC "device/"

/* Subscribe topic */
#define SUB_PREFIX_TELEMETRY_TOPIC "/telemetry"
#define SUB_PREFIX_CONTROL_TOPIC "/control"
#define SUB_PREFIX_VIRTUAL_TOPIC "/virtual_pin/"

/* Publish topic */
#define PUB_PREFIX_TELEMETRY_TOPIC "telemetry"
#define PUB_PREFIX_CONTROL_TOPIC "control"
#define PUB_PREFIX_MODBUS_DATA_TOPIC "/data/"

#endif /*INC_IoT_TopiC*/