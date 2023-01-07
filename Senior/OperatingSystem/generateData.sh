#! /bin/bash

mkdir input
mkdir source
mkdir thread

for i in {1..1000}
do
    cp in.txt ./input/in$i.txt
done