#!bin/bash

scp -o "StrictHostKeyChecking=no" cat/s21_cat jake@192.168.100.15:/usr/local/bin
scp -o "StrictHostKeyChecking=no" grep/s21_grep jake@192.168.100.15:/usr/local/bin