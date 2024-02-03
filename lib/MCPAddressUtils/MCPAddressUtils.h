#ifndef MCP_ADDRESS_UTILS_H
#define MCP_ADDRESS_UTILS_H

#include <Arduino.h>
#include <SoftI2cMaster.h>

#define SDA_PIN 0
#define SCL_PIN 1
#define LDAC_PIN 2

typedef struct ReadMCPAddress
{
    uint8_t eepromAddress;
    uint8_t inputRegisterAddress;
} ReadMCPAddress;

class MCPAddressUtils
{
public:
    MCPAddressUtils(uint8_t sdaPin = SDA_PIN, uint8_t sclPin = SCL_PIN, uint8_t ldacPin = LDAC_PIN);
    void begin();
    ReadMCPAddress getMCPAddress();
    void setMCPAddress(uint8_t currentAddress, uint8_t newAddress);

private:
    uint8_t _sdaPin;
    uint8_t _sclPin;
    uint8_t _ldacPin;
    SoftI2cMaster _i2c;
};

extern MCPAddressUtils mcpAddressUtils;

#endif
