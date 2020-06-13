#!/bin/bash
set -e

bash simulator_compile.sh

g++ test_generator_r.cpp -o test_generator
./test_generator

for i in {1..100}
do
  echo "Test $i :"
  <test$i.txt time ./simulator >test$i.s.txt
  echo ""
done
