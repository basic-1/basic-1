#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1iu b1i armhf gcc rel "-DCMAKE_BUILD_TYPE=RELEASE" $1
