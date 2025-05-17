#!/bin/sh
echo "Removing quarantine from AnimeEffects"
xattr -d com.apple.quarantine AnimeEffects.app
echo "Done, attempting run..."
open -a AnimeEffects