#!/bin/bash
# <project_name> <platform> <compiler> <configuration> <cmake1_options> <is_next_revision>

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

project_name=$1
platform=$2
compiler=$3
configuration=$4

mkdir ../../build
mkdir ../../build/lnx
mkdir ../../build/lnx/$platform
mkdir ../../build/lnx/$platform/$compiler
mkdir ../../build/lnx/$platform/$compiler/$configuration
mkdir ../../build/lnx/$platform/$compiler/$configuration/$project_name

mkdir ../../bin
mkdir ../../bin/lnx
mkdir ../../bin/lnx/$platform
mkdir ../../bin/lnx/$platform/$compiler
mkdir ../../bin/lnx/$platform/$compiler/$configuration

./get_git_rev.sh "$6"

if [ -f "./lnx_${platform}_${compiler}_env.sh" ]
then
  . ./lnx_${platform}_${compiler}_env.sh
fi

cd ../../build/lnx/$platform/$compiler/$configuration/$project_name

cmake ../../../../../../source/$project_name $5
cmake --build . --target $project_name

cp $project_name ../../../../../../bin/lnx/$platform/$compiler/$configuration

cd ../../../../../../source/$project_name
