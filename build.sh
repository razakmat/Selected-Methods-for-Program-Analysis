#!/bin/bash

sudo apt-get install nlohmann-json3-dev
make compile

cd microc
sbt assembly
cd ..

