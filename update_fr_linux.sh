#!/bin/sh
sudo git stash push --include-untracked
sudo git stash drop
sudo git pull origin endgame
