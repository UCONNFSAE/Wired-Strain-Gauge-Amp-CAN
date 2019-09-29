
#include <SPI.h>
#include "mcp_can.h"

#define SPI_CAN 9

int diffV_A, diffV_B, calOut_A, calOut_B, intTemp_A, intTemp_B, extTemp_A, extTemp_B = 0;
int diffV, calOut, intTemp, extTemp = 0;
String alpha_diffV, alpha_calOut, alpha_intTemp, alpha_extTemp = "";

MCP_CAN CAN(SPI_CAN);

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
  unsigned char len = 0;
  unsigned char buf[8];

  if(CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buf);
    unsigned int canID = CAN.getCanId();
    //String ID = String(canID);
    //Serial.println(ID);

    if(canID == 0x4E5) { // Strain Gauge Amplifier address (1253d)
        diffV_A = buf[0];
        diffV_B = buf[1];
        calOut_A = buf[2];
        calOut_B = buf[3];
        intTemp_A = buf[4];
        intTemp_B = buf[5];
        extTemp_A = buf[6];
        extTemp_B = buf[7];

        diffV = diffV_A * 256 + diffV_B;
        calOut = (calOut_A * 256 + calOut_B) * 0.1;
        intTemp = (intTemp_A * 256 + intTemp_B) * 0.1;
        extTemp = (extTemp_A * 256 + extTemp_B) * 0.1;

        alpha_diffV = String(diffV);
        alpha_calOut = String(calOut);
        alpha_intTemp = String(intTemp);
        alpha_extTemp = String(extTemp);

        Serial.println("Differential Voltage in uV: " + alpha_diffV);
        Serial.println("Calibrated Force Output in N: " + alpha_calOut);
        Serial.println("Internal Temperature in C: " + alpha_intTemp);
        Serial.println("External Temperature in C: " + alpha_extTemp);
        Serial.println();

        delay(500);
    }
  }
}
