#!/bin/bash

target=$1

LD_LIBRARY_PATH=./src:$LD_LIBRARY_PATH ./test/$target/$target

