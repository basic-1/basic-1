# distr directory  
  
## Brief  
  
The directory contains distributives or archives with binary executable files, samples and documents.  
  
## Distributives  
  
`b1i_lnx_amd64_gcc_1.0.0-XX.deb` - deb package for Linux, amd64 (Intel x64) architecture.  
`b1i_lnx_armhf_gcc_1.0.0-XX.deb` - deb package for Linux, armhf (armv7l) architecture (suitable for Raspbian, Armbian and maybe other Linux-based operating systems for ARM computers).  
`b1i_lnx_i386_gcc_1.0.0-XX.deb` - deb package for Linux, i386 (Intel x86) architecture.  
`b1i_lnx_amd64_gcc_1.0.0-XX.tar.gz` - tar.gz archive for Linux, amd64 (Intel x64) architecture.  
`b1i_lnx_armhf_gcc_1.0.0-XX.tar.gz` - tar.gz archive for Linux, armhf (armv7l) architecture (suitable for Raspbian, Armbian and maybe other Linux-based operating systems for ARM computers).  
`b1i_lnx_i386_gcc_1.0.0-XX.tar.gz` - tar.gz archive for Linux, i386 (Intel x86) architecture.  
`b1i_win_x64_mingw_1.0.0-XX.zip` - zip archive with executables, samples and documentation for Windows x64.  
`b1i_win_x86_mingw_1.0.0-XX.zip` - zip archive with executables, samples and documentation for Windows x86.  
`b1ig_lnx_amd64_gcc_1.0.0-XX.deb` - deb package for Linux, amd64 (Intel x64) architecture (includes **b1ig** interpreter with graphical IDE and debugger).  
`b1ig_lnx_i386_gcc_1.0.0-XX.deb` - deb package for Linux, i386 (Intel x86) architecture (includes **b1ig** interpreter with graphical IDE and debugger).  
`b1ig_lnx_amd64_gcc_1.0.0-XX.tar.gz` - tar.gz archive for Linux, amd64 (Intel x64) architecture (includes **b1ig** interpreter with graphical IDE and debugger).  
`b1ig_lnx_i386_gcc_1.0.0-XX.tar.gz` - tar.gz archive for Linux, i386 (Intel x86) architecture (includes **b1ig** interpreter with graphical IDE and debugger).  
`b1ig_win_x64_mingw_1.0.0-XX.zip` - zip archive with executables, samples and documentation for Windows x64 (includes **b1ig.exe** interpreter with graphical IDE and debugger).  
`b1ig_win_x86_mingw_1.0.0-XX.zip` - zip archive with executables, samples and documentation for Windows x86 (includes **b1ig.exe** interpreter with graphical IDE and debugger).  
  
`install.sh` - Linux script to install BASIC1 interpreter from tar.gz archive.  
`uninstall.sh` - Linux shell script to uninstall BASIC1 interpreter installed from tar.gz archive.  
  
The distributives does not contain source code so download it from https://github.com/basic-1/basic-1 if needed. `XX` in distributive file name stands for build number (git revision), e.g.: `b1i_lnx_amd64_gcc_1.0.0-42.deb`.  
  
Distributives with name starting from `b1ig` contain not only command-line interpreter but also **b1ig** simple IDE with graphical user interface and debugger.  
  
**Important note:** Do not install `b1i` and `b1ig` packages at the same time because they can use the same directories, settings, etc. The previous package should be uninstalled first.  
  
## deb packages  
  
Command sample to install BASIC1 interpreter from deb package with dpkg (on Debian and Ubuntu):  
`sudo dpkg -i b1i_lnx_amd64_gcc_1.0.0-42.deb`  
  
To uninstall previously installed interpreter:  
`sudo dpkg -r b1i`  
  
## tar.gz packages  
  
To install the interpreter from tar.gz archive just copy its content to `/usr/local` directory:  
`tar -xzvf b1i_lnx_armhf_gcc_1.0.0-42.tar.gz -C /tmp`  
`sudo cp -r /tmp/b1i/* /usr/local`  
`rm -r /tmp/b1i`  

`install.sh` script uses the same commands so the simplest way to install the interpreter from tar.gz archive is running the script with archive name as argument:  
`sudo ./install.sh b1i_lnx_armhf_gcc_1.0.0-42.tar.gz`  
  
To uninstall the interpreter installed from tar.gz archive use `uninstall.sh` script:  
`sudo ./uninstall.sh`  