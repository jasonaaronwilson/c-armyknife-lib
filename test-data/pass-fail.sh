#!/bin/bash

if [[ $2 == "0" ]] ; then
    echo PASS $1
    exit 0
else
    echo FAIL $1
    exit $2
fi
