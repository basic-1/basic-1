rem usage: win_7zip_zip_distr.bat <project_name> <platform> <compiler> [<is_next_revision>]

if "%~1"=="" goto _invargs
if "%~2"=="" goto _invargs
if "%~3"=="" goto _invargs
goto _argsok

:_invargs
echo invalid arguments
goto :eof

:_argsok
set project_name=%~1
set platform=%~2
set compiler=%~3

rem configurable path to 7-Zip archiver
set path_to_7z="C:\Program Files\7-Zip\7z.exe"

if not exist %path_to_7z% (
  echo 7z archiver not found
  goto :eof
)

git rev-list --count --first-parent HEAD >>gitrev.tmp
if errorlevel 1 (
  del gitrev.tmp
  echo get build number error
  goto :eof
)

set /p build_num=<gitrev.tmp
del gitrev.tmp
if "%~4"=="next" (set /a build_num+=1)

rem get interpreter version
set /p version=<..\%project_name%\version.h
set version=%version:#define=%
set version=%version:B1_INT_VERSION=%
set version=%version:"=%
set version=%version: =%

rem archive file name and path
set out_file_path=..\..\distr\%project_name%_win_%platform%_%compiler%_%version%

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
copy ..\..\bin\win\%platform%\%compiler%\rel\*.* bin

rem delete output file
del %out_file_path%*.zip

rem create distr directory
mkdir ..\..\distr

rem move temp. bin, docs and samples directories to the archive
%path_to_7z% a -sdel -tZIP -- %out_file_path%-%build_num%.zip bin
%path_to_7z% a -sdel -tZIP -- %out_file_path%-%build_num%.zip samples
%path_to_7z% a -sdel -tZIP -- %out_file_path%-%build_num%.zip docs

rem add LICENSE and README.md files to the archive
%path_to_7z% a -tZIP -- %out_file_path%-%build_num%.zip ..\..\LICENSE
%path_to_7z% a -tZIP -- %out_file_path%-%build_num%.zip ..\..\README.md
