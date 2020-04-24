#!/bin/bash

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] || [ "$4" = "" ] || [ "$5" = "" ]
then
  echo "invalid arguments"
  exit 1
fi

project_name=$1
platform=$2
lnx_arch=$3
compiler=$4
depends=$5

version=`../../bin/lnx/$platform/$compiler/rel/$project_name /sv`
if [ $? -ne 0 ]
then
  echo "get version error"
  exit 1
fi

build_num=`git rev-list --count --first-parent HEAD`
if [ $? -ne 0 ]
then
  echo "get build number error"
  exit 1
fi

((build_num++))

#create directories for deb package
mkdir -p ./$project_name
mkdir -p ./$project_name/DEBIAN
mkdir -p ./$project_name/usr
mkdir -p ./$project_name/usr/bin
mkdir -p ./$project_name/usr/share
mkdir -p ./$project_name/usr/share/doc
mkdir -p ./$project_name/usr/share/doc/$project_name

#copy executable modules, samples, docs and README
cp -a ../../bin/lnx/$platform/$compiler/rel/. ./$project_name/usr/bin/
cp -a ../../docs ./$project_name/usr/share/doc/$project_name/
cp -a ../../samples ./$project_name/usr/share/doc/$project_name/
cp ../../README.md ./$project_name/usr/share/doc/$project_name/

#create deb package control file
echo "Package: $project_name" >./$project_name/DEBIAN/control
echo "Version: $version-$build_num" >>./$project_name/DEBIAN/control
echo "Maintainer: Nikolay Pletnev <b1justomore@gmail.com>" >>./$project_name/DEBIAN/control
echo "Architecture: $lnx_arch" >>./$project_name/DEBIAN/control
echo "Section: misc" >>./$project_name/DEBIAN/control
echo "Depends: $depends" >>./$project_name/DEBIAN/control
echo "Priority: optional" >>./$project_name/DEBIAN/control
echo "Description: BASIC1 interpreter" >>./$project_name/DEBIAN/control
echo " Just one more BASIC interpreter." >>./$project_name/DEBIAN/control
echo " Command-line BASIC interpreter." >>./$project_name/DEBIAN/control
echo " Type b1i to get usage help." >>./$project_name/DEBIAN/control

#copy license and changelog, compress changelog with gzip
cp ../../LICENSE ./$project_name/usr/share/doc/$project_name/copyright
cp ../../docs/changelog ./$project_name/usr/share/doc/$project_name/changelog
gzip -9 -n -S.Debian.gz ./$project_name/usr/share/doc/$project_name/changelog

#strip copied executable
strip --strip-all ./$project_name/usr/bin/$project_name

#correct file attributes
chmod 755 `find ./$project_name -type d`
chmod 755 ./$project_name/usr/bin/$project_name
chmod 644 `find ./$project_name/usr/share -type f`

#create deb package
fakeroot dpkg-deb --build $project_name

#delete all the stuff we collected for the package
rm -r ./$project_name

#delete package with the same name from distr directory
rm ../../distr/${project_name}_lnx_${lnx_arch}_${version}-*.deb

#move the package to distr directory
mv $project_name.deb ../../distr/${project_name}_lnx_${lnx_arch}_${version}-${build_num}.deb
