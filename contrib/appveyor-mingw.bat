rem CMake/MinGW workaround - remove sh.exe from PATH
where sh
set MINGW=C:\Qt\Tools\mingw530_32
set QTDIR=C:\Qt\5.11.1\mingw53_32
set PATH=%PATH:C:\Program Files\Git\usr\bin;=%
set CMAKE_PREFIX_PATH=%QTDIR%
set PATH=%MINGW%\bin;%PATH%;%QTDIR%\bin
:: Qt needs to find the windows platform plugin to run tests
set QT_QPA_PLATFORM_PLUGIN_PATH=%QTDIR%\plugins
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../INSTALL .. || goto :error
cmake --build . --target install || goto :error
rem TODO: reenable this
rem ctest -VV || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
