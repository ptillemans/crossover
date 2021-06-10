/**********************************************|
| Crossover project                            |
| https://github.com/ptillemans/crossover      |
|                                              |
| crossover.ino                                |
|**********************************************/

// Include Wire and SigmaDSP library
#include <Wire.h>
#include <SigmaDSP.h>

// Include generated parameter file
#include "SigmaDSP_parameters.h"


// The first parameter is the Wire object we'll be using when communicating wth the DSP
// The second parameter is the DSP i2c address, which is defined in the parameter file
// The third parameter is the sample rate
// An optional fourth parameter is the pin to physically reset the DSP
SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f /*,12*/);


// Only needed if an external i2c EEPROM is present + the DSP is in selfboot mode
// The first parameter is the Wire object we'll be using when communicating wth the EEPROM
// The second parameter is the EEPROM i2c address, which is defined in the parameter file
// The third parameter is the EEPROM size in kilobits (kb)
// An optional fourth parameter is the pin to toggle while writing content to EEPROM
DSPEEPROM ee(Wire, EEPROM_I2C_ADDRESS, 256, LED_BUILTIN);

byte ping(byte address) {
  Serial.print(F("testing address 0x"));
  Serial.println(address, HEX);
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0) {
    Serial.print(F("Device found at address 0x"));
    Serial.println(address,HEX);
  } else if (error==4) {
    Serial.print(F("Unknown error at address 0x"));
    if (address < 16) {
      Serial.print("0");
    }
    Serial.println(address,HEX);
  }
  return error;
}

void setup() 
{
  byte error;

  Wire.begin();
  Serial.begin(9600);
  Serial.println(F("SigmaDSP 0_Template example\n"));

  dsp.begin();
  ee.begin();

  delay(2000);

  
  Serial.println(F("Pinging i2c lines...\n0 -> device is present\n2 -> device is not present"));
  Serial.print(F("DSP response: "));
  error = dsp.ping();

  Serial.print(error, HEX);
  Serial.println(".");

  Serial.print(F("EEPROM response: "));
  error = ee.ping();
  Serial.println(error);
  
 
  // Use this step if no EEPROM is present
  Serial.print(F("\nLoading DSP program... "));
  loadProgram(dsp);
  Serial.println("Done!\n");


  // Comment out the three code lines above and use this step instead if EEPROM is present
  // The last parameter in writeFirmware is the FW version, and prevents the MCU from overwriting on every reboot
  //ee.writeFirmware(DSP_eeprom_firmware, sizeof(DSP_eeprom_firmware), 0);
  //dsp.reset();
  //delay(2000); // Wait for the FW to load from the EEPROM
}


void loop() 
{
  // Nothing to do here!
}
