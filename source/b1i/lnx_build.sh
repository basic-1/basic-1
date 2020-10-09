#!/bin/bash
# <root_path> <target_name> <project_name> <platform> <compiler> <configuration> <cmake1_options> [<is_next_revision>]

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ] || [ "$6" = "" ] || [ "$7" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

target_name=$2
project_name=$3
platform=$4
compiler=$5
configuration=$6

mkdir $1/build
mkdir $1/build/lnx
mkdir $1/build/lnx/$platform
mkdir $1/build/lnx/$platform/$compiler
mkdir $1/build/lnx/$platform/$compiler/$configuration
mkdir $1/build/lnx/$platform/$compiler/$configuration/$target_name

mkdir $1/bin
mkdir $1/bin/lnx
mkdir $1/bin/lnx/$platform
mkdir $1/bin/lnx/$platform/$compiler
mkdir $1/bin/lnx/$platform/$compiler/$configuration

if [ -f "./get_git_rev.sh" ]
then
  . ./get_git_rev.sh "$8"
fi

if [ -f "./lnx_${platform}_${compiler}_env.sh" ]
then
  . ./lnx_${platform}_${compiler}_env.sh
fi

B1I_TARGET=$target_name
export B1I_TARGET

cd $1/build/lnx/$platform/$compiler/$configuration/$target_name

cmake ../../../../../../source/$project_name $7
cmake --build . --target $target_name

cp $target_name ../../../../../../bin/lnx/$platform/$compiler/$configuration

cd ../../../../../../source/$project_name
