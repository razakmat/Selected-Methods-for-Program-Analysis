#!/bin/bash
if [ $1 = "run" ]
then
if [ $2 = "--ascii" ]
then
	./microc/uc export --indent 2 --output $3.json $3
	./interpreter $3.json 0
else
	./microc/uc export --indent 2 --output $2.json $2
	./interpreter $2.json
fi
else
    echo "Unknown command: $1. Supported commands: run"
fi
