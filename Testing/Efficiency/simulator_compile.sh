#!/bin/bash

set -e

g++ -I /usr/local/include/eigen-3.3.7 ../../ProjectFiles/simulator.cpp ../../ProjectFiles/netlist_reader.cpp -o simulator

echo "Success"
