#!/bin/bash

#usage: ./lnx_tar_gz_distr.sh <project_name> <platform> <lnx_arch> <compiler> <extra_projects> [<next>]
#<project_name>=b1i|b1ig
#<platform>=x86|x64|armhf
#<lnx_arch>=i386|amd64|armhf
#<compiler>=gcc

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

#temporary directory to build package in
#tmp_dir=./
tmp_dir=/tmp/

#make install directory name not depending on project_name argument
proj_dir_name=b1i

#get command line arguments
project_name=$1
platform=$2
lnx_arch=$3
compiler=$4
extra_projects=$5

#get wxWidgets libraries path
if [ "$project_name" = "b1ig" ]
then
  if [ -f "../../env/lnx_${platform}_${compiler}_env.sh" ]
  then
    . ../../env/lnx_${platform}_${compiler}_env.sh
    wx_libs_dir=`/bin/sh $B1_WX_CONFIG --libs |grep -P -o -e '-L.*?\s' |cut -c 3- |tr -d ' '`
  fi
fi

#get interpreter version
if [ -f "../../common/source/version.h" ]
then
  version=$(cat "../../common/source/version.h" | tr -d '\r' | tr -d '\n')
  if [ "$version" = "" ]
  then
    echo "get version error"
    exit 1
  fi
else
  echo "get version error"
  exit 1
fi
version=${version/"#define"/}
version=${version/"B1_CORE_VERSION"/}
version=${version// /}
version=${version//'"'/}

#get build number
if [ -f "../../common/source/gitrev.h" ]
then
  build_num=$(cat "../../common/source/gitrev.h" | tr -d '\r' | tr -d '\n')
  if [ "$build_num" = "" ]
  then
    echo "no build number found"
  else
    build_num=${build_num/"#define"/}
    build_num=${build_num/"B1_GIT_REVISION"/}
    build_num=${build_num// /}
    build_num=${build_num//'"'/}

#do not increase current revision number from gitrev.h
#    if [ "$6" = "next" ]
#    then
#      ((build_num++))
#    fi
  fi
else
  echo "no build number found"
fi

#create directories for tar.gz package
mkdir -p -m 775 $tmp_dir$proj_dir_name
mkdir -p -m 775 $tmp_dir$proj_dir_name/local
mkdir -p -m 775 $tmp_dir$proj_dir_name/local/bin
mkdir -p -m 775 $tmp_dir$proj_dir_name/local/share
mkdir -p -m 775 $tmp_dir$proj_dir_name/local/share/$proj_dir_name
mkdir -p -m 775 $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc

#create lib direcory only if wxWidgets libraries directory is specified
if [ "$project_name" = "b1ig" ]
then
  if [ "$wx_libs_dir" != "" ]
  then
    mkdir -p -m 775 $tmp_dir$proj_dir_name/local/lib
    mkdir -p -m 775 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name
  fi

#create directory for menu shortcut
  mkdir -p -m 775 $tmp_dir$proj_dir_name/share
  mkdir -p -m 775 $tmp_dir$proj_dir_name/share/applications
fi


#copy executable modules, samples, docs and README
cp ../../bin/lnx/$platform/$compiler/rel/b1i $tmp_dir$proj_dir_name/local/bin/
cp ../../bin/lnx/$platform/$compiler/rel/b1iu $tmp_dir$proj_dir_name/local/bin/

if [ "$project_name" = "b1ig" ]
then
  cp ../../bin/lnx/$platform/$compiler/rel/b1ig $tmp_dir$proj_dir_name/local/bin/

#copy wxWidgets libraries only if the path is specified
  if [ "$wx_libs_dir" != "" ]
  then
    cp $wx_libs_dir/libwx_baseu*.so.4.0.0 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*core*.so.4.0.0 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*aui*.so.4.0.0 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*stc*.so.4.0.0 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_baseu*.so.4 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*core*.so.4 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*aui*.so.4 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*stc*.so.4 $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/

    unlink $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_baseu*net*.so.4
    unlink $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_baseu*xml*.so.4
    unlink $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_baseu*net*.so.4.0.0
    unlink $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_baseu*xml*.so.4.0.0
  fi
fi

\cp -af ../../b1core/docs $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
\cp -af ../../common/docs $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
if [ ${extra_projects} != "" ]
then
  \cp -af ../../${extra_projects}/docs $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
fi
\cp -af ../../${project_name}/docs $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/

\cp -af ../../b1core/samples $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
\cp -af ../../common/samples $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
if [ ${extra_projects} != "" ]
then
  \cp -af ../../${extra_projects}/samples $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
fi
\cp -af ../../${project_name}/samples $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/

cp ../../README.md $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/
cp ../../LICENSE $tmp_dir$proj_dir_name/local/share/$proj_dir_name/doc/

if [ "$project_name" = "b1ig" ]
then
  cp ../../b1ig/source/b1ig.desktop $tmp_dir$proj_dir_name/share/applications
fi

#strip copied executables
${7}strip --strip-all $tmp_dir$proj_dir_name/local/bin/b1i
${7}strip --strip-all $tmp_dir$proj_dir_name/local/bin/b1iu
if [ "$project_name" = "b1ig" ]
then
  ${7}strip --strip-all $tmp_dir$proj_dir_name/local/bin/b1ig
  if [ "$wx_libs_dir" != "" ]
  then
    ${7}strip --strip-all $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_baseu*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_gtk3u*core*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_gtk3u*aui*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/local/lib/$proj_dir_name/libwx_gtk3u*stc*.so.4.0.0
  fi
fi

#correct file attributes
chmod 755 `find $tmp_dir$proj_dir_name -type d`
chmod 755 $tmp_dir$proj_dir_name/local/bin/*
chmod 644 `find $tmp_dir$proj_dir_name/local/share/$proj_dir_name -type f`
if [ "$project_name" = "b1ig" ]
then
  if [ "$wx_libs_dir" != "" ]
  then
    chmod 644 `find $tmp_dir$proj_dir_name/local/lib -type f`
  fi

  chmod 644 `find $tmp_dir$proj_dir_name/share/applications -type f`
fi

orig_path=`pwd`
cd $tmp_dir

#create tar.gz package
tar -zcvf $project_name.tar.gz $proj_dir_name

#delete all the stuff we collected for the package
rm -r $proj_dir_name

cd $orig_path

#delete package with the same name from distr directory
if [ "$build_num" = "" ]
then
  rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}.tar.gz
else
  rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-*.tar.gz
fi

#move the package to distr directory
if [ "$build_num" = "" ]
then
  mv $tmp_dir$project_name.tar.gz ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}.tar.gz
else
  mv $tmp_dir$project_name.tar.gz ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-${build_num}.tar.gz
fi