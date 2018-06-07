PROTOTYPE NOTES
================

## Hardware

1. For a 20 meter/60 foot CAT 6 ethernet cable, by passive POE, the loss in voltage along the cable is 1.3 volts leaving 10.7 volts reaching the device. The current increases by 30mA. This means 0.039 Watts is dropped through the 20meter/60foot cable.

2. When calculating the peak power taken by the device, care must be taken during calculation. Aggregating values of peak current may lead to an inflated power budget. A missing component in most current calculations is the voltage at which the test was done. 

For example: 

* LoRa concentrator peak current in Tx(660mA)
* Cellular module WCDMA peak current in transmission slot(600mA)
* Ethernet module current (150mA)
* ESP32, leakage and other misc currents(100mA)

if we add all currents that will be 1510mA and at 12 volts supply that will give 18 Watts.

If we consider also the test voltages taking conservative values:


* LoRa concentrator peak current in Tx(5v)
* Cellular module WCDMA peak current in transmission slot(4.3v)
* Ethernet module current (3.3v)
* ESP32, leakage and other misc currents(5v)

This gives a total peak power budget of: 6.875 Watts, which is approximately 7 Watts. At gateway occupany, the current at 12 volts gives 583 mA or 600mA approximately.


3. I notice a 70ms delay on air using a software defined radio when implementing a Wifi backhaul. Tested examples are as follows:


* Up packet timestamp: 637996852 Down pkt timestamp: 647996852. msecs elapsed since boot using high precision timer: 648719. Time of start of uplink transmission: 3.7376 Time of start of downlink transmission: 4.8064. Deviation: 0.0688s=68.8ms.

* Up packet timestamp: 134471339 Down pkt timestamp: 135471339. msecs elapsed since boot using high precision timer: 143174. Time of start of uplink transmission: 5.7216 Time of start of downlink transmission: 6.7904. Deviation: 0.0688s=68.8ms.

* Up packet timestamp: 134471339 Down pkt timestamp: 135471339. msecs elapsed since boot using high precision timer: 143174. Time of start of uplink transmission: 5.7216 Time of start of downlink transmission: 6.7904. Deviation: 0.0688s=68.8ms.

* Up packet timestamp: 1996227395 Down pkt timestamp: 1997227395. msecs elapsed since boot using high precision timer: 2004144. Time of start of uplink transmission: 5.0304 Time of start of downlink transmission: 6.0992. Deviation: 0.0688s=68.8ms.

After setting no output in menuconfig, the deviation is still 68.8ms(approximately). This is end of downlink packet to the start of the uplink packet.

__correction__: The reason for the constant 68.8ms is because this is the packet length time on air. From the LoRa airtime calculator sheet...the packet time of 29 bytes is 66.168ms. The 1 second RX time is exact whether logging is used or not on the ESP32 and is measured from the end of transmission of packet to the beggining of transmission of the downlink packet.

4. The latency of connection over cellular is not good.We are noticing a 1.5-1.6 second roundtrip time. This is detrimental to the 1 second RX1 window slot over cellular. RX1 window has a 1% duty cycle and RX2 has a 10% duty cycle thus transmissions on SF12 may be used on the RX2 window.