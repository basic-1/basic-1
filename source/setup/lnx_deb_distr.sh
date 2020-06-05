#!/bin/bash

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ]
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
depends=$5

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

if [ "$6" = "next" ]
then
  ((build_num++))
fi

#create directories for deb package
mkdir -p -m 775 $tmp_dir$project_name
mkdir -p -m 775 $tmp_dir$project_name/DEBIAN
mkdir -p -m 775 $tmp_dir$project_name/usr
mkdir -p -m 775 $tmp_dir$project_name/usr/bin
mkdir -p -m 775 $tmp_dir$project_name/usr/share
mkdir -p -m 775 $tmp_dir$project_name/usr/share/doc
mkdir -p -m 775 $tmp_dir$project_name/usr/share/doc/$project_name

#copy executable modules, samples, docs and README
cp -a ../../bin/lnx/$platform/$compiler/rel/. $tmp_dir$project_name/usr/bin/
cp -a ../../docs $tmp_dir$project_name/usr/share/doc/$project_name/
cp -a ../../samples $tmp_dir$project_name/usr/share/doc/$project_name/
cp ../../README.md $tmp_dir$project_name/usr/share/doc/$project_name/

#create deb package control file
echo "Package: $project_name" >$tmp_dir$project_name/DEBIAN/control
echo "Version: $version-$build_num" >>$tmp_dir$project_name/DEBIAN/control
echo "Maintainer: Nikolay Pletnev <b1justomore@gmail.com>" >>$tmp_dir$project_name/DEBIAN/control
echo "Architecture: $lnx_arch" >>$tmp_dir$project_name/DEBIAN/control
echo "Section: misc" >>$tmp_dir$project_name/DEBIAN/control
echo "Depends: $depends" >>$tmp_dir$project_name/DEBIAN/control
echo "Priority: optional" >>$tmp_dir$project_name/DEBIAN/control
echo "Description: BASIC1 interpreter" >>$tmp_dir$project_name/DEBIAN/control
echo " Just one more BASIC interpreter." >>$tmp_dir$project_name/DEBIAN/control
echo " Type b1i or b1iu to get usage help." >>$tmp_dir$project_name/DEBIAN/control

#copy license and changelog, compress changelog with gzip
cp ../../LICENSE $tmp_dir$project_name/usr/share/doc/$project_name/copyright
cp ../../docs/changelog $tmp_dir$project_name/usr/share/doc/$project_name/changelog
gzip -9 -n -S.Debian.gz $tmp_dir$project_name/usr/share/doc/$project_name/changelog

#strip copied executables
${7}strip --strip-all $tmp_dir$project_name/usr/bin/b1i
${7}strip --strip-all $tmp_dir$project_name/usr/bin/b1iu

#correct file attributes
chmod 755 `find $tmp_dir$project_name -type d`
chmod 755 $tmp_dir$project_name/usr/bin/*
chmod 644 `find $tmp_dir$project_name/usr/share -type f`

#create deb package
fakeroot dpkg-deb --build $tmp_dir$project_name

#delete all the stuff we collected for the package
rm -r $tmp_dir$project_name

#delete package with the same name from distr directory
rm ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-*.deb

#move the package to distr directory
mv $tmp_dir$project_name.deb ../../distr/${project_name}_lnx_${lnx_arch}_${compiler}_${version}-${build_num}.deb
