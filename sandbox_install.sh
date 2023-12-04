#!/bin/sh
echo Downloading steam-runtime...
cd /
mkdir valve
cd valve
wget https://media.steampowered.com/client/runtime/steam-runtime-sdk_latest.tar.xz

echo Installing steam-runtime...
tar xvf steam-runtime-sdk_latest.tar.xz
mv steam-runtime-sdk_2013-09-05 steam-runtime

USERNAME=$(id -u -n)
GROUPNAME=$(id -g -n)
echo Claiming folder ownership as $USERNAME:$GROUPNAME...
chown $USERNAME:$GROUPNAME * -R

echo Running setup...
cd steam-runtime
./setup.sh

echo Running Sandbox shell...
./shell.sh --arch=i386
