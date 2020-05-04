@echo off
pushd %~dp0
devtools\bin\vpc.exe /fr +game /mksln game_firefightreloaded.sln
popd
pause