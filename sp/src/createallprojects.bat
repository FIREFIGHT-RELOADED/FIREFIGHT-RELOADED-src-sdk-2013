@echo off
pushd %~dp0
devtools\bin\vpc.exe /fr +everything /mksln everything_firefightreloaded.sln
popd
