@echo off

call prepare.bat

cd Client
call build-script.bat
cd ../Server
call build-script.bat
cd ..