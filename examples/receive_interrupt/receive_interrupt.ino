// demo: CAN-BUS Shield, receive data with interrupt mode
// when in interrupt mode, the data coming can't be too fast, must >20ms, or else you can use check mode
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"

MCP_CAN CAN(10);                                            // Set CS to pin 10

volatile bool Flag_Recv = false;

void setup() {
    Serial.begin(115200);

START_INIT:

    if (CAN_OK == CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)) {                // init can bus : baudrate = 500k
        Serial.println("CAN BUS Shield init ok!");
    } else {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }

    attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
}

void MCP2515_ISR() {
    Flag_Recv = true;
}

void loop() {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];
    cli();
    bool data_received = Flag_Recv;
    Flag_Recv = false;
    sei();
    if (data_received) {                          // check if get data
        CAN.readMsgBuf(&rxId, &len, rxBuf);       // Read data: len = data length, buf = data byte(s)

        Serial.print("ID: ");                     // Print the message ID.
        Serial.print(rxId, HEX);

        Serial.print("  Data: ");
        for (int i = 0; i < len; i++) {           // Print each byte of the data.
            if (rxBuf[i] < 0x10) {                // If data byte is less than 0x10, add a leading zero.
                Serial.print("0");
            }
            Serial.print(rxBuf[i], HEX);
            Serial.print(" ");
        }

        Serial.println();
    }
}
