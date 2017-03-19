#!/usr/bin/env bash

if pgrep redis-server > /dev/null
then
    echo "Redis already running... attempting to run tests!"
else
    echo "Starting Redis"
    redis-server --loadmodule ../muten.so &
    sleep 1
fi

redis-cli mut.debug "key"

pkill redis-server
