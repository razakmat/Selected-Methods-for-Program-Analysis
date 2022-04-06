#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" > /dev/null && pwd)"

if [ $1 = "run" ] || [ $1 = "type" ]
then
if [ $2 = "--ascii" ]
then
	$SCRIPT_DIR/microc/uc export --indent 2 --output $3.json $3
	$SCRIPT_DIR/APR $3.json $1 0
else
	$SCRIPT_DIR/microc/uc export --indent 2 --output $2.json $2
	$SCRIPT_DIR/APR $2.json $1
fi
else
    echo "Unknown command: $1. Supported commands: run and type"
fi

