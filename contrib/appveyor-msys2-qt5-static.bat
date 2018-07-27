@echo on
set BITS=""
set ARCH=""
if "%1%"=="" (
  if "%MBITS%" == "" (set BITS=32) else (set BITS=%MBITS%)
) else (
  set BITS=%1%
) 
if "%BITS%"=="32" (
  set ARCH=i686
) else (
  set ARCH=x86_64
)

echo Build %BITS% bits with mingw-w64-%ARCH%

set OPATH=%PATH%
SET MSYS2=C:\msys64

set MINGW=%MSYS2%\mingw%BITS%\bin;%MSYS2%\usr\bin
set PATH=%MINGW%;%OPATH%;%CD%\Neovim\bin

pacman -Sy --noconfirm
pacman -S --needed --noconfirm mingw-w64-%ARCH%-gcc
pacman -S --needed --noconfirm mingw-w64-%ARCH%-cmake
pacman -S --needed --noconfirm mingw-w64-%ARCH%-jasper
pacman -S --needed --noconfirm mingw-w64-%ARCH%-python2
pacman -S --needed --noconfirm mingw-w64-%ARCH%-qt5-static

set MINGW=%MSYS2%\mingw%BITS%\bin
set PATH=%MINGW%;%OPATH%

rem CMake/MinGW workaround - remove sh.exe from PATH
set PATH=%PATH:C:\Program Files\Git\usr\bin;=%
where sh

set QTDIR=%MSYS2%\mingw%BITS%\qt5-static
set CMAKE_PREFIX_PATH=%QTDIR%
SET NVIM_QT_RUNTIME_PATH=../src/gui/runtime bin/nvim-qt

set PATH=%PATH%;%QTDIR%\bin;%CD%\Neovim\bin;

mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DUSE_STATIC_QT=1 -DCMAKE_PREFIX_PATH=%QTDIR% -DCMAKE_INSTALL_PREFIX=../INSTALL .. || goto error
cmake --build . --target install || goto error
ctest -VV || goto error
strip ..\INSTALL\bin\nvim-qt.exe

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
