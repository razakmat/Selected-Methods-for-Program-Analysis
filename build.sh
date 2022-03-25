#!/bin/bash

make compile

cd microc
sbt assembly
cd ..

