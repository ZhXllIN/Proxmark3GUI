@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

set "GUI_DIR=%~dp0..\Proxmark3GUI-v0.2.16-rrg-v4.21611-matched"
set "CLIENT=%GUI_DIR%\client\proxmark3.exe"
set "CLIENT_LIBS=%GUI_DIR%\client\libs"
set "BOOTROM=%~dp0bootrom.elf"
set "FULLIMAGE=%~dp0fullimage.elf"

echo Proxmark3 RDV4 firmware flasher
echo Firmware: v4.21611-145-g2bf48a02c
echo.

if not exist "%CLIENT%" (
    echo ERROR: Matching Windows client was not found.
    echo Extract both release ZIP files into the same parent folder.
    echo Expected client: "%CLIENT%"
    pause
    exit /b 1
)

if not exist "%BOOTROM%" (
    echo ERROR: bootrom.elf was not found in this folder.
    pause
    exit /b 1
)

if not exist "%FULLIMAGE%" (
    echo ERROR: fullimage.elf was not found in this folder.
    pause
    exit /b 1
)

if not "%~1"=="" (
    set "PM3_PORT=%~1"
) else (
    set /p "PM3_PORT=Enter the Proxmark3 COM port, for example COM5: "
)

echo(!PM3_PORT!| findstr /R /I "^COM[0-9][0-9]*$" >nul
if errorlevel 1 (
    echo ERROR: Invalid COM port: !PM3_PORT!
    pause
    exit /b 1
)

echo.
echo WARNING: This firmware is only for Proxmark3 RDV4 with AT91SAM7S512.
echo Flashing the wrong hardware can make the device unbootable.
choice /C YN /N /M "Continue with !PM3_PORT!? [Y/N] "
if errorlevel 2 exit /b 0

set "PATH=%CLIENT_LIBS%;%PATH%"

echo.
echo Flashing bootrom and fullimage. Do not disconnect USB...
"%CLIENT%" "!PM3_PORT!" --flash --unlock-bootloader --image "%BOOTROM%" --image "%FULLIMAGE%"
if errorlevel 1 (
    echo.
    echo FLASH FAILED.
    echo Disconnect USB, hold the Proxmark3 button while reconnecting, then run this script again.
    pause
    exit /b 1
)

echo.
echo Waiting for the device to reconnect...
timeout /t 3 /nobreak >nul

echo.
echo Verifying installed versions...
"%CLIENT%" "!PM3_PORT!" -w --incognito -c "hw version"
if errorlevel 1 (
    echo.
    echo Firmware was written, but automatic verification failed.
    echo Reconnect the device and check hw version from the GUI.
    pause
    exit /b 1
)

echo.
echo DONE. Client, bootrom, and OS should all show:
echo v4.21611-145-g2bf48a02c
pause
exit /b 0
