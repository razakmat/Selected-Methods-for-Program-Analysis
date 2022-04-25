#!/bin/bash

make compile
make -C ./src/DataAnalysis/ compile

cd microc
sbt assembly
cd ..

