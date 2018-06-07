
ESP32 lora gateway project
==========================

## Testing

Two types of tests are included:

1. Normal unit tests.
2. Command-line tests.

### Normal unit tests

These tests are executed as unit tests using the unity testing framework. The unity checks are not strictly implemented but are given as an option for the user to choose. The tests run in a menu.

To run the tests, add a test folder as per component as per unit testing documentation in esp-idf.

These tests are more useful in a lab setup prior to a pilot.

The tests included are:

1.  __loragw_spi__-Tests the SPI reading and writing. This includes normal read/writes and burst read/writes. Alogic analyzer may be used to verify the data sent over the wire and compare to that sent/expected in the test.

2. __loragw_reg__-Tests the reading and writing to the registers of the concentrator over the SPI link.

3. __loragw_gps__-Tests the GPS link by searching for valid NMEA RMC and/or UBX messages.


To compile and flash:

```shell
sh run_unit_tests.sh
```

This will set up the required sdkconfig file and use it to compile and flash the binary to a connected esp32. A concentrator needs to be connected to the spi bus of the device using the loragw_spi component lora pin configuration in menuconfig.

The reason why tests require a separate sdkconfig due to the flash partitions being different from the command line tests and the main program.

### Command-line tests

This use the linenoise and arg3table library provided within the esp-idf framework.

### Flashing the command-line tests and handling the filesystem

Firstly compile the command-line tests residing [here](./cmdline_tests) to a program binary and flash to the esp32, and finally flash the files to the filesystem, which include the json config files using the below commands:

```shell
sh run_cmd_tests.sh
make menuconfig
make -j5 flash 
make flashfs image=cmdline_tests monitor
```

The process for flashing files onto the SPIFFS filesystem is stated [here](./components/loboris_spiffs_image/readme.md). The target flashfs writes the files in the [cmdline_tests folder](./components/loboris_spiffs_image/writeops/cmdline_tests) to the SPIFFS filesystem partition in the esp32 flash. The image argument selects the folder to flash in the [writeops directory](./components/loboris_spiffs_image/writeops). By default if no image argument is passed, the default files whose folder is flashed is the "normal" folder. The normal folder contains files needed during normal gateway operation.

After running any logging tests to a file on the filesystem, one needs to read back the file from the SPIFFS partition to the computer. The commands to do this are stated above with the flashing commands and involve reading back the files and unpacking them from the SPIFFS image format.

To readback the SPIFFS image:

```shell
make readbackfs
make unpackfs
```

### Command-line tests description

These tests are used to test various including new functionality to be implemented on the gateway, that can be useful in-field during a pilot, before integrating it into the main program for deployment.

The individual tests include:

1. __util_spi_stress__-This software is used to check the reliability of the link between the host platform (on which the program is run) and the LoRa concentrator register file that is the interface through which all interaction with the LoRa concentrator happens. Tests the burst and normal read/write modes. Of particular importance is the burst read/write. This should be done in lab before deployment on the target PCB and left to run for a minimum of 24-48 hours. Failure of these tests indicates wiring/layout problems, EMI and crosstalk on the setup. As a prototype solution with the SPI link run over wires, coil a ground wire over the SPI wires to form a shield. On a PCB, the link should be relatively length matched and the various connections separated by grounded copper areas. 
2. __loragw_cal__-Tests the calibration of the radio and loads the calibration firmware into the cores of the SX1301. This calibrates the radio for effects such as image rejection and so on. This should be the first test run prior to running other tests on the radio, except the packet forwarder test, which runs the test inherently.
3. __loragw_hal__-Tests the TX/RX ability of the LoRa radio via it's HAL interface. The test reports any over the air packets received and sent data over the air per time interval.
4. __util_tx_continuous__-This software is used to set LoRa concentrator in Tx continuous mode, for spectral measurement. This test would be useful to test radio transmitted occupied channel bandwidth(OCBW)) and power using a spectrum analyzer and tuning the various gains(Mixer,Digital, PA) in the test and compare against the LUT values provided in the datasheet for output transmit power compliance(not EIRP).
5. __util_pkt_logger__-This software is used to set up a LoRa concentrator using a JSON configuration file and then record all the packets received in a log file, indefinitely. As an extension for this repository as compared to the original LoRa gateway repository, the packet logger parses the payload and displays the various fields of ineterest according to the LoRaWAN frame format [[link]](https://hackmd.io/s/S1kg6Ymo-#). This is useful when testing new devices on the network to test they are transmitting, especially those with hardcoded ABP device addresses from the factory, or debugging.
6. __util_lbt__-Test "Listen-Before-Talk" channels timestamps. Untested as it needs an FPGA to perform a spectral scan on the concentrator which tests if the channel power is above a threshold thus representing occupancy of the channel. 
6. __parson_test__-Tests the functionality of the parson library included in the components folder. The library reads and writes to JSON files using an ANSI C compliant interface.
7. __iperf__-Tests the latency and roundtrip time (RTT) of a given backhaul connection. The most useful setup for Iperf on the esp32 is as a client as it reports data back to a server. This is useful in testing if the RTT may overrun the RX1/RX2 windows on devices, which is 1 usually 1 second after transmission for class A devices. This would be a problem for long RTT backhaul connections such as satellite. This is in addition to the 20ppm clock drift of the esp32 RTC clock from the SNTP wall time.
8. __ping__-Tests the throughput of a given  backhaul connection in Mbps.

## Main program

The main program is the C program which resides [here](./main). The main program is an amalgamation of the basic tests and functionality tests above. This program runs indefinitely and for field updatability, should include over the air(OTA) upgrades.

### Flashing the command-line tests and handling the filesystem

Compile and flash the program binary and files to the esp32 using the commands:

```shell
sh run_cmd_tests.sh
make menuconfig
make -j5 BUILD_NORMAL=1 flash 
make flashfs monitor
```

__TODO__: Find similariies in the main program and the commandline tests and see if this will need a separate sdkconfig, especially with the over the air update functionality changing the flash partitions.