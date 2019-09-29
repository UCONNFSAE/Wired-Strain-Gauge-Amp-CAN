
#include <SPI.h>
#include "mcp_can.h"

#define SPI_CAN 9

MCP_CAN CAN(SPI_CAN);

uint8_t pgmA = 01001110,      // Programming constant, MSB of 20000d or 0x4E20h
        pgmB = 0b00100000,    // Programming constant, LSB of 20000d or 0x4E20h
        gainC_A = 0b00000000, // Gain Coefficient, MSB
        gainC_B = 0b00000001, // Gain Coefficient, LSB
        gainE = 0,            // Gain Exponent
        offsetC_A = 0,        // Offset Coefficient, MSB
        offsetC_B = 0,        // Offset Coefficient, LSB
        offsetE = 0;          // Offset Exponent

//CAN Data message to send
uint8_t cfg_amp[8] = {pgmA, pgmB, gainC_A, gainC_B, gainE, offsetC_A, offsetC_B, offsetE};

int n = 1;

void setup() {

  Serial.begin(9600);

  while(CAN_OK != CAN.begin(CAN_1000KBPS))
  {
    Serial.println("CAN BUS Shield Init Failed. Retrying...");
    delay(100);
  }
  Serial.println("CAN BUS Shield Init OK!");
}

void loop() {
  while(n == 1)
  {
    Serial.println("Configuring Amplifier...");
    for(int i = 0; i <= 12; i++)  // Send a total of 10 times (12 just in case)
    {
      CAN.sendMsgBuf(0x4E5, 0, 8, cfg_amp); // (Target address, Standard frame, Length of frame, Data array)
      delay(1000);  // Send 1Hz interval
    }
    Serial.println("Amplifier Config Done! Please cycle power to sensor now.");

    n = 0;
  }
}
