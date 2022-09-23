@ECHO OFF

SET dest=G:\Projects\GitHub\FIREFIGHT-RELOADED-src-sdk-2013
echo Destination set to %dest%

SET debug=1
SET basedir=%CD%\github
SET gamescriptdir=%basedir%\scripts
SET gamecfgdir=%basedir%\cfg
SET gamesresourcedir=%basedir%\resource
SET gamesresourcedir=%basedir%\mapadd
if not exist "%gamescriptdir%" mkdir "%gamescriptdir%"
if not exist "%gamecfgdir%" mkdir "%gamecfgdir%"
if not exist "%gamesresourcedir%" mkdir "%gamesresourcedir%"
if not exist "%gamemapadddir%" mkdir "%gamemapadddir%"

echo.
echo Copying game data...
XCOPY /E "%CD%\resource" "%gamescriptdir%" /sy
XCOPY /E "%CD%\scripts" "%gamescriptdir%" /sy
XCOPY /E "%CD%\cfg" "%gamecfgdir%" /sy
XCOPY /E "%CD%\mapadd" "%gamemapadddir%" /sy

echo.
echo Moving game data to GitHub folder...
SET scriptsdir="%dest%\gamedata"
if not exist "%scriptsdir%" mkdir "%scriptsdir%"
XCOPY /E "%basedir%" "%scriptsdir%" /sy
rmdir "%basedir%" /s /q

echo.
echo Coying additional files to GitHub folder...
if not exist "%scriptsdir%\batch" mkdir "%scriptsdir%\batch"
XCOPY "%cd%\github_sync.bat" "%scriptsdir%\batch" /y
if %debug%==1 pause