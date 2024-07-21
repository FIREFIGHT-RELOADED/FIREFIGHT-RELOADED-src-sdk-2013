@ECHO OFF
call reset.bat
cls
SET dest=G:\Projects\FIREFIGHT\steambuild\FIREFIGHTRELOADED\release\firefightreloaded
SET dlldest=G:\Projects\FIREFIGHT\steambuild\FIREFIGHTRELOADED\windows_client\firefightreloaded\bin
SET linuxdlldest=G:\Projects\FIREFIGHT\steambuild\FIREFIGHTRELOADED\linux_client\firefightreloaded\bin
echo Destination set to %dest%

SET debug=0
SET basedir=%CD%\move
SET gamescriptdir=%basedir%\scripts
SET gamecfgdir=%basedir%\cfg
SET gamesresourcedir=%basedir%\resource
SET gamemapdir=%basedir%\maps
SET gamemapadddir=%basedir%\mapadd
SET gamepaduidir=%basedir%\gamepadui
SET gamebindir=%basedir%\bin
SET gamebinlinuxdir=%basedir%\bin_linux
SET gamescriptdir=%basedir%\scripts
SET gamesounddir=%basedir%\sound
if not exist "%gamesounddir%" mkdir "%gamesounddir%"
if not exist "%gamescriptdir%" mkdir "%gamescriptdir%"
if not exist "%gamecfgdir%" mkdir "%gamecfgdir%"
if not exist "%gamesresourcedir%" mkdir "%gamesresourcedir%"
if not exist "%gamemapdir%" mkdir "%gamemapdir%"
if not exist "%gamemapadddir%" mkdir "%gamemapadddir%"
if not exist "%gamepaduidir%" mkdir "%gamepaduidir%"
if not exist "%gamebindir%" mkdir "%gamebindir%"
if not exist "%gamebinlinuxdir%" mkdir "%gamebinlinuxdir%"

echo.
echo Copying game data...
XCOPY /E "%CD%\resource" "%gamesresourcedir%" /sy
XCOPY /E "%CD%\scripts" "%gamescriptdir%" /sy
XCOPY /E "%CD%\sound" "%gamesounddir%" /sy
XCOPY /E "%CD%\cfg" "%gamecfgdir%" /sy
XCOPY /E "%CD%\maps" "%gamemapdir%" /sy
XCOPY /E "%CD%\mapadd" "%gamemapadddir%" /sy
XCOPY /E "%CD%\gamepadui" "%gamepaduidir%" /sy
XCOPY /E "%CD%\bin" "%gamebindir%" /sy
XCOPY /E "%CD%\bin_linux" "%gamebinlinuxdir%" /sy
XCOPY "%CD%\gameinfo.txt" "%basedir%" /y
XCOPY "%CD%\steam.inf" "%basedir%" /y
XCOPY "%CD%\version.txt" "%basedir%" /y
XCOPY "%CD%\firefightreloaded_pak_000.vpk" "%basedir%" /y
XCOPY "%CD%\firefightreloaded_pak_001.vpk" "%basedir%" /y
XCOPY "%CD%\firefightreloaded_pak_002.vpk" "%basedir%" /y
XCOPY "%CD%\firefightreloaded_pak_003.vpk" "%basedir%" /y
XCOPY "%CD%\firefightreloaded_pak_dir.vpk" "%basedir%" /y

echo.
echo Moving game data to release folder...
XCOPY /E "%gamebindir%" "%dlldest%" /sy
rmdir "%gamebindir%" /s /q
XCOPY /E "%gamebinlinuxdir%" "%linuxdlldest%" /sy
rmdir "%gamebinlinuxdir%" /s /q
XCOPY /E "%basedir%" "%dest%" /sy
rmdir "%basedir%" /s /q

echo.
echo Coying additional files to release folder...
XCOPY "%cd%\reset.bat" "%dest%" /y
if %debug%==1 pause

call github_sync.bat