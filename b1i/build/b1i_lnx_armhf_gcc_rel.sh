#!/bin/bash

../../common/build/lnx_build.sh "../.." b1i b1i armhf gcc rel "-DCMAKE_BUILD_TYPE=RELEASE" $1
