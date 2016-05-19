set CMAKE_PREFIX_PATH=%QTDIR%
set PATH=%PATH%;%QTDIR%\bin
mkdir build
cd build
rem FYI CMAKE_BUILD_TYPE does not work with VS generators, use --config
cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%configuration% -DCMAKE_INSTALL_PREFIX=../INSTALL ..
cmake --build . --config %configuration% --target install
