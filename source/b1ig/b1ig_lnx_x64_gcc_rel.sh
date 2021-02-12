#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1ig b1ig x64 gcc rel "-DCMAKE_BUILD_TYPE=RELEASE" $1
