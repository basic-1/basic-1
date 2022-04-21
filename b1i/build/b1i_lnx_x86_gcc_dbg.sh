#!/bin/bash

../../common/build/lnx_build.sh "../.." b1i b1i x86 gcc dbg "-DCMAKE_BUILD_TYPE=DEBUG" $1
