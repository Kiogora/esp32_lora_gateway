PROTOTYPE NOTES
================

## Hardware

1. For a 20 meter/60 foot CAT 6 ethernet cable, by passive POE, the loss in voltage along the cable is 1.3 volts leaving 10.7 volts reaching the device. The current increases by 30mA. This means 0.039 Watts is dropped through the 20meter/60foot cable.

2. When calculating the peak power taken by the device, care must be taken during calculation. Aggregating values of peak current may lead to an inflated power budget. A missing component in most current calculations is the voltage at which the test was done. 

For example: 

1. LoRa concentrator peak current in Tx(660mA)
2. Cellular module WCDMA peak current in transmission slot(600mA)
3. Ethernet module current (150mA)
4. ESP32, leakage and other misc currents(100mA)

if we add all currents that will be 1510mA and at 12 volts supply that will give 18 Watts.

If we consider also the test voltages taking conservative values:


1. LoRa concentrator peak current in Tx(5v)
2. Cellular module WCDMA peak current in transmission slot(4.3v)
3. Ethernet module current (3.3v)
4. ESP32, leakage and other misc currents(5v)

This gives a total peak power budget of: 6.875 Watts, which is approximately 7 Watts. At gateway occupany, the current at 12 volts gives 583 mA or 600mA approximately.