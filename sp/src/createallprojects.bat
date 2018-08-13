@echo off
pushd %~dp0
devtools\bin\vpc.exe /episodic +everything /mksln everything_firefightreloaded.sln
popd
