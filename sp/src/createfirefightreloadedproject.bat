@echo off
pushd %~dp0
devtools\bin\vpc.exe /episodic +game /mksln game_firefightreloaded.sln
popd
pause