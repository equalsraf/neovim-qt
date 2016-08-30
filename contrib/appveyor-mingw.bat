rem CMake/MinGW workaround - remove sh.exe from PATH
where sh
set PATH=%PATH:C:\Program Files\Git\usr\bin;=%
set CMAKE_PREFIX_PATH=%QTDIR%
set PATH=%MINGW%\bin;%PATH%;%QTDIR%\bin
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%configuration% -DCMAKE_INSTALL_PREFIX=../INSTALL ..
cmake --build . --target install
