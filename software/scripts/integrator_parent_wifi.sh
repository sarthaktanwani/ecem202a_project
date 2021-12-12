#!/bin/bash

i=1
while [ $i -le 10 ]
do
    echo "Running simulation $i for Scenario 1"
  bash integrator_scenario1_wifi.sh
  i=$(( $i + 1 ))
done

i=1
while [ $i -le 10 ]
do
    echo "Running simulation $i for Scenario 2"
  bash integrator_scenario2_wifi.sh
  i=$(( $i + 1 ))
done