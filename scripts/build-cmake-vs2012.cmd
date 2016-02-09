set UPLINK_DIR="%~dp0\.."
set BUILD_DIR="%~dp0\..\builds\cmake-win64-vs2012"
set CMAKE_GENERATOR="Visual Studio 11 Win64"
set CMAKE_BUILD_TYPES="Release;Debug"
mkdir %BUILD_DIR%
pushd %BUILD_DIR%
if %ERRORLEVEL% equ 0 call cmake -G%CMAKE_GENERATOR% -DCMAKE_CONFIGURATION_TYPES=%CMAKE_BUILD_TYPES% %UPLINK_DIR%
if %ERRORLEVEL% equ 0 call "%ProgramFiles(x86)%\Microsoft Visual Studio 11.0\Common7\Ide\devenv.exe" uplink.sln
popd
