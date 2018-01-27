set QTDIR=C:\Qt\5.7\msvc2015
set CMAKE_PREFIX_PATH=%QTDIR%
set PATH=%PATH%;%QTDIR%\bin
mkdir build
cd build
:: FYI CMAKE_BUILD_TYPE does not work with VS generators, use --config
cmake -G "Visual Studio 14" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../INSTALL ..
cmake --build . --config Release --target install
:: TODO enable tests for msvc builds
:: set QT_QPA_PLATFORM_PLUGIN_PATH=%QTDIR%\plugins
:: ctest -C Release -VV
