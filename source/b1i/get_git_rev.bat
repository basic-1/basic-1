setlocal enabledelayedexpansion

del gitrev.tmp
del gitrev.h

git rev-list --count --first-parent HEAD >>gitrev.tmp
if errorlevel 1 goto _fail

set /p rev=<gitrev.tmp
if "%~1"=="next" (set /a rev+=1)

echo #define B1_GIT_REVISION ^"%rev%^">>gitrev.h

goto _success

:_fail
del gitrev.h
echo. >>gitrev.h

:_success
del gitrev.tmp
