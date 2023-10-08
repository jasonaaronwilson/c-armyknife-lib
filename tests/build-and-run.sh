#!/bin/bash

C_FILE=$1
EXE_FILE=$2
shift
shift

echo ${C_FILE}

# CC=gcc
CC=clang
CC_FLAGS="-g -rdynamic"

${CC} ${CC_FLAGS} ${C_FILE} -o build/${EXE_FILE}

if [[ $? != 0 ]] ; then
    echo "-- failed to compile test case."
    exit 1
fi

./build/${EXE_FILE} $*

if [[ $? != 0 ]] ; then
    echo "-- the built binary build/${EXE_FILE} returned non-zero return code"
    exit 1
fi

exit 0
