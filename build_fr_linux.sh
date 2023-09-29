#!/bin/sh
cd -
sudo git stash push --include-untracked
sudo git stash drop
sudo git pull origin endgame
cd ./sp/src
sudo chmod -R +x devtools/*
sudo chmod -R +x devtools/bin
sudo chmod -R +x devtools/bin/linux
sudo chmod +x createfirefightreloadedproject
sudo bash createfirefightreloadedproject
sudo make -f firefight.mak
