@echo off
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
cd ..
REM move Release\r-type_server.exe ..\
powershell -Command "Copy-Item -Path '.\Release\r-type_server.exe' -Destination '..'"
powershell -Command "Copy-Item -Path '.\Release\r-type_server.exe' -Destination '.'"

if not exist "..\config" (
    mkdir ..\config
)

powershell -Command "Copy-Item -Path '.\config\server-config.json' -Destination '..\config'"

if not exist "..\scripts" (
    powershell -Command "Copy-Item -Path '.\scripts' -Destination '..' -Recurse"
)

if not exist "..\zlib.dll" (
    powershell -Command "Copy-Item -Path '.\Release\zlib.dll' -Destination '..'"
)