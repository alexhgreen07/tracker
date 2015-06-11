#!/bin/bash

./bin/core_test.exe -v && \
./bin/database_test.exe -v && \
./bin/network_test.exe -v && \
./bin/app_test.exe -v