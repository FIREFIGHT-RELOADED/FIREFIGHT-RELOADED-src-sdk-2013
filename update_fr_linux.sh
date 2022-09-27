#!/bin/sh
cd ~/projects/FIREFIGHT-RELOADED-src-sdk-2013
sudo git stash push --include-untracked
sudo git stash drop
sudo git pull origin endgame
