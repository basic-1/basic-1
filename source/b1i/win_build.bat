rem usage: win_build.bat <project_name> <platform> <compiler> <configuration> <cmake1_options> <cmake2_options> <build_folder> <is_next_revision>
rem sample: win_build.bat b1i x86 msvc19 rel "-G ""Visual Studio 16 2019"" "--config Release" ".\Release" "next"
rem sample: win_build.bat b1i x64 mingw dbg "-G ""MinGW Makefiles"" -DCMAKE_BUILD_TYPE=Debug" " " "." " "

rem remove embracing double-quotes and replace doubled double-quotes with single ones
set cm1_opts=%~5
if not "%cm1_opts%"=="" (set cm1_opts=%cm1_opts:""="%)
set cm2_opts=%~6
if not "%cm2_opts%"=="" (set cm2_opts=%cm2_opts:""="%)

mkdir ..\..\build
mkdir ..\..\build\win
mkdir ..\..\build\win\%~2
mkdir ..\..\build\win\%~2\%~3
mkdir ..\..\build\win\%~2\%~3\%~4
mkdir ..\..\build\win\%~2\%~3\%~4\%~1

mkdir ..\..\bin
mkdir ..\..\bin\win
mkdir ..\..\bin\win\%~2
mkdir ..\..\bin\win\%~2\%~3
mkdir ..\..\bin\win\%~2\%~3\%~4

call get_git_rev.bat %8

if exist win_%~2_%~3_env.bat call win_%~2_%~3_env.bat

cd ..\..\build\win\%~2\%~3\%~4\%~1
cmake %cm1_opts% ..\..\..\..\..\..\source\%~1
cmake --build . --target %~1 %cm2_opts%
copy "%~7\%~1.exe" ..\..\..\..\..\..\bin\win\%~2\%~3\%~4
cd ..\..\..\..\..\..\source\%~1
