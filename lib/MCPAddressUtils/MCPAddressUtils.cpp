#include <MCPAddressUtils.h>

#define ADDRESS_MASK             (0B00000111)
#define INPUT_REG_ADDRESS_MASK   (0B00001110)

#define GENERAL_CALL_ADDRESS     (0B00000000)
#define READ_ADDRESS_COMMAND     (0B00001100)
#define RESTART_COMMAND          (0B11000001)
#define DEVICE_CODE_COMMAND      (0B11000000)
#define COMMAND_TYPE_COMMAND     (0B01100000)

#define SET_CURRENT_ADDRESS_FLAG (0B00000001)
#define SET_NEW_ADDRESS_FLAG     (0B00000010)
#define REPEAT_NEW_ADDRESS_FLAG  (0B00000011)

#define EEPROM_WRITE_DELAY 100 // ms

MCPAddressUtils::MCPAddressUtils(uint8_t sdaPin, uint8_t sclPin, uint8_t ldacPin)
{
    _sdaPin = sdaPin;
    _sclPin = sclPin;
    _ldacPin = ldacPin;
}

void MCPAddressUtils::begin()
{
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.begin(9600);
#endif

    pinMode(_ldacPin, OUTPUT);
    _i2c.init(_sdaPin, _sclPin);
    delay(250);
}

ReadMCPAddress MCPAddressUtils::getMCPAddress()
{
    digitalWrite(_ldacPin, HIGH);

    int ack1 = _i2c.start(GENERAL_CALL_ADDRESS);
    int ack2 = _i2c.ldacwrite(READ_ADDRESS_COMMAND, _ldacPin); // Set ldac low at the end of command
    int ack3 = _i2c.restart(RESTART_COMMAND);
    uint8_t address = _i2c.read(true);
    _i2c.stop();
    delay(EEPROM_WRITE_DELAY);

    digitalWrite(_ldacPin, HIGH); // Reset after stop bit

#ifdef DEBUG_SERIAL
    Serial.println("Get address acknowledgements: ");
    Serial.print("Ack 1: ");
    Serial.println(ack1);
    Serial.print("Ack 2: ");
    Serial.println(ack2);
    Serial.print("Ack 3: ");
    Serial.println(ack3);
    Serial.print("Address: 0B");
    Serial.println(address, BIN);
#endif

    return ReadMCPAddress{
        static_cast<uint8_t>(address >> 5),
        static_cast<uint8_t>(address | INPUT_REG_ADDRESS_MASK)
    };
}

void MCPAddressUtils::setMCPAddress(uint8_t currentAddress, uint8_t newAddress)
{
  currentAddress &= ADDRESS_MASK;
  newAddress &= ADDRESS_MASK;

  digitalWrite(_ldacPin, HIGH);

  int ack1 = _i2c.start(DEVICE_CODE_COMMAND | (currentAddress << 1));
  int ack2 = _i2c.ldacwrite(COMMAND_TYPE_COMMAND | (currentAddress << 2) | SET_CURRENT_ADDRESS_FLAG, _ldacPin); // Set ldac low at the end of command
  int ack3 = _i2c.write(COMMAND_TYPE_COMMAND | (newAddress << 2) | SET_NEW_ADDRESS_FLAG);
  int ack4 = _i2c.write(COMMAND_TYPE_COMMAND | (newAddress << 2) | REPEAT_NEW_ADDRESS_FLAG);
  _i2c.stop();
  delay(EEPROM_WRITE_DELAY);

  digitalWrite(_ldacPin, HIGH); // Reset after stop bit

#ifdef DEBUG_SERIAL
  Serial.println("Set address acknowledgements: ");
  Serial.print("Ack 1: ");
  Serial.println(ack1);
  Serial.print("Ack 2: ");
  Serial.println(ack2);
  Serial.print("Ack 3: ");
  Serial.println(ack3);
  Serial.print("Ack 4: ");
  Serial.println(ack4);
#endif
}
