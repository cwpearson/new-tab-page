#! /bin/bash

set -eou pipefail

PLIST_NAME=com.user.new-tab-server.plist

echo "try to unload ~/Library/LaunchAgents/$PLIST_NAME ..."
launchctl unload ~/Library/LaunchAgents/"$PLIST_NAME"

echo "try to remove ~/Library/LaunchAgents/$PLIST_NAME ..."
rm -v ~/Library/LaunchAgents/"$PLIST_NAME"