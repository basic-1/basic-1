set project_name=b1i

if exist win_x86_msvc10_env.bat call win_x86_msvc10_env.bat

mkdir ..\..\build
mkdir ..\..\build\win
mkdir ..\..\build\win\x86
mkdir ..\..\build\win\x86\msvc10
mkdir ..\..\build\win\x86\msvc10\rel
mkdir ..\..\build\win\x86\msvc10\rel\%project_name%

mkdir ..\..\bin
mkdir ..\..\bin\win
mkdir ..\..\bin\win\x86
mkdir ..\..\bin\win\x86\msvc10
mkdir ..\..\bin\win\x86\msvc10\rel

cd ..\..\build\win\x86\msvc10\rel\%project_name%
cmake -A Win32 ..\..\..\..\..\..\source\%project_name%
cmake --build . --target %project_name% --config Release
copy .\Release\%project_name%.exe ..\..\..\..\..\..\bin\win\x86\msvc10\rel
cd ..\..\..\..\..\..\source\%project_name%
