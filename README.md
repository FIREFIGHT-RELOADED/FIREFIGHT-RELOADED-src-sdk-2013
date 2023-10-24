# ![Logo](https://github.com/FIREFIGHT-RELOADED/FIREFIGHT-RELOADED-src-sdk-2013/raw/endgame/Graphics/FR_Logo.png)

[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)

FIREFIGHT RELOADED is an open-source Source SDK 2013 modification with the goal to combine gameplay elements from Half-Life 2, SMOD, old-school first-person shooters like Quake and Unreal Tournament, and the RPG-styled horde-mode gameplay that ties this mod together. 

- ModDB Page: http://www.moddb.com/mods/firefight-reloaded
- BSNOOCH Board: http://bsnooch.com/forums/index.php?board=68.0
- Steam Store page: http://store.steampowered.com/app/397680

**NOTE: PLEASE NOTE THAT THIS REPOSITORY WILL HAVE UNFINISHED OR BUGGY CODE FROM A NEWER RELEASE OF FIREFIGHT RELOADED. KEEP THIS IN MIND BEFORE COMPILING OR FORKING.**

As this is a Half-Life 2/Source Engine mod, a file named LICENCE-VALVE.md is included which includes Valve's Source 1 SDK licence.\
All code written by Valve and Bitl are covered by this licence.\
Code used in FIREFIGHT RELOADED may be used for your Source Engine game/mod, however you must give credit to Bitl if you do use code from this mod into your Source SDK project.

## Compiling

### WINDOWS:

To be able to build FIREFIGHT RELOADED you will need to download:
* Visual Studio 2013 or later
* C++ Build Tools for Visual Studio 2013 (Not required if you already have Visual Studio 2013)
* Multibyte MFC Library for Visual Studio 2013

Read this for more details: https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Windows

You must also run this command in your CMD with administrator privileges in order to get VPC to build a project:
* REG ADD "HKLM\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\10.0\Projects\{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" /v DefaultProjectExtension /t REG_SZ /d vcproj /f

Note that the Microsoft Speech API is only required for certain SDK tools.

Creating a solution is as simple as running sp/src/creategameprojects.bat, then opening up the solution in Visual Studio.

### LINUX:
Read this first to install the steam runtime and other necessary components.\
https://developer.valvesoftware.com/wiki/Source_SDK_2013#Source_SDK_2013_on_Linux \
After installing it and other dependencies, do the 3 steps below.

1. CD to your FIREFIGHT RELOADED git directory, I.E.\
```cd ~/projects/FIREFIGHT-RELOADED-src-sdk-2013```

1. Run the steam-runtime. NOTE: Make sure you have the steam runtime installed in /valve.\
```bash ./sandbox_setup.sh```

3. Go back to this directory and run:\
```bash ./build_fr_linux.sh```\
This will update the repo automatically. If you would like to update the repo without building it, run:\
```bash ./update_fr_linux.sh```

[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com) [![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
