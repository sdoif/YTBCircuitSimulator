#!/bin/bash

set -e

g++ -I /usr/local/include/eigen-3.3.7 simulator.cpp netlist_reader.cpp -o simulator

echo "Success"
