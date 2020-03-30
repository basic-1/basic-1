set platform=x86
set compiler=mingw
set project_name=b1i

set path_to_7z="C:\Program Files\7-Zip\7z.exe"

for /f %%L in ('..\..\bin\win\%platform%\%compiler%\rel\%project_name%.exe /sv') do set version=_%%L

set out_file_path=..\..\distr\%project_name%_win_%platform%_%compiler%%version%.zip

rmdir /s /q samples
rmdir /s /q bin

mkdir samples
copy ..\..\samples\*.* samples
mkdir bin
copy ..\..\bin\win\%platform%\%compiler%\rel\*.* bin

del %out_file_path%

mkdir ..\..\distr

%path_to_7z% a -sdel -tZIP -- %out_file_path% bin
%path_to_7z% a -sdel -tZIP -- %out_file_path% samples
%path_to_7z% a -tZIP -- %out_file_path% ..\..\LICENSE
%path_to_7z% a -tZIP -- %out_file_path% ..\..\README.md
