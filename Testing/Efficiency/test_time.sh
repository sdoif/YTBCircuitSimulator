#!/bin/bash
set -e

for i in {1..100}
do
  <test$i.txt time ./simulator >test$i.s.txt
done
