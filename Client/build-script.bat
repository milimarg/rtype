@echo off
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
cd ..
REM move Release\r-type_client.exe ..\
powershell -Command "Copy-Item -Path '.\Release\r-type_client.exe' -Destination '..'"
powershell -Command "Copy-Item -Path '.\Release\r-type_client.exe' -Destination '.'"

if not exist "..\config" (
    mkdir ..\config
)

powershell -Command "Copy-Item -Path '.\config\client-config.json' -Destination '..\config'"

if not exist "..\assets" (
    powershell -Command "Copy-Item -Path '.\assets' -Destination '..' -Recurse"
)

if not exist "..\mods" (
    powershell -Command "Copy-Item -Path '.\mods' -Destination '..' -Recurse"
)