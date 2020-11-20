#!/bin/bash -eux

pip install --user platformio

read -ra EXAMPLES_ARRAY <<< "$EXAMPLES"

for EXAMPLE in "${EXAMPLES_ARRAY[@]}"; 
do
	platformio ci "$PWD/examples/$EXAMPLE/$EXAMPLE.ino" -l '.' -b "$BOARD"
done
