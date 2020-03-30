set project_name=b1i

if exist win_x86_mingw_env.bat call win_x86_mingw_env.bat

mkdir ..\..\build
mkdir ..\..\build\win
mkdir ..\..\build\win\x86
mkdir ..\..\build\win\x86\mingw
mkdir ..\..\build\win\x86\mingw\dbg
mkdir ..\..\build\win\x86\mingw\dbg\%project_name%

mkdir ..\..\bin
mkdir ..\..\bin\win
mkdir ..\..\bin\win\x86
mkdir ..\..\bin\win\x86\mingw
mkdir ..\..\bin\win\x86\mingw\dbg

call get_git_rev.bat %1

cd ..\..\build\win\x86\mingw\dbg\%project_name%
cmake -DCMAKE_BUILD_TYPE=Debug ..\..\..\..\..\..\source\%project_name% -G "MinGW Makefiles"
cmake --build . --target %project_name%
copy .\%project_name%.exe ..\..\..\..\..\..\bin\win\x86\mingw\dbg
cd ..\..\..\..\..\..\source\%project_name%
