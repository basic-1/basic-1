setlocal enabledelayedexpansion

del "%~dp0gitrev.tmp"
del "%~dp0gitrev.h"

git rev-list --count --first-parent HEAD >>"%~dp0gitrev.tmp"
if errorlevel 1 goto _fail

set /p rev=<"%~dp0gitrev.tmp"
if "%~1"=="next" (set /a rev+=1)

echo #define B1_GIT_REVISION ^"%rev%^">>"%~dp0gitrev.h"

goto _success

:_fail
del "%~dp0gitrev.h"
echo: >>"%~dp0gitrev.h"

:_success
del "%~dp0gitrev.tmp"
