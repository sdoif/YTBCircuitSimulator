#!/bin/bash
set -e

echo "Compiling simulator"
g++ -I /usr/local/include/eigen-3.3.7 simulator_err.cpp ../../ProjectFiles/netlist_reader.cpp -o error
echo "Sucess"
echo "Generating test cases"
g++ test_generator.cpp -o test_generator
./test_generator

for i in {1..16}
do
  <test$i.txt ./eroor >error$i.txt
done
