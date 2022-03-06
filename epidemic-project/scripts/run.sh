#!/bin/bash

if [[ $(pwd) != */projet-epidemie ]]; then
  echo "Error, you should execute this script in projet-epidemie folder. Exiting"
  exit
fi

echo "Compiling first"

source ./scripts/install.sh

echo "Launching the simulation!"

./build/epidemicSim &
sleep 0.5
./build/pressAgency &
sleep 0.5
./build/citizenManager &
sleep 0.5
./build/window &
wait
echo "End of simulation"