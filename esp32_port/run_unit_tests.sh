#!/bin/bash

#The unit tests use the ESP-IDF's unit-test-app sdkconfig definition

#Compile and flash using the ESP-IDF unit-test-app residing on the system. Add project components.

project_dir=$(pwd)
#Must read the extra component directories before running menuconfig in order to load configuration.
cd ${IDF_PATH}/tools/unit-test-app
make EXTRA_COMPONENT_DIRS=${project_dir}/components menuconfig flash monitor TEST_COMPONENTS='loragw_spi loragw_reg parson_test'
cd ${project_dir}