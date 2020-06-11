#!/bin/bash

set -e

echo "Testing..."

<AC_CapacitorNetlist.txt ./simulator >F_AC_C.txt
<DC_CapacitorNetlist.txt ./simulator >F_DC_C.txt
<AC_InductorNetlist.txt ./simulator >F_AC_L.txt
<DC_InductorNetlist.txt ./simulator >F_DC_L.txt

echo "Output Captured"
