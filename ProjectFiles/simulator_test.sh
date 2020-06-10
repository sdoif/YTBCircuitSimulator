#!/bin/bash
set -e

echo "Compiling..."
bash simulator_compile.sh
echo "Compilation success!"

echo "Running Test 1"
<netlist1.txt ./simulator >simdata1.txt
echo "Running Test 2"
<netlist2.txt ./simulator >simdata2.txt
echo "Running Test 3"
<netlist3.txt ./simulator >simdata3.txt
echo "Running Test 4"
<netlist_ex1.txt ./simulator >simdata.txt
