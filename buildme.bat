:: setup separate object root

set separate_object_root=TRUE
set _NTTREE=%CD%
set OBJECT_ROOT=%CD%\obj

set BINPLACE_EXCLUDE_FILE=%BASEDIR%\bin\symbad.txt
set BINPLACE_LOG=%_NTTREE%\build_logs\binplace.log
set BINPLACE_PDB_DLL=mspdb80.dll

set NO_BINPLACE=

set NTDBGFILES=1
set NTDBGFILES_PRIVATE=0

if Not Exist %_NTTREE% md %_NTTREE%
if Not Exist %_NTTREE%\build_logs md %_NTTREE%\build_logs
if Not Exist %OBJECT_ROOT% md %OBJECT_ROOT%
if Not Exist bin md bin

:: build files
build -wgc

:: extra copy coinstaller from redist
copy %BASEDIR%\redist\wdf\x86\WdfCoInstaller01009.dll bin\

echo off
:: create bat file for easier installation of driver
echo %BASEDIR%\tools\devcon\i386\devcon.exe install vmulti.inf djpnewton\vmulti > bin\install_driver.bat

:: create bat file for easier testing
echo %BASEDIR%\tools\devcon\i386\devcon.exe remove djpnewton\vmulti > bin\test.bat
echo cd .. >> bin\test.bat
echo cmd /c buildme.bat >> bin\test.bat
echo cd bin >> bin\test.bat
echo cmd /c install_driver.bat >> bin\test.bat
echo echo After keypress, launching testvmulti >> bin\test.bat
echo pause >> bin\test.bat
echo testvmulti /joystick >> bin\test.bat

echo on
