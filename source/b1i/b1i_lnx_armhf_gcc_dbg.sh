#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1i b1i armhf gcc dbg "-DCMAKE_BUILD_TYPE=DEBUG" $1
