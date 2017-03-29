#!/usr/bin/env bash

if pgrep redis-server > /dev/null
then
    echo "Redis already running... attempting to run tests!"
else
    echo "Starting Redis"
    redis-server --loadmodule $(pwd)/muten.so &
    sleep 1
fi

redis-cli muten.store.debug "58dbb63dffa724bcd65da73e" "58dbb6524c6adfbb8b583824"

redis-cli muten.store.insert "58dbb63dffa724bcd65da73e" "58dbb6524c6adfbb8b583824" 1 "58dbb6524c6adfbb8b583824" "data"

pkill redis-server
