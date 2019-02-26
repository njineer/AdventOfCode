#!/usr/bin/env bash

if [ -z "$1" ]; then
    echo "Missing src"
elif [ -z "$2" ]; then
    echo "Missing destination"
else
    cp -r $1 $2
    cd $2
    rm Makefile
    ln -s ../Makefile .
fi
