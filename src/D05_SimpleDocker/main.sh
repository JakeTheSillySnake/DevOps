#!/bin/bash

gcc ./server/server.c -lfcgi -o ./server/server.fcgi
spawn-fcgi -a 127.0.0.1 -p 8080 ./server/server.fcgi
