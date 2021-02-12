#!/bin/bash

../b1icommon/lnx_build.sh "../.." b1i b1i x64 gcc rel "-DCMAKE_BUILD_TYPE=RELEASE" $1
