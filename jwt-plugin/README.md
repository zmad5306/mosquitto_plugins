`mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf -v`

`gcc -fPIC -shared -o build/jwt_auth.so jwt_auth.c -ljwt -ljansson`
