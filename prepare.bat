@echo off
setlocal

set "boostFolderPath=C:\local\boost_1_86_0"

if not exist "%boostFolderPath%" (
    set BOOST_VERSION=1.86.0
    set BOOST_VERSION_UNDERSCORES=1_86_0
    set BOOST_ROOT=%cd%\boost_%BOOST_VERSION_UNDERSCORES%
    set BOOST_URL=https://boostorg.jfrog.io/artifactory/main/release/%BOOST_VERSION%/binaries/boost_%BOOST_VERSION_UNDERSCORES%-msvc-14.3-64.exe

    if not exist boost_%BOOST_VERSION_UNDERSCORES%.zip (
        echo Downloading Boost %BOOST_VERSION%...
        powershell -Command "Invoke-WebRequest -Uri %BOOST_URL% -OutFile boost_%BOOST_VERSION_UNDERSCORES%.exe"
    ) else (
        echo Boost archive is already present.
    )
    .\boost_%BOOST_VERSION_UNDERSCORES%.exe
    echo Boost %BOOST_VERSION% has been installed.
    pause
)

REM TODO get current user name
set "pythonFolderPath=C:\Users\%USERNAME%\AppData\Local\Programs\Python\Python311"

if not exist "%pythonFolderPath%" (
    set PYTHON_VERSION=3.11.0
    set PYTHON_URL=https://www.python.org/ftp/python/%PYTHON_VERSION%/python-%PYTHON_VERSION%-amd64.exe

    echo Downloading Python %PYTHON_VERSION%
    powershell -Command "Invoke-WebRequest -Uri %PYTHON_URL% -OutFile python.exe"
    .\python.exe
    echo Python %PYTHON_VERSION% has been installed.
    pause
)
