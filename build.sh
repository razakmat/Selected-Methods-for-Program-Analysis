#!/bin/bash

make compile
make -C ./src/DataAnalysis/ compile
make -C ./src/Pointers/ compile

cd microc
sbt assembly
cd ..

