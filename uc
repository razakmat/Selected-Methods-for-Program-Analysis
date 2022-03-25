#!/bin/bash
if [ $1 = "run" ] || [ $1 = "type" ]
then
if [ $2 = "--ascii" ]
then
	./microc/uc export --indent 2 --output $3.json $3
	./APR $3.json $1 0
else
	./microc/uc export --indent 2 --output $2.json $2
	./APR $2.json $1
fi
else
    echo "Unknown command: $1. Supported commands: run and type"
fi

