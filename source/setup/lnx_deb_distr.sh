#!/bin/bash

#usage: ./lnx_deb_distr.sh <project_name> <platform> <lnx_arch> <compiler> <depends> [<next>]
#<project_name>=b1i|b1ig
#<platform>=x86|x64|armhf
#<lnx_arch>=i386|amd64|armhf
#<compiler>=gcc

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

#temporary directory to build package in
#tmp_dir=./
tmp_dir=/tmp/

proj_dir_name=b1i

#get command line arguments
project_name=$1
platform=$2
lnx_arch=$3
compiler=$4
depends=$5

#get wxWidgets libraries path
if [ "$project_name" = "b1ig" ]
then
  if [ -f "../b1icommon/lnx_${platform}_${compiler}_env.sh" ]
  then
    . ../b1icommon/lnx_${platform}_${compiler}_env.sh
    wx_libs_dir=`/bin/sh $B1_WX_CONFIG --libs |grep -P -o -e '-L.*?\s' |cut -c 3- |tr -d ' '`
  fi
fi

#get interpreter version
if [ -f "../b1icommon/version.h" ]
then
  version=$(cat "../b1icommon/version.h" | tr -d '\r' | tr -d '\n')
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
version=${version/"B1_INT_VERSION"/}
version=${version// /}
version=${version//'"'/}

#get build number
if [ -f "../b1icommon/gitrev.h" ]
then
  build_num=$(cat "../b1icommon/gitrev.h" | tr -d '\r' | tr -d '\n')
  if [ "$build_num" = "" ]
  then
    echo "get build number error"
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
  echo "get build number error"
fi

#create directories for deb package
mkdir -p -m 775 $tmp_dir$proj_dir_name
mkdir -p -m 775 $tmp_dir$proj_dir_name/DEBIAN
mkdir -p -m 775 $tmp_dir$proj_dir_name/usr
mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/bin
mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/share
mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/share/doc
mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name

#create lib direcory only if wxWidgets libraries directory is specified
if [ "$project_name" = "b1ig" ]
then
  if [ "$wx_libs_dir" != "" ]
  then
    mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/lib
    mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name
  fi

#create directory for menu shortcut
  mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/share
  mkdir -p -m 775 $tmp_dir$proj_dir_name/usr/share/applications
fi

#copy executable modules, samples, docs and README
cp ../../bin/lnx/$platform/$compiler/rel/b1i $tmp_dir$proj_dir_name/usr/bin/
cp ../../bin/lnx/$platform/$compiler/rel/b1iu $tmp_dir$proj_dir_name/usr/bin/
cp -a ../../docs $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/
cp -a ../../samples $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/
cp ../../README.md $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/

if [ "$project_name" = "b1ig" ]
then
  cp ../../bin/lnx/$platform/$compiler/rel/b1ig $tmp_dir$proj_dir_name/usr/bin/

#copy wxWidgets libraries only if the path is specified
  if [ "$wx_libs_dir" != "" ]
  then
    cp $wx_libs_dir/libwx_baseu*.so.4.0.0 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*core*.so.4.0.0 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*aui*.so.4.0.0 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp $wx_libs_dir/libwx_gtk3u*stc*.so.4.0.0 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_baseu*.so.4 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*core*.so.4 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*aui*.so.4 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/
    cp -d $wx_libs_dir/libwx_gtk3u*stc*.so.4 $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/

    unlink $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_baseu*net*.so.4
    unlink $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_baseu*xml*.so.4
    unlink $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_baseu*net*.so.4.0.0
    unlink $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_baseu*xml*.so.4.0.0
  fi
fi

if [ "$project_name" = "b1ig" ]
then
  cp b1ig.desktop $tmp_dir$proj_dir_name/usr/share/applications
fi

#create deb package control file
echo "Package: $proj_dir_name" >$tmp_dir$proj_dir_name/DEBIAN/control
if [ "$build_num" = "" ]
then
  echo "Version: $version" >>$tmp_dir$proj_dir_name/DEBIAN/control
else
  echo "Version: $version-$build_num" >>$tmp_dir$proj_dir_name/DEBIAN/control
fi
echo "Maintainer: Nikolay Pletnev <b1justomore@gmail.com>" >>$tmp_dir$proj_dir_name/DEBIAN/control
echo "Architecture: $lnx_arch" >>$tmp_dir$proj_dir_name/DEBIAN/control
echo "Section: misc" >>$tmp_dir$proj_dir_name/DEBIAN/control
echo "Depends: $depends" >>$tmp_dir$proj_dir_name/DEBIAN/control
echo "Priority: optional" >>$tmp_dir$proj_dir_name/DEBIAN/control
if [ "$project_name" = "b1ig" ]
then
  echo "Description: BASIC1 interpreter, IDE and debugger" >>$tmp_dir$proj_dir_name/DEBIAN/control
  echo " Just one more BASIC interpreter." >>$tmp_dir$proj_dir_name/DEBIAN/control
else
  echo "Description: BASIC1 interpreter" >>$tmp_dir$proj_dir_name/DEBIAN/control
  echo " Just one more BASIC interpreter." >>$tmp_dir$proj_dir_name/DEBIAN/control
  echo " Type b1i or b1iu to get usage help." >>$tmp_dir$proj_dir_name/DEBIAN/control
fi

#copy license and changelog, compress changelog with gzip
cp ../../LICENSE $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/copyright
cp ../../docs/changelog $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/
gzip -9 -n -S.Debian.gz $tmp_dir$proj_dir_name/usr/share/doc/$proj_dir_name/changelog

#strip copied executables
${7}strip --strip-all $tmp_dir$proj_dir_name/usr/bin/b1i
${7}strip --strip-all $tmp_dir$proj_dir_name/usr/bin/b1iu
if [ "$project_name" = "b1ig" ]
then
  ${7}strip --strip-all $tmp_dir$proj_dir_name/usr/bin/b1ig
  if [ "$wx_libs_dir" != "" ]
  then
    ${7}strip --strip-all $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_baseu*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_gtk3u*core*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_gtk3u*aui*.so.4.0.0
    ${7}strip --strip-all $tmp_dir$proj_dir_name/usr/lib/$proj_dir_name/libwx_gtk3u*stc*.so.4.0.0
  fi
fi

#correct file attributes
chmod 755 `find $tmp_dir$proj_dir_name -type d`
chmod 755 $tmp_dir$proj_dir_name/usr/bin/*
chmod 644 `find $tmp_dir$proj_dir_name/usr/share -type f`
if [ "$project_name" = "b1ig" ]
then
  if [ "$wx_libs_dir" != "" ]
  then
    chmod 644 `find $tmp_dir$proj_dir_name/usr/lib -type f`
  fi
fi

#create deb package
fakeroot dpkg-deb --build $tmp_dir$proj_dir_name

#delete all the stuff we collected for the package
rm -r $tmp_dir$proj_dir_name

#delete package with the same name from distr directory
if [ "$build_num" = "" ]
then
  rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}.deb
else
  rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-*.deb
fi

#move the package to distr directory
if [ "$build_num" = "" ]
then
  mv $tmp_dir$proj_dir_name.deb ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}.deb
else
  mv $tmp_dir$proj_dir_name.deb ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-${build_num}.deb
fi
