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

elif [ $1 = "cfg" ] || [ $1 = "sign" ] || [ $1 = "const" ] || [ $1 = "live" ] || [ $1 = "available" ] || [ $1 = "busy" ] || [ $1 = "reaching" ]
then
if [ $2 = "--norm" ]
then
	$SCRIPT_DIR/microc/uc export --indent 2 --output $3.json $3
	$SCRIPT_DIR/src/DataAnalysis/DataAnalysis $3.json "norm" $1
else
	$SCRIPT_DIR/microc/uc export --indent 2 --output $2.json $2
	$SCRIPT_DIR/src/DataAnalysis/DataAnalysis $2.json "no" $1
fi

elif [ $1 = "Andersen" ]
then
	$SCRIPT_DIR/microc/uc export --indent 2 --output $2.json $2
	$SCRIPT_DIR/src/Pointers/Pointers $2.json $1

elif [ $1 = "Steensgaard" ]
then
	$SCRIPT_DIR/microc/uc export --indent 2 --output $2.json $2
	$SCRIPT_DIR/src/Pointers/Pointers $2.json $1

elif [ $1 = "Null" ]
then
if [ $2 = "Steensgaard" ]
then
	$SCRIPT_DIR/microc/uc export --indent 2 --output $3.json $3
	$SCRIPT_DIR/src/Null/Null $3.json 1 
else
	$SCRIPT_DIR/microc/uc export --indent 2 --output $2.json $2
	$SCRIPT_DIR/src/Null/Null $2.json
fi

else
    echo "Unknown command: $1. Supported commands: run, type, cfg, sign, const, live, available, busy, reaching, Andersen, Steensgaard, Null"
fi

