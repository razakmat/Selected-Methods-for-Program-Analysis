#!/bin/bash

make compile
make -C ./src/DataAnalysis/ compile
make -C ./src/Pointers/ compile
make -C ./src/Null/ compile

cd microc
sbt assembly
cd ..

