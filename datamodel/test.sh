#!/bin/bash

#benchmark backends: -tickcounter -callgrind -eventcounter

LD_LIBRARY_PATH=dist/src:$LD_LIBRARY_PATH ./test/test_datamodel

