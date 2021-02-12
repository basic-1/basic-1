#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1iu b1i x64 gcc dbg "-DCMAKE_BUILD_TYPE=DEBUG" $1
