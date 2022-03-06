#!/bin/bash

if [[ $(pwd) != */projet-epidemie ]]; then
  echo "Error, you should execute this script in projet-epidemie folder. Exiting"
  exit
fi

echo "Compiling everything"

make mrproper
make SILENCE_MODE=on

echo "-------------------------------------------------"
echo "DONE"
echo "Installation has completed"
echo "-------------------------------------------------"