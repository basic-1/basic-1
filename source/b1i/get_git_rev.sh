#!/bin/bash

unlink gitrev.h
touch gitrev.h

if git_rev=`git rev-list --count --first-parent HEAD`
then
  if [ "$1" == "next" ]
  then
    ((git_rev++))
  fi
  echo '#define B1_GIT_REVISION "'$git_rev'"' >>gitrev.h
fi

