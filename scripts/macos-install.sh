#! /bin/bash

set -eou pipefail

SERVER_REL_PATH=build/server

if [ ! -f "$SERVER_REL_PATH" ]; then
  echo "Ensure server is compiled"
  exit 1
fi
echo found server binary at "$SERVER_REL_PATH"

SERVER_ABS_PATH=$(realpath "$SERVER_REL_PATH")

echo a.k.a. "$SERVER_ABS_PATH"

PLIST_NAME=com.user.new-tab-server.plist

echo create "$PLIST_NAME"
cat > "$PLIST_NAME" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.user.new-tab-server</string>
    <key>ProgramArguments</key>
    <array>
        <string>$SERVER_ABS_PATH</string>
        <string>8081</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
    <key>KeepAlive</key>
    <true/>
    <key>StandardOutPath</key>
    <string>/tmp/new-tab-server.stdout</string>
    <key>StandardErrorPath</key>
    <string>/tmp/new-tab-server.stderr</string>
</dict>
</plist>
EOF

set -x
chmod 644 "$PLIST_NAME"
set +x

mv -v "$PLIST_NAME" ~/Library/LaunchAgents/"$PLIST_NAME"

set -x
launchctl load ~/Library/LaunchAgents/"$PLIST_NAME"
set +x

launchctl list | grep com.user.new-tab-server
