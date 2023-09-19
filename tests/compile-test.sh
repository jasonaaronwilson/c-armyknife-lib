#!/bin/bash

CC=gcc
CC=clang

${CC} compiler-test.c -o build/compiler-test

if [[ $? != 0 ]] ; then
    echo "-- gcc failed to compile a trivial use case."
    exit 1
fi

./build/compiler-test
if [[ $? != 0 ]] ; then
    echo "-- the built binary build/compiler-test returned non-zero return code"
    exit 1
fi

exit 0
