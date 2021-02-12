rem usage: win_7zip_zip_distr.bat <project_name> <platform> <compiler> [<is_next_revision>]

setlocal EnableDelayedExpansion

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

if exist "..\b1icommon\win_%platform%_%compiler%_env.bat" call "..\b1icommon\win_%platform%_%compiler%_env.bat"

rem configurable path to 7-Zip archiver
set path_to_7z="C:\Program Files\7-Zip\7z.exe"

if not exist %path_to_7z% (
  echo 7z archiver not found
  goto :eof
)

if not exist ..\b1icommon\gitrev.h (
  echo gitrev.h not found
) else (
  set /p build_num=<..\b1icommon\gitrev.h
  set build_num=!build_num:#define=!
  set build_num=!build_num:B1_GIT_REVISION=!
  set build_num=!build_num:"=!
  set build_num=!build_num: =!

rem do not increase git revision number read from gitrev.h
rem  if "%~4"=="next" (set /a build_num+=1)
)

rem get interpreter version
if not exist ..\b1icommon\version.h (
  echo version.h not found
  goto :eof
)
set /p version=<..\b1icommon\version.h
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
copy ..\..\bin\win\%platform%\%compiler%\rel\b1i.exe bin
copy ..\..\bin\win\%platform%\%compiler%\rel\b1iu.exe bin
rem copy C/C++ run-time libraries (temp. solution: now they should be present in the output directory)
copy ..\..\bin\win\%platform%\%compiler%\rel\lib*.dll bin
if "%project_name%"=="b1ig" (
  copy ..\..\bin\win\%platform%\%compiler%\rel\b1ig.exe bin
)

rem copy wxWidgets libraries if necessary
if "%project_name%"=="b1ig" (
  "%B1_WX_CONFIG%" --libs >>wxlibs.tmp
  if errorlevel 1 (
   del wxlibs.tmp
    echo no wxWidgets libraries
    goto path_read
  )

  set /p wx_libs_dir=<wxlibs.tmp
  del wxlibs.tmp
)
:path_read

if "%wx_libs_dir%"=="" goto wx_libs_copied

for %%s in (%wx_libs_dir%) do (
  set subs=%%s
  if "!subs:~0,2!"=="-L" (
    set wx_libs_dir=!subs:~2!
    goto path_found
  )
)
:path_found

if "%wx_libs_dir%"=="" goto wx_libs_copied

rem don't know how to locate wxWidgets dlls properly at the moment...
copy "%wx_libs_dir%\wxbase*.dll" bin
del bin\wxbase*_net_*.dll
del bin\wxbase*_xml_*.dll
copy "%wx_libs_dir%\*_core_*.dll" bin
copy "%wx_libs_dir%\*_stc_*.dll" bin
copy "%wx_libs_dir%\*_aui_*.dll" bin
:wx_libs_copied

rem delete output file
if "%build_num%"=="" (
  del %out_file_path%.zip
  set out_file_path=%out_file_path%.zip
) else (
  del %out_file_path%*.zip
  set out_file_path=%out_file_path%-%build_num%.zip
)

rem create distr directory
mkdir ..\..\distr

rem move temp. bin, docs and samples directories to the archive
%path_to_7z% a -sdel -tZIP -- %out_file_path% bin
%path_to_7z% a -sdel -tZIP -- %out_file_path% samples
%path_to_7z% a -sdel -tZIP -- %out_file_path% docs

rem add LICENSE and README.md files to the archive
%path_to_7z% a -tZIP -- %out_file_path% ..\..\LICENSE
%path_to_7z% a -tZIP -- %out_file_path% ..\..\README.md
