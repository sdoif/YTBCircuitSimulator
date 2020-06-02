#!/bin/bash
set -e

g++ -g netlist_reader_test.cpp netlist_reader.cpp -o netlist_reader_test

echo "Compiled"

<netlist_ex1.txt ./netlist_reader_test >netlist_ex1_out.txt

echo "Success"
