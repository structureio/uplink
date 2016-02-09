set UPLINK_DIR="%~dp0\.."
set BUILD_DIR="%~dp0\..\builds\cmake-nmake-r"
set CMAKE_GENERATOR="NMake Makefiles"
set CMAKE_BUILD_TYPE=Release
mkdir %BUILD_DIR%
pushd %BUILD_DIR%
if %ERRORLEVEL% equ 0 call cmake -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -G%CMAKE_GENERATOR% %UPLINK_DIR%
if %ERRORLEVEL% equ 0 call nmake
popd
