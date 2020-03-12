del gitrev.tmp
del gitrev.h

echo #define B1_GIT_REVISION ^"> gitrev.tmp

git rev-list --count --first-parent HEAD >>gitrev.tmp
if errorlevel 1 goto _fail

for /f "delims=" %%L in ('type gitrev.tmp') do (
  set /p "X=%%L" 0<nul >>gitrev.h
)

echo ^">>gitrev.h

goto _success

:_fail
del gitrev.h
echo. >>gitrev.h

:_success
del gitrev.tmp
