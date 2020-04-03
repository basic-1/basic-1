rem usage: win_7zip_zip_distr.bat <project_name> <platform> <compiler>
set platform=x86
set compiler=mingw
set project_name=b1i

rem configurable path to 7-Zip archiver
set path_to_7z="C:\Program Files\7-Zip\7z.exe"

rem get interpreter version
for /f %%L in ('..\..\bin\win\%2\%3\rel\%1.exe /sv') do set version=_%%L

rem archive file name and path
set out_file_path=..\..\distr\%1_win_%2_%3%version%.zip

rem delete temp. directories
rmdir /s /q samples
rmdir /s /q bin
rmdir /s /q docs

rem create samples temp. directory and copy sample files there
mkdir samples
copy ..\..\samples\*.* samples

rem create docs temp. directory and copy document files there
mkdir docs
copy ..\..\docs\*.* docs

rem create temp. bin directory and copy binaries
mkdir bin
copy ..\..\bin\win\%2\%3\rel\*.* bin

rem delete output file
del %out_file_path%

rem create distr directory
mkdir ..\..\distr

rem move temp. bin, docs and samples directories to the archive
%path_to_7z% a -sdel -tZIP -- %out_file_path% bin
%path_to_7z% a -sdel -tZIP -- %out_file_path% samples
%path_to_7z% a -sdel -tZIP -- %out_file_path% docs

rem add LICENSE and README.md files to the archive
%path_to_7z% a -tZIP -- %out_file_path% ..\..\LICENSE
%path_to_7z% a -tZIP -- %out_file_path% ..\..\README.md
