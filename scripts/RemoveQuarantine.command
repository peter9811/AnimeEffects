#!/bin/sh
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR"
echo "Removing quarantine from AnimeEffects"
xattr -d com.apple.quarantine AnimeEffects.app/
echo "Done, attempting run..."
open AnimeEffects.app
@