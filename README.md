# SparkCore-heatpumpir
A Spark Core library to control split-unit heatpumps over Infrared

A port from parts of ToniA arduino-heatpumpir (https://github.com/ToniA/arduino-heatpumpir/).
Mainly to make a version for Panasonic heatpump "Sverigepumpen"

Warning! I'm NOT a experienced developer, this is made from guessing and googling.

Supported versions should be:
* Panasonic Sverigepumpen
* Panasonic E9/E12-DKE (Panasonic remote control P/N A75C2616)
* Panasonic E9/E12-JKE and E9/E12-NKE

Instructions:
==============

```
#include "PanasonicHeatpumpIR.h"
//Defaults-
#define DEFAULT_TEMP 20;
#define DEFAULT_STATE POWER_ON;

IRSender irSender(D3); // Example Output PIN D3
PanasonicJKWHeatpumpIR *heatpumpIR;

//For this part a function should be created with args for ON/OFF (State) and Temperature (i_Temp).
heatpumpIR = new PanasonicJKWHeatpumpIR();
heatpumpIR->send(irSender, State, MODE_HEAT, FAN_AUTO, i_Temp, VDIR_MUP, HDIR_AUTO);

void setup()
{
  //Create a Spark.function to call.
}

void loop()
{
  
}

```
