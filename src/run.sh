#!/bin/bash
make clean

make

echo 'proxy server launch ...'

./main

while true
do
    sleep 1
done
