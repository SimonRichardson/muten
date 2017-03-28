#!/usr/bin/env bash

if pgrep redis-server > /dev/null
then
    echo "Redis already running... attempting to run tests!"
else
    echo "Starting Redis"
    redis-server --loadmodule $(pwd)/muten.so &
    sleep 1
fi

redis-cli muten.debug "key"

pkill redis-server
