
ESP32 lora gateway project
==========================

# 1. Testing

Two types of tests are included:

1. Normal unit tests.
2. Command-line tests.

## Normal unit tests

These tests are executed as unit tests using the unity testing framework. The unity checks are not strictly implemented but are given as an option for the user to choose. The tests run in a menu.

To run the tests, add a test folder as per component as per unit testing documentation in esp-idf.

These tests are more useful in a lab setup prior to a pilot.

The tests included are:

1. __loragw_spi__-Tests the SPI reading and writing. This includes normal read/writes and burst read/writes. Alogic analyzer may be used to verify the data sent over the wire and compare to that sent/expected in the test.

2. __loragw_reg__-Tests the reading and writing to the registers of the concentrator over the SPI link.

3. __loragw_gps__-Tests the GPS link by searching for valid NMEA RMC and/or UBX messages.


To compile and flash:

```shell
sh run_unit_tests.sh
```

This will set up the required sdkconfig file and use it to compile and flash the binary to a connected esp32. A concentrator needs to be connected to the spi bus of the device using the loragw_spi component lora pin configuration in menuconfig.

The reason why tests require a separate sdkconfig due to the flash partitions being different from the command line tests and the main program.

## Command-line tests

This use the linenoise and arg3table library provided within the esp-idf framework.

* __Flashing the command-line tests and handling the filesystem__

Firstly compile the command-line tests residing [here](./cmdline_tests) to a program binary and flash to the esp32, and finally flash the files to the filesystem, which include the json config files using the below commands:

```shell
make menuconfig
make -j5 BUILD_TESTS=1 flash
make flashfs image=cmdline_tests monitor
```

The process for flashing files onto the SPIFFS filesystem is stated [here](./components/loboris_spiffs_image/readme.md). The target flashfs writes the files in the [cmdline_tests folder](./components/loboris_spiffs_image/writeops/cmdline_tests) to the SPIFFS filesystem partition in the esp32 flash. The image argument selects the folder to flash in the [writeops directory](./components/loboris_spiffs_image/writeops). By default if no image argument is passed, the default files whose folder is flashed is the "main" folder in the writeops directory. The main folder contains files needed during normal gateway operation.

After running any logging tests to a file on the filesystem, one needs to read back the file from the SPIFFS partition to the computer. The commands to do this are stated above with the flashing commands and involve reading back the files and unpacking them from the SPIFFS image format.

To readback the SPIFFS image:

```shell
make readbackfs
make unpackfs
```

* __Command-line tests description__

These tests are used to test various including new functionality to be implemented on the gateway, that can be useful in-field during a pilot, before integrating it into the main program for deployment.

__utilities:__ In order to restart the tests by reboot just enter the following in the prompt: 

```
loragw test console>restart
```

__connectivity prerequisites:__ Some tests need an internet connection before running properly. The various network connection methods are:

__cellular connection:__
Uses the information on the APN, password and auth protocol in menuconfig.

__Usage:__

```
loragw test console>cellular_connect
```

__Wifi connection:__
Uses the information on the APN, password and auth protocol in menuconfig.

__Usage:__

```
loragw test console>loragw test console>sta  "WIFI_SSID" "PASSKEY"
  WiFi is station mode, join specified soft-AP
    "WIFI_SSID"  SSID of AP
    "PASSKEY"  password of AP
```

The individual tests include:

+ __util_spi_stress__-This software is used to check the reliability of the link between the host platform (on which the program is run) and the LoRa concentrator register file that is the interface through which all interaction with the LoRa concentrator happens. Tests the burst and normal read/write modes. Of particular importance is the burst read/write. This should be done in lab before deployment on the target PCB and left to run for a minimum of 24-48 hours. Failure of these tests indicates wiring/layout problems, EMI and crosstalk on the setup. As a prototype solution with the SPI link run over wires, coil a ground wire over the SPI wires to form a shield. On a PCB, the link should be relatively length matched and the various connections separated by grounded copper areas. 

__Usage:__

```
loragw test console>Util_spi_stress  -t 4 -n 2500
    -t 4  <int> specify which test you want to run (1-4)
    -n 2500  <int> specify read write cycles to perform, max 3000, beyond 3000 => infinite
```

+ __loragw_cal__-Tests the calibration of the radio and loads the calibration firmware into the cores of the SX1301. This calibrates the radio for effects such as image rejection and so on. This should be the first test run prior to running other tests on the radio, except the packet forwarder test, which runs the test inherently.

__Usage:__

```
loragw test console>loragw_cal  -a 867.5 -b 868.5 -r 1257 -n 1 -k 1 -t 1
    -a 867.5  <float> Radio A frequency in MHz
    -b 868.5  <float> Radio B frequency in MHz
    -r 1257  <int> Radio type (SX1255:1255, SX1257:1257)
    -n 1  <uint> Number of calibration iterations
    -k 1  <int> Concentrator clock source (0:radio_A, 1:radio_B(default))
    -t 1  <int> Radio to run TX calibration on (0:None(default), 1:radio_A, 2:radio_B, 3:both)
```

+ __loragw_hal__-Tests the TX/RX ability of the LoRa radio via it's HAL interface. The test reports any over the air packets received and sent data over the air per time interval.

__Usage:__

```
loragw test console>loragw_hal  -a 867.5 -b 868.5 -t 868.0 -r 1257 -k 1
    -a 867.5  Radio A RX frequency in MHz
    -b 868.5  Radio B RX frequency in MHz
    -t 868.0  Radio TX frequency in MHz
    -r 1257  Radio type (SX1255:1255, SX1257:1257)
    -k 1  Concentrator clock source (0: radio_A, 1: radio_B(default))
```

