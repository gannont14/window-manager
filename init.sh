#!/bin/sh
set -e 
make build

# Start Xephyr directly
Xephyr -ac -screen 1380x720 -host-cursor :1 &
XEPHYR_PID=$!
sleep 1  # Wait for Xephyr to start

# Export the display variable
export DISPLAY=:1
echo "Checking for running window managers:"
xlsclients

# Run xeyes
# xeyes &
# XEYES_PID=$!
# sleep 1

# Start the window manager
./main &
WM_PID=$!


# Keep the script running until the user presses Ctrl+C
echo "Press Ctrl+C to exit"
trap "kill $WM_PID $XEYES_PID $XEPHYR_PID 2>/dev/null || true" EXIT
wait $WM_PID
