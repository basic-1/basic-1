set project_name=b1i

if exist win_x86_msvc10_env.bat call win_x86_msvc10_env.bat

mkdir ..\..\build
mkdir ..\..\build\win
mkdir ..\..\build\win\x86
mkdir ..\..\build\win\x86\msvc10
mkdir ..\..\build\win\x86\msvc10\dbg
mkdir ..\..\build\win\x86\msvc10\dbg\%project_name%

mkdir ..\..\bin
mkdir ..\..\bin\win
mkdir ..\..\bin\win\x86
mkdir ..\..\bin\win\x86\msvc10
mkdir ..\..\bin\win\x86\msvc10\dbg

call get_git_rev.bat %1

cd ..\..\build\win\x86\msvc10\dbg\%project_name%
cmake -A Win32 ..\..\..\..\..\..\source\%project_name%
cmake --build . --target %project_name% --config Debug
copy .\Debug\%project_name%.exe ..\..\..\..\..\..\bin\win\x86\msvc10\dbg
cd ..\..\..\..\..\..\source\%project_name%
