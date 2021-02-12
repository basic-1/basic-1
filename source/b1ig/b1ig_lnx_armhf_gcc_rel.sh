#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1ig b1ig armhf gcc rel "-DCMAKE_BUILD_TYPE=RELEASE" $1
