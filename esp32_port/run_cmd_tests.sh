#!/bin/bash

#Set preconfigured sdkconfig as working sdkconfig.
cp sdkconfig.cmdline sdkconfig

make -j4 menuconfig flash monitor
