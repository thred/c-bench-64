#!/bin/bash

flatpak run net.sf.VICE \
    -config ../src/vice.ini \
    -autostart "$1" \
    -initbreak $(VICETRAP) \
    -moncommands ../src/vice.mon \
    -monlog \
    -monlogname monitor.log &

# # Wait for it to start and open port
# sleep 5

# # Connect and send monitor commands
# {
#   echo "break $C000"                 # or your own breakpoint
#   echo "cont"
#   sleep 2
#   echo "savebin \"memdump.bin\" 0x0400 0x07E8"
#   echo "quit"
# } | nc localhost 6510
