#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>
#include <stdio.h>
#include <string.h>
#include <jwt.h> // libjwt

#define JWT_SECRET "supersecret" // TODO: move to mosquitto.conf options
#define JWT_ALG JWT_ALG_HS256    // HS256 algorithm

int mosquitto_auth_plugin_version(void)
{
    return MOSQ_AUTH_PLUGIN_VERSION;
}

int mosquitto_auth_plugin_init(void **user_data, struct mosquitto_opt *opts, int opt_count)
{
    *user_data = NULL;
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Auth plugin initialized");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_plugin_cleanup(void *user_data, struct mosquitto_opt *opts, int opt_count)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Auth plugin cleanup");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_security_init(void *user_data, struct mosquitto_opt *opts, int opt_count, bool reload)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Security init");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_security_cleanup(void *user_data, struct mosquitto_opt *opts, int opt_count, bool reload)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Security cleanup");
    return MOSQ_ERR_SUCCESS;
}

//int mosquitto_auth_unpwd_check(void *user_data, struct mosquitto *client,
//                               const char *username, const char *password)
//{
//    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Checking username: %s",
//                         username ? username : "(null)");
//    return MOSQ_ERR_SUCCESS;
//}

int mosquitto_auth_unpwd_check(void *user_data, struct mosquitto *client, const char *username, const char *password)
{
    enum mosquitto_protocol proto = mosquitto_client_protocol(client);

    if(proto == mp_websockets)
    {
        mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] WebSocket connection from client: %s",
            mosquitto_client_id(client) ? mosquitto_client_id(client) : "(unknown)");

        // Expect JWT in password field (could also check username)
        if(password == NULL || strlen(password) == 0)
        {
            mosquitto_log_printf(MOSQ_LOG_ERR, "[JWT-AUTH] No JWT token provided");
            return MOSQ_ERR_AUTH;
        }

        // Decode and validate JWT
        jwt_t *jwt = NULL;
        if(jwt_decode(&jwt, password, (unsigned char *)JWT_SECRET, strlen(JWT_SECRET)) != 0)
        {
            mosquitto_log_printf(MOSQ_LOG_ERR, "[JWT-AUTH] Invalid JWT token");
            return MOSQ_ERR_AUTH;
        }

        // Optional: check claims
        const char *sub = jwt_get_grant(jwt, "sub");
        if(!sub)
        {
            mosquitto_log_printf(MOSQ_LOG_ERR, "[JWT-AUTH] Missing 'sub' claim");
            jwt_free(jwt);
            return MOSQ_ERR_AUTH;
        }

        mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] JWT valid, subject: %s", sub);

        jwt_free(jwt);
    }
    else
    {
        mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Non-WebSocket connection from client: %s",
            mosquitto_client_id(client) ? mosquitto_client_id(client) : "(unknown)");
    }

    return MOSQ_ERR_SUCCESS; // allow if passed checks
}

int mosquitto_auth_acl_check(void *user_data, int access, struct mosquitto *client,
                             const struct mosquitto_acl_msg *msg)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] ACL check");
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_disconnect(void *user_data, struct mosquitto *client)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "[JWT-AUTH] Client disconnected");
    return MOSQ_ERR_SUCCESS;
}
