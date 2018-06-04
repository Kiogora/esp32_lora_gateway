PROTOTYPE ERRATA
================

## Hardware errata

### 1. GPS pins swapped

GPS pins(TX-(GPIO15) and RX-(GPIO36/VPN)) are swapped fromthe PCB design stage hence affecting the PCB produced. The effect is that GPIO36/VPN is input only and can't be used for TX to send commands to the GPS module.

#### Workaround

Swap the two TX UART connections and set the setup and RX only. This enables the NMEA messages to still be received but commands can't be sent.

### 2. DHT22 Temperature/Humidity sensor on input only pin

The DHT22 sensor which relies on the Dallas one wire protocol has been setup on an input only pin. The effect is that the module is unable to start up as the microcontroller is supposed to toggle the line in push pull mode in order to signal a start of frame.

#### Workaround

There is currently no workaround for this in software unless the pin that it is connected is set as an input and is connected to a pin with output capability that is not being used by the LAN module due to errata 3.

### 3. LAN module reference oscillator line pulling GPIO0 low on boot

The LAN module is pulling GPIO 0 low on boot. This is regardless of no power(Below minimum VDD) is supplied due to the 3.3v regulator being disabled via its enable pin. The board works as normal without the module attached.

#### Workaround

Currently there is no workaround for this. A probable solution may be to pull the clock enable line low, but it is not known how this may affect the output impedance of the oscillator in the absence of power.

## Software errata

### 1. Time drift affecting downlink

Due to the innacuracy of the RTC of the ESP32, the internal clock reference pulled from NTP may drift. It is thus required that the clock does not drift off by more that 1ms in order not to miss the RX1 window, thus having to rely on the RX2 window to complete the downlink transmission.

#### Workaround

Form a test that stores SNTP time in the RTC and periodically notes the drift of the RTC time from UNIX wall time as a function of monotonic, elapsed time. The drift should not be more than 1 millisecond for the sake of downlink. A second note is the drift of the concetrator micrrosecond counter from the RTC time. This should give a good view of the relation of the three variables(UNIX time, RTC time, SX1301 us conter). __Do note__: There is some latency with querying data over a shared network connection such as cellular or wifi. This will add onto the differenceof the variables.

### 1. USE UNIX UTC time from SNTP to schedule downlinks

Currently the code implementation expects GPS giving GPS time otherwise no downlinks are sent to the nodes.

#### Workaround

Currently pull SNTP time and use that in the downlink task. Also the effect of this should be noted and how it would affect timersync.