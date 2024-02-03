#include <Arduino.h>
#include <MCPAddressUtils.h>

void setup() {
    Serial.begin(9600);

    mcpAddressUtils.begin();
    ReadMCPAddress currentAddress = mcpAddressUtils.getMCPAddress();
    mcpAddressUtils.setMCPAddress(currentAddress.eepromAddress, currentAddress.eepromAddress + 1);
    ReadMCPAddress newAddress = mcpAddressUtils.getMCPAddress();

    Serial.print("Current address: ");
    Serial.println(currentAddress.eepromAddress, BIN);
    Serial.print("New address: ");
    Serial.println(newAddress.eepromAddress, BIN);
}

void loop() {}
