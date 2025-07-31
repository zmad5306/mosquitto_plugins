#define MOSQ_PLUGIN_VERSION 5

#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>

#include <string.h>
#include <jwt.h>

static char jwt_secret[256] = {0};

static int validate_jwt(const char *token)
{
    jwt_t *jwt = NULL;
    if (jwt_decode(&jwt, token, (unsigned char*)jwt_secret, strlen(jwt_secret))) {
        return MOSQ_ERR_AUTH;
    }
    jwt_free(jwt);
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_unpwd_check(void *user_data, struct mosquitto *client,
                               const char *username, const char *password)
{
    if (!password) {
        mosquitto_log_printf(MOSQ_LOG_ERR, "JWT Auth: No token");
        return MOSQ_ERR_AUTH;
    }
    return validate_jwt(password);
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

int mosquitto_plugin_init(mosquitto_plugin_id_t *identifier, void **user_data,
                          struct mosquitto_opt *options, int option_count)
{
    for (int i = 0; i < option_count; i++) {
        if (!strcmp(options[i].key, "jwt_secret")) {
            strncpy(jwt_secret, options[i].value, sizeof(jwt_secret) - 1);
        }
    }
    if (jwt_secret[0] == '\0') {
        mosquitto_log_printf(MOSQ_LOG_ERR, "JWT Auth: Missing secret");
        return MOSQ_ERR_INVAL;
    }
    mosquitto_log_printf(MOSQ_LOG_INFO, "JWT Auth plugin loaded");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_plugin_cleanup(void *user_data, struct mosquitto_opt *options, int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "JWT Auth plugin unloaded");
    return MOSQ_ERR_SUCCESS;
}
