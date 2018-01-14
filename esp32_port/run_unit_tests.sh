#!/bin/bash

#The unit tests use the ESP-IDF's unit-test-app sdkconfig definition

#Compile and flash using the ESP-IDF unit-test-app residing on the system. Add project components.
make flash monitor -C ${IDF_PATH}/tools/unit-test-app EXTRA_COMPONENT_DIRS=./components TEST_COMPONENTS='loragw_spi loragw_reg'
