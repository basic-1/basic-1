#!/bin/bash
# <target_name> <project_name> <platform> <compiler> <configuration> <cmake1_options> [<is_next_revision>]

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ] || [ "$6" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

target_name=$1
project_name=$2
platform=$3
compiler=$4
configuration=$5

mkdir ../../build
mkdir ../../build/lnx
mkdir ../../build/lnx/$platform
mkdir ../../build/lnx/$platform/$compiler
mkdir ../../build/lnx/$platform/$compiler/$configuration
mkdir ../../build/lnx/$platform/$compiler/$configuration/$target_name

mkdir ../../bin
mkdir ../../bin/lnx
mkdir ../../bin/lnx/$platform
mkdir ../../bin/lnx/$platform/$compiler
mkdir ../../bin/lnx/$platform/$compiler/$configuration

./get_git_rev.sh "$7"

if [ -f "./lnx_${platform}_${compiler}_env.sh" ]
then
  . ./lnx_${platform}_${compiler}_env.sh
fi

B1I_TARGET=$target_name
export B1I_TARGET

cd ../../build/lnx/$platform/$compiler/$configuration/$target_name

cmake ../../../../../../source/$project_name $6
cmake --build . --target $target_name

cp $target_name ../../../../../../bin/lnx/$platform/$compiler/$configuration

cd ../../../../../../source/$project_name