+ __util_tx_continuous__-This software is used to set LoRa concentrator in Tx continuous mode, for spectral measurement. This test would be useful to test radio transmitted occupied channel bandwidth(OCBW)) and power using a spectrum analyzer and tuning the various gains(Mixer,Digital, PA) in the test and compare against the LUT values provided in the datasheet for output transmit power compliance(not EIRP).

__Usage:__

```
loragw test console>util_continuous_tx  -f <F> -r <R> --notch=<NOTCH> --dac=<DAC> --dig=<DIG> --mix=<MIX> --pa=<PA> --mod=<MOD> --sf=<SF> --bw=<BW> --br=<BR> --fdev=<FDEV> --bt=<BT>
    -f <F>  <float>  Tx RF frequency in MHz [800:1000]
    -r <R>  <int>    Radio type (SX1255:1255, SX1257:1257)
    --notch=<NOTCH>  <uint>   Tx notch filter frequency in KhZ [126..250]
    --dac=<DAC>  <uint>   DAC gain trim, [0:3] (0:1, 1:7/8, 2:3/4, 3:1/2)
    --dig=<DIG>  <uint>   Digital gain trim
    --mix=<MIX>  <uint>   Radio Tx mixer gain trim, [0:15](15 corresponds to maximum gain, 1 LSB corresponds to 2dB step)
    --pa=<PA>  <uint>   PA gain trim, [0:3]
    --mod=<MOD>  <char>   Modulation type ['LORA','FSK','CW']
    --sf=<SF>  <uint>   LoRa Spreading Factor, [7:12]
    --bw=<BW>  <uint>   LoRa bandwidth in kHz, [125,250,500]
    --br=<BR>  <float>  FSK bitrate in kbps, [0.5:250]
    --fdev=<FDEV>  <uint>   FSK frequency deviation in kHz, [1:250]
    --bt=<BT>  <uint>   FSK gaussian filter BT trim, [0:3]
```

+ __util_pkt_logger__-This software is used to set up a LoRa concentrator using a JSON configuration file and then record all the packets received in a log file, indefinitely. As an extension for this repository as compared to the original LoRa gateway repository, the packet logger parses the payload and displays the various fields of ineterest according to the LoRaWAN frame format [[link]](https://hackmd.io/s/S1kg6Ymo-#). This is useful when testing new devices on the network to test they are transmitting, especially those with hardcoded ABP device addresses from the factory, or debugging. The logged CSV file from this test can be readback and unpacked for analysis as detailed in the command line tests description.

__Usage:__

```
loragw test console>util_pkt_logger  -r 60s
    -r 60s  (int) Log rotate interval in seconds
```

+ __util_lbt__-Test "Listen-Before-Talk" channels timestamps. Untested as it needs an FPGA to perform a spectral scan on the concentrator which tests if the channel power is above a threshold thus representing occupancy of the channel.

__Usage:__

```
loragw test console>loragw_lbt  -f <F> -o <O> -r <R> -s <S>
    -f <F>  <float> frequency in MHz of the first LBT channel
    -o <O>  <int>   offset in dB to be applied to the SX127x RSSI [-128..127]
    -r <R>  <int>   target RSSI: signal strength target used to detect if the channel is clear or not [-128..0]
    -s <S>  <uint>  scan time in µs for all 8 LBT channels [128,5000]
```

+ __parson_test__-Tests the functionality of the parson library included in the components folder. The library reads and writes to JSON files using an ANSI C compliant interface.

__Usage:__

```
loragw test console>parson_test
```

+ __iperf__-Tests the latency and roundtrip time (RTT) of a given backhaul connection. The most useful setup for Iperf on the esp32 is as a client as it reports data back to a server. This is useful in testing if the RTT may overrun the RX1/RX2 windows on devices, which is 1 usually 1 second after transmission for class A devices. This would be a problem for long RTT backhaul connections such as satellite. This is in addition to the 20ppm clock drift of the esp32 RTC clock from the SNTP wall time.

__Usage:__

```
loragw test console>iperf  [-sua] [-c <ip>] [-p <port>] [-i <interval>] [-t <time>]
  iperf command
    -c, --client=<ip>  run in client mode, connecting to <host>
    -s, --server  run in server mode
    -u, --udp  use UDP rather than TCP
    -p, --port=<port>  server port to listen on/connect to
    -i, --interval=<interval>  seconds between periodic bandwidth reports
    -t, --time=<time>  time in seconds to transmit for (default 10 secs)
    -a, --abort  abort running iperf
```

+ __ping__-Tests the throughput of a given  backhaul connection in Mbps.

__Usage:__

```
loragw test console>ping  -n 8.8.8.8 -c 10 -t 1 -d 1
  Run ICMP ping test
    -n 8.8.8.8  <int> specify ipV4 ip address in dot notation
    -c 10  <int> specify ping count per analysis
    -t 1  <int> specify timeout in seconds as an integer
    -d 1  <int> specify delay in seconds as an integer
```

# 2. Main program

The main program is the C program which resides [here](./main). The main program is an amalgamation of the basic tests and functionality tests above. This program runs indefinitely and for field updatability, should include over the air(OTA) upgrades.

* __Flashing the command-line tests and handling the filesystem__

Compile and flash the program binary and files to the esp32 using the commands:

```shell
make menuconfig
make -j5 BUILD_MAIN=1 flash
make flashfs monitor
```

__TODO__: Find similariies in the main program and the commandline tests and see if this will need a separate sdkconfig, especially with the over the air update functionality changing the flash partitions.