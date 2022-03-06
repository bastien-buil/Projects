#!/bin/bash

if [[ $(pwd) != */projet-epidemie ]]; then
  echo "Error, you should execute this script in projet-epidemie folder. Exiting"
  exit
fi

echo "Compiling everything"

make mrproper
make CONSOLE_MODE=on

echo "-------------------------------------------------"
echo "DONE"
echo "Now everything is setup. You should run each program inside its own terminal"
echo "Run order: epidemicSim -> pressAgency -> citizenManager -> timer"
echo "-------------------------------------------------"

