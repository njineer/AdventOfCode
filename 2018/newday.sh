#!/usr/bin/env bash

if [ -z "$1" ]; then
    echo "Missing destination"
else
    cp -r day2 $1
    cd $1
    rm Makefile
    ln -s ../Makefile .
fi
