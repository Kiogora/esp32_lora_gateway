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

SNTP fails to be fetched even after the 10 retries.

#### Workaround

Form a task that fetches SNTP time from the time servers after every so often instead of just at start incase it fails to obtain SNTP time at start. How often is a function of drift of RTC time from the ESP32 time. Ss per this: https://github.com/espressif/esp-idf/issues/769 , the unit to use forworst case crystals is 20ppm thus we update the clock per 500second interval for a maximum drift of 10ms in order not to to have a deviation of 1s +/- 10ms of the downlink RX1 window.