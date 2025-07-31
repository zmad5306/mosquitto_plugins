#include "mosquitto_broker.h"
#include "mosquitto_plugin.h"
#include "mosquitto.h"
#include <stdio.h>
#include <string.h>

static mosquitto_plugin_id_t *plugin_id = NULL;

static int on_message(int event, void *event_data, void *userdata)
{
    struct mosquitto_evt_message *ed = event_data;
    mosquitto_log_printf(MOSQ_LOG_INFO, "[PLUGIN] Message on topic '%s': %.*s",
                         ed->topic,
                         ed->payloadlen,
                         (const char *)ed->payload);
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_version(int supported_version_count, const int *supported_versions)
{
    for (int i = 0; i < supported_version_count; i++) {
        if (supported_versions[i] == 5) {
            return 5;
        }
    }
    return -1;
}

int mosquitto_plugin_init(mosquitto_plugin_id_t *identifier, void **userdata,
                          struct mosquitto_opt *options, int option_count)
{
    plugin_id = identifier;
    mosquitto_log_printf(MOSQ_LOG_INFO, "[PLUGIN] Hello from my Mosquitto plugin!");

    // Register for message events
    mosquitto_callback_register(plugin_id, MOSQ_EVT_MESSAGE, on_message, NULL, NULL);

    return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_cleanup(void *userdata,
                             struct mosquitto_opt *options, int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[PLUGIN] Goodbye from my Mosquitto plugin!");

    // Unregister callbacks
    mosquitto_callback_unregister(plugin_id, MOSQ_EVT_MESSAGE, on_message, NULL);

    return MOSQ_ERR_SUCCESS;
}
