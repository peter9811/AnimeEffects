#!/bin/sh
echo Removing quarentine from AnimeEffects
xattr -d com.apple.quarantine AnimeEffects.app
echo Done, attempting run...
open -a AnimeEffects