#!/bin/bash

C_FILE=$1
EXE_FILE=$2
shift
shift

echo ${C_FILE}

CC_FLAGS="-g -rdynamic"
AT_LEAST_ONE_COMPILER=0

# tcc doesn't understand "no return" and gives spurious warnings

for CC in clang gcc tcc ; do
    echo
    if which ${CC} >/dev/null ; then
        AT_LEAST_ONE_COMPILER=1
        echo "-- Bulding and running ${C_FILE} with ${CC} --"
        ${CC} ${CC_FLAGS} ${C_FILE} -o build/${EXE_FILE}
        if [[ $? != 0 ]] ; then
            echo "-- failed to compile test case with " ${CC}
            exit 1
        fi
        ./build/${EXE_FILE} $*
        if [[ $? != 0 ]] ; then
            echo "-- the built binary build/${EXE_FILE} returned non-zero return code"
            exit 1
        fi
    else
        echo "-- (${CC} does not appears to be installed). Skipping. --"
    fi
done

if [[ ${AT_LEAST_ONE_COMPILER} == 0 ]] ; then
    echo "ERROR: A C compiler was not found so this test was not run!"
    exit 1
fi

exit 0
