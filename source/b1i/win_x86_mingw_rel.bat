set project_name=b1i

if exist win_x86_mingw_env.bat call win_x86_mingw_env.bat

mkdir ..\..\build
mkdir ..\..\build\win
mkdir ..\..\build\win\x86
mkdir ..\..\build\win\x86\mingw
mkdir ..\..\build\win\x86\mingw\rel
mkdir ..\..\build\win\x86\mingw\rel\%project_name%

mkdir ..\..\bin
mkdir ..\..\bin\win
mkdir ..\..\bin\win\x86
mkdir ..\..\bin\win\x86\mingw
mkdir ..\..\bin\win\x86\mingw\rel

call get_git_rev.bat

cd ..\..\build\win\x86\mingw\rel\%project_name%
cmake -DCMAKE_BUILD_TYPE=Release ..\..\..\..\..\..\source\%project_name% -G "MinGW Makefiles"
cmake --build . --target %project_name%
copy .\%project_name%.exe ..\..\..\..\..\..\bin\win\x86\mingw\rel
cd ..\..\..\..\..\..\source\%project_name%
