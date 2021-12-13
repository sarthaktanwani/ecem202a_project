#!/bin/bash

# Running Scenario_1 10 times
i=1
while [ $i -le 10 ]
do
    echo "Running simulation $i for Scenario 1"
  bash integrator_scenario1_ble.sh
  i=$(( $i + 1 ))
done

# Running Scenario_2 10 times
i=1
while [ $i -le 10 ]
do
    echo "Running simulation $i for Scenario 2"
  bash integrator_scenario2_ble.sh
  i=$(( $i + 1 ))
done