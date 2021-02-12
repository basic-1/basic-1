rem usage: win_build.bat <root_path> <target_name> <project_name> <platform> <compiler> <configuration> <cmake1_options> <cmake2_options> <build_folder> [<is_next_revision>]
rem sample: win_build.bat "..\.." b1i b1i x86 msvc19 rel "-G ""Visual Studio 16 2019"" "--config Release" ".\Release" "next"
rem sample: win_build.bat "..\.." b1i b1i x64 mingw dbg "-G ""MinGW Makefiles"" -DCMAKE_BUILD_TYPE=Debug" " " "." " "

if "%~1"=="" goto _invargs
if "%~2"=="" goto _invargs
if "%~3"=="" goto _invargs
if "%~4"=="" goto _invargs
if "%~5"=="" goto _invargs
if "%~6"=="" goto _invargs
if "%~9"=="" goto _invargs
goto _argsok

:_invargs
echo invalid arguments
goto :eof

:_argsok

rem remove embracing double-quotes and replace doubled double-quotes with single ones
set cm1_opts=%~7
if not "%cm1_opts%"=="" (set cm1_opts=%cm1_opts:""="%)
set cm2_opts=%~8
if not "%cm2_opts%"=="" (set cm2_opts=%cm2_opts:""="%)

mkdir %~1\build
mkdir %~1\build\win
mkdir %~1\build\win\%~4
mkdir %~1\build\win\%~4\%~5
mkdir %~1\build\win\%~4\%~5\%~6
mkdir %~1\build\win\%~4\%~5\%~6\%~2

mkdir %~1\bin
mkdir %~1\bin\win
mkdir %~1\bin\win\%~4
mkdir %~1\bin\win\%~4\%~5
mkdir %~1\bin\win\%~4\%~5\%~6

set b1icommon_dir=%~dp0

if exist "%b1icommon_dir%win_%~4_%~5_env.bat" call "%b1icommon_dir%win_%~4_%~5_env.bat"

set B1I_TARGET=%~2
set B1I_BUILD_PATH=%~1\build\win\%~4\%~5\%~6\%~2
set B1I_SOURCE_PATH=..\..\..\..\..\..\source\%~3
set B1I_BIN_PATH=..\..\..\..\..\..\bin\win\%~4\%~5\%~6
set B1I_EXE_MODULE=%~9\%~2.exe

shift
if exist "%b1icommon_dir%get_git_rev.bat" call "%b1icommon_dir%get_git_rev.bat" %9

cd %B1I_BUILD_PATH%
cmake %cm1_opts% %B1I_SOURCE_PATH%
cmake --build . --target %B1I_TARGET% %cm2_opts%
copy %B1I_EXE_MODULE% %B1I_BIN_PATH%
cd %B1I_SOURCE_PATH%
