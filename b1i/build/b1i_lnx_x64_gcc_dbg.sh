#!/bin/bash

../../common/build/lnx_build.sh "../.." b1i b1i x64 gcc dbg "-DCMAKE_BUILD_TYPE=DEBUG" $1
