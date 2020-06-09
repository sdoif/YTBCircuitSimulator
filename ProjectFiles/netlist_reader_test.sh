#!/bin/bash
set -e

g++ netlist_reader_test.cpp netlist_reader.cpp -o netlist_reader_test

echo "Compiled"

<netlist_ex1.txt ./netlist_reader_test >netlist_ex1_out.txt

diff netlist_ex1_out.txt netlist_ex1_out.ref.txt

echo "Success"
