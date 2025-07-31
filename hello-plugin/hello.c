#include "mosquitto_broker.h"   // Broker plugin API
#include "mosquitto_plugin.h"   // Plugin registration API
#include "mosquitto.h"          // Client API for MOSQ_LOG_INFO, MOSQ_ERR_SUCCESS
#include <stdio.h>

int mosquitto_plugin_version(int supported_version_count, const int *supported_versions)
{
    for (int i = 0; i < supported_version_count; i++) {
        if (supported_versions[i] == 5) { // API version 5
            return 5;
        }
    }
    return -1;
}

int mosquitto_plugin_init(mosquitto_plugin_id_t *identifier, void **userdata,
                          struct mosquitto_opt *options, int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "Hello from my Mosquitto plugin!");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_cleanup(void *userdata,
                             struct mosquitto_opt *options, int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "Goodbye from my Mosquitto plugin!");
    return MOSQ_ERR_SUCCESS;
}
