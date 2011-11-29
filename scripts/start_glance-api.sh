#!/bin/bash

ps aux | grep [b]in/glance-api
if [ "$?" -eq 1 ]; then
    cd /data/stack/glance && bin/glance-api --config-file=etc/glance-api.conf &
fi
