#!/bin/bash

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

#temporary directory to build package in
#tmp_dir=./
tmp_dir=/tmp/

#get command line arguments
project_name=$1
platform=$2
lnx_arch=$3
compiler=$4

#get interpreter version
version=$(cat "../$project_name/version.h" | tr -d '\r' | tr -d '\n')
if [ $? -ne 0 ]
then
  echo "get version error"
  exit 1
fi
version=${version/"#define"/}
version=${version/"B1_INT_VERSION"/}
version=${version// /}
version=${version//'"'/}

#get build number
build_num=`git rev-list --count --first-parent HEAD`
if [ $? -ne 0 ]
then
  echo "get build number error"
  exit 1
fi

if [ "$5" = "next" ]
then
  ((build_num++))
fi

#create directories for tar.gz package
mkdir -p -m 775 $tmp_dir$project_name
mkdir -p -m 775 $tmp_dir$project_name/bin
mkdir -p -m 775 $tmp_dir$project_name/share
mkdir -p -m 775 $tmp_dir$project_name/share/$project_name
mkdir -p -m 775 $tmp_dir$project_name/share/$project_name/doc

#copy executable modules, samples, docs and README
cp -a ../../bin/lnx/$platform/$compiler/rel/. $tmp_dir$project_name/bin/
cp -a ../../docs $tmp_dir$project_name/share/$project_name/doc/
cp -a ../../samples $tmp_dir$project_name/share/$project_name/doc/
cp ../../README.md $tmp_dir$project_name/share/$project_name/doc/
cp ../../LICENSE $tmp_dir$project_name/share/$project_name/doc/

#strip copied executables
${6}strip --strip-all $tmp_dir$project_name/bin/b1i
${6}strip --strip-all $tmp_dir$project_name/bin/b1iu

#correct file attributes
chmod 755 `find $tmp_dir$project_name -type d`
chmod 755 $tmp_dir$project_name/bin/*
chmod 644 `find $tmp_dir$project_name/share/$project_name -type f`

orig_path=`pwd`
cd $tmp_dir

#create tar.gz package
tar -zcvf $project_name.tar.gz $project_name

#delete all the stuff we collected for the package
rm -r $project_name

cd $orig_path

#delete package with the same name from distr directory
rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-*.tar.gz

#move the package to distr directory
mv $tmp_dir$project_name.tar.gz ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-${build_num}.tar.gz
