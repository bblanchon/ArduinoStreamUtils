#!/bin/sh -eux

pip install --user platformio

for EXAMPLE in $PWD/examples/*/*.ino; 
do
	platformio ci "$EXAMPLE" -l '.' -b "$BOARD"
done
