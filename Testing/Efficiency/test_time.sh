#!/bin/bash
set -e

g++ test_generator.cpp -o test_generator
./test_generator

for i in {1..100}
do
  <test$i.txt time ./simulator >test$i.s.txt
done
