#ifndef I2CDevice_h
#define I2CDevice_h

#include <Arduino.h>
#include <TinyWireM.h>

///< The class which defines how we will talk to this device over I2C
class I2CDevice {
public:
  I2CDevice(uint8_t addr, USI_TWI *theWire);
  uint8_t address(void);
  bool begin(bool addr_detect = true);
  void end(void);
  bool detected(void);

  bool read(uint8_t *buffer, uint8_t len, bool stop = true);
  bool write(uint8_t *buffer, uint8_t len, bool stop = true,
             uint8_t *prefix_buffer = nullptr, uint8_t prefix_len = 0);
  bool write_then_read(uint8_t *write_buffer, uint8_t write_len,
                       uint8_t *read_buffer, uint8_t read_len,
                       bool stop = false);
  bool setSpeed(uint32_t desiredclk);

  /*!   @brief  How many bytes we can read in a transaction
   *    @return The size of the Wire receive/transmit buffer */
  size_t maxBufferSize() { return _maxBufferSize; }

private:
  uint8_t _addr;
  USI_TWI *_wire;
  bool _begun;
  size_t _maxBufferSize;
  bool _read(uint8_t *buffer, uint8_t len, bool stop);
};

#endif // I2CDevice_h
