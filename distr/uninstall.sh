#!/bin/bash

project_name=b1i

#delete b1ig menu shortcut
rm /usr/share/applications/b1ig.desktop

#delete binaries
rm /usr/local/bin/b1i
rm /usr/local/bin/b1iu
rm /usr/local/bin/b1ig

#delete libraries
rm -r /usr/local/lib/$project_name

#delete docs
rm -r /usr/local/share/$project_name
