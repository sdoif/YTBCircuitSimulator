#!/bin/bash

set -e

echo "Testing..."

<AC_CapacitorNetlist ./simulator >F_AC_C.txt
<DC_CapacitorNetlist ./simulator >F_DC_C.txt
<AC_InductorNetlist ./simulator >F_AC_L.txt
<DC_InductorNetlist ./simulator >F_DC_L.txt

echo "Output Captured"
