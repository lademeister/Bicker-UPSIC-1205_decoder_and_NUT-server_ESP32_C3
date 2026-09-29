# Bicker-UPSIC-1205_decoder_and_NUT-server_ESP32_C3
This code does NOT require the optional and expensive PSZ-1063 module, as it reads the registers from the USV module directly via I2C.

It provides:
NUT Server (Network UPS Server) via WiFi
You can use the NUT data on multiple computers in the network, control server shutdown etc. 
MQTT publishing of internal device data
Home Assistant Auto discovery (as MQTT Device)
you can connect a 128x32 OLED Display (shows status and remaining runtime countdown when on battery 
you can connect a programmable LED strip to show charge percentage like a bargraph in multiple colors (green = full, lowering amount of LEDs based on charge level, red when almost empty, flashing red jsut before poweroff) and you can customize it as well.


Typical data reading via MQTT while on net power would look like this:
UPS Charge 100 %
UPS Current 28 mA
UPS Power 0 W
UPS Runtime 0,00 s
UPS Voltage 12 V

The "runtime" is used to LIVE calculate the remaining runtime when running from the supercaps (=on powerloss), for this a live calculation is done based on output current consumption. This is calculated against the internal voltage and internal current draw (before the output voltage converter that creates the stable 12V output).
From that we derive the actual wattage of the output load and from that we can can calculate back against the power capacity of the supercaps.
From that again we can estimate remaining runtime in seconds.

The Current is the actual charge/discharge current, so it will be negative on powerloss and around +/-30mA when normally powered.
Note that this is the actual charge current of the supercaps, so the lower their voltage falls, the higher the current mus be, to maintain constant Power at 12V at the UPS's output. Therefore you may easily see about 6A and more while your 12V device only draws 1,5A. This is because P=U*I (Power=Voltage * current).
So P(powercaps) = P(Output)
Therefore the lower powercap voltage, the higher the current must be for same output after boosting the voltage up to 12V on the putput.

I did the reverse engineering based on a trial-and-error approach without any knowledge of the register layout, so I may be wrong with my assumptions, but it seems that  the data that we derive from the module using this code seems pretty valid.

We decode its internal register data and status like charge percentage, current etc.
This code is written for ESP32-C3, and does NOT require the optional PSZ-1063 addon module. There is no serial converter and no level converter needed.

ESP32 communicates directly to the small 14-pin header on the side of the board.

For I2C communication, we only need:
Pin 11 (GND)
Pin 12 (SDA)
Pin 14 (SCL)
...on this connector.
Be very sure to count correctly, as Pin 13 is directly next to our communication pins and carries 12V, which most likely would fry your ESP32 GPIO pins if you mix it up.
Nevertheless, you could use it as power source for the ESP32, but 300mA is a little on the low end for current spikes during wifi transmission; you might want to add a larger capacitor or just separately power the ESP32.

Make sure to also connect GND between ESP32 and Bicker UPSIC-1205 (Pin 11), as this creates a common voltage reference for the I2C clock and data lines (SCL and SDA).



Nevertheless, here is the full pinout.
You do NOT need the DSUB9 connector, you do NOT need the extension module, and the information you could get from there is also available with this code, directly from the internal registers. Via Network. Via standard NUT integration.

Pinbelegung 
PIN SIGNAL                                     DSUB9
01 DCD am PC – Erkennung Kabel angeschlossen   1
02 DSR am PC – Erkennung Caps Ladezustand      6
03 TXD (wird an RXT am PC angeschlossen)       2
04 RTS am PC – Versorgungspannung              7
05 RXD (wird an TXD am PC angeschlossen)       3
06 CTS am PC – Power Fail Erkennung            8
07 N/A
08 N/A 9
09 GND 5
10 SMBAlert
11 GND
12 xSDA I2C
13 Vout (12V!) (max. 300mA)
14 xSCL I2C
