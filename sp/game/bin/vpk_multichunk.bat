@ECHO OFF
ECHO Compressing "%~1" into a VPK.
"%~dp0/vpk.exe" -M "%~1"
pause