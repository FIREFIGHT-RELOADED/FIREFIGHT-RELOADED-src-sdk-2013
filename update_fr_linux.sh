#!/bin/sh
cd -
sudo git stash push --include-untracked
sudo git stash drop
sudo git pull origin endgame
