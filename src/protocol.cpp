/*
 * protocol.cpp
 *
 *  Created on: 15 août 2019
 *      Author: tuxin
 */

#include <string.h>
// Frame format : _SOH_ ADDR(1) _STX_ CMD(1) DATA(x) _ETX_ CHK(1) _EOT_

#define _SOH_    0x01
#define _STX_    0x02
#define _ETX_    0x03
#define _EOT_    0x04
#define _ACK_    0x06
#define _NAK_    0x15

#define HOST_ADDR 0x80
#define UNO_ADDR  0x10

#define ACK_MESSAGE    'A'


const int DRIVER_PIN = 2;
const unsigned int REC_BUFFER_SIZE = 20;

static unsigned int rec_pnt;
static byte buffer[REC_BUFFER_SIZE];

void processRecFrame(void);
void processCommand(void);
void sendMessage(byte cmd, byte *param);

void initProtocol(void)
{
    Serial.begin(9600);
    rec_pnt = 0;
}

void processProtocol(void)
{
	byte b;

	if (Serial.available()) {
        // read the most recent byte (which will be from 0 to 255):
		b = Serial.read();
		if ((b == _SOH_) || (rec_pnt >= REC_BUFFER_SIZE)) {
			rec_pnt = 0;
		}
		buffer[rec_pnt++] = b;

		if (b == _EOT_) {
			processRecFrame();
			rec_pnt = 0;
		}
    }
}

void processRecFrame(void)
{
	bool ack = true;
	unsigned int pos = rec_pnt;

    if ((buffer[0] != _SOH_) ||
		(buffer[2] != _STX_) ||
		(buffer[rec_pnt - 3] != _ETX_)) {
    	ack = false;
    }
    else if (buffer[1] == UNO_ADDR) {
    	byte chksum = buffer[rec_pnt - 2];
    	for (pos = 3; pos < rec_pnt - 3; ++pos) {
    		chksum += buffer[pos];
    	}
    	if (chksum != 0) {
    		ack = false;
    	}
    }
    else {
    	return;
    }
	if (ack) {
		//sendAck();
		processCommand();
	}
}

void sendAck(void)
{
	sendMessage(ACK_MESSAGE, 0);
}

void processCommand(void)
{
    switch(buffer[3]) {

    }
}

void sendMessage(byte cmd, byte *param)
{
	// _SOH_ ADDR(1) _STX_ CMD(1) DATA(x) _ETX_ CHK(1) _EOT_
	unsigned int param_len = 0;
	byte chksum = 0;
	unsigned int i;

	if (param) {
		 param_len = strlen(param);
	}

	buffer[0] = _SOH_;
	buffer[1] = HOST_ADDR;
	buffer[2] = _STX_;
	buffer[3] = cmd;
	chksum -= cmd;
	for (i = 0; i < param_len; ++i) {
	    buffer[4 + i] = param[i];
	    chksum -= param[i];
	}
	param_len += 4;
	buffer[param_len++] = _ETX_;
	buffer[param_len++] = chksum;
	buffer[param_len++] = _EOT_;
	Serial.write(buffer, param_len);
}



