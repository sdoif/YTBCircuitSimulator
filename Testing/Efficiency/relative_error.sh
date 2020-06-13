#!/bin/bash
set -e

echo "Compiling simulator..."
g++ -I /usr/local/include/eigen-3.3.7 simulator_err.cpp ../../ProjectFiles/netlist_reader.cpp -o error
echo "Compilation complete"
echo "Generating test cases..."
g++ test_generator.cpp -o test_generator
./test_generator
echo "Test cases generated"

echo "Calculating relative error..."
for i in {1..16}
do
  <test$i.txt ./error >error$i.txt
done
echo "Sucess"
