# ATTiny85 MIDI to CV converter

MIDI to CV converter for AE modular systems (based on ATTiny85 and MCP4728 chips)

## Dependencies

### External (via platformIO)
- [Adafruit MCP4728 library](https://github.com/adafruit/Adafruit_MCP4728)
- [Adafruit BusIO library](https://github.com/adafruit/Adafruit_BusIO)

### External (included in repo)
- [ReadOnlySoftwareSerial](http://gammon.com.au/Arduino/ReceiveOnlySoftwareSerial.zip)

## Setup

### MCP I2C addresses
Expected address for the first and second MCP DACs are 0 and 1 respectively (you can use the `MCPAddressUtils` library to change the address).

This is achieved by using the [SoftI2cMaster](https://github.com/TrippyLighting/SoftI2cMaster/tree/master) bit-banging library rather than the custom adapted MCP4728 library due to the needed LDAC pin behavior that can't be reproduced easily using I2C communication.

An example on how to change the address of a MCP dac can be found in the library `example` folder or [here](https://github.com/jknipper/mcp4728_program_address/blob/master/mcp4728_program_address.ino)

### Adafruit MCP4728 library compilation error
To fix the issue compiling Adafruit MCP4728 library change the begin function implementation in Adafruit_MCP4728.cpp to return `bool` rather than `boolean`

**TO DO** I should create a PR to fix this at the source.
