# Bicker-UPSIC-1205 Decoder and NUT Server for ESP32-C3

This code does **NOT** require the optional and expensive **PSZ-1063 module**, as it reads the registers from the UPS module directly via I²C.

It provides:

- **NUT Server (Network UPS Tools) via Wi-Fi**
  - You can use the NUT data on multiple computers in the network.
  - It can be used for monitoring, server shutdown, etc.

- **MQTT publishing** of internal device data

- **Home Assistant Auto Discovery**
  - The UPS appears as an MQTT device in Home Assistant.

- **Optional 128×32 OLED display**
  - Shows the current UPS status.
  - Shows a remaining-runtime countdown while running on battery/supercapacitor power.

- **Optional programmable LED strip**
  - Can display the charge percentage like a bar graph.
  - Green when full.
  - Fewer LEDs as the charge level decreases.
  - Red when almost empty.
  - Flashing red shortly before power-off.
  - The behavior can be customized.

---

## Example MQTT Data

Typical data while running from mains power may look like this:

```text
UPS Charge  100 %
UPS Current 28 mA
UPS Power   0 W
UPS Runtime 0.00 s
UPS Voltage 12 V
```

The `runtime` value is used to **live-calculate the estimated remaining runtime** when running from the supercapacitors, for example after a power loss.

For this, a live calculation is performed based on the output power consumption.

The calculation uses the internal voltage and current draw, measured before the voltage converter that generates the stable 12 V output.

From this, we derive the actual wattage of the output load and calculate it back against the available energy stored in the supercapacitors.

From that, we can estimate the remaining runtime in seconds.

---

## Current Reading

The `Current` value is the actual charge/discharge current of the supercapacitors.

It will therefore be negative during a power loss and typically around approximately ±30 mA when normally powered.

Note that this is the actual current on the supercapacitor side.

As the supercapacitor voltage drops, the current must increase in order to maintain constant power at the regulated 12 V UPS output.

Therefore, you may easily see currents of around 6 A or more while your connected 12 V device only draws around 1.5 A.

This is because:

```text
P = U × I
```

Power = Voltage × Current

Therefore:

```text
P(supercapacitors) ≈ P(output)
```

The lower the supercapacitor voltage becomes, the higher the current must be to maintain approximately the same output power after boosting the voltage to 12 V.

---

## Reverse Engineering

I did the reverse engineering using a trial-and-error approach, without any knowledge of the internal register layout.

Therefore, some of my assumptions may be wrong.

However, the data derived from the module using this code appears to be quite plausible and consistent.

We decode internal register data and status information such as:

- Charge percentage
- Current
- Voltage
- Runtime
- Power status
- Other UPS status information

This code is written for the **ESP32-C3** and does **NOT** require the optional PSZ-1063 add-on module.

No serial converter or level converter is needed for the connection described here.

The ESP32 communicates directly with the small 14-pin header on the side of the UPSIC-1205.

---

## I²C Connection

For I²C communication, we only need:

```text
Pin 11  GND
Pin 12  SDA
Pin 14  SCL
```

Be very sure to count the pins correctly.

**Pin 13 is directly next to the communication pins and carries 12 V.**

Connecting Pin 13 accidentally to an ESP32 GPIO would most likely damage the ESP32.

Pin 13 could theoretically be used as a power source for the ESP32, but its maximum current of approximately 300 mA is somewhat low considering possible current spikes during Wi-Fi transmission.

You may therefore want to add a sufficiently large capacitor or simply power the ESP32 separately.

Make sure to also connect **GND between the ESP32 and the Bicker UPSIC-1205 using Pin 11**, as this provides the common voltage reference required for the I²C clock and data lines, SCL and SDA.

---

## Full Pinout

For completeness, here is the full connector pinout.

You do **NOT** need the DSUB9 connector.

You do **NOT** need the extension module.

The information normally available through those interfaces can also be accessed with this code directly from the internal registers and made available over the network via the standard NUT integration.


| Pin | Signal | DSUB9 |
|---:|---|---:|
| 01 | DCD at PC – cable connected detection | 1 |
| 02 | DSR at PC – capacitor charge-state detection | 6 |
| 03 | TXD – connected to RX at PC | 2 |
| 04 | RTS at PC – supply voltage | 7 |
| 05 | RXD – connected to TX at PC | 3 |
| 06 | CTS at PC – power-fail detection | 8 |
| 07 | N/A | |
| 08 | N/A | 9 |
| 09 | GND | 5 |
| 10 | SMBAlert | |
| **11** | **🟢 GND** | |
| **12** | **🟢 xSDA / I²C** | |
| 13 | Vout 12 V, max. 300 mA | |
| **14** | **🟢 xSCL / I²C** | |
