#include <Arduino.h>
#include <string.h>
#include <SoftwareSerial.h>

#include "wheel.h"
#include "transmission.h"
#include "transportProtocol.h"

#define TIMEOUT 1
#define LED_PIN 13
#define USB_SERIAL
#define PAUSE_CNT_MAX 250

#ifdef USB_SERIAL
#define xferSerial Serial
#else
SoftwareSerial xferSerial = SoftwareSerial(0, 1, false);
#endif

XferCommand xchgCommand = XferCommand();

void setup() {
  xferSerial.begin(115200);
  pinMode(LED_PIN, LOW);
}

////////////////////////////////////////////////////////////

typedef struct {
  uint8_t ledCnt;
  uint8_t pauseCnt;
} t_tog;

typedef enum {
  STATE_STDBY = 0,
  STATE_ACTIVE
} e_state;

static t_tog toggle = {0, 0};
static e_state globalState = STATE_STDBY;
static String cmdString = String("");
static e_parse_state parseState = PARSE_UNKNOWN;
//static e_cmd commandType = CMD_UNKNOWN;

void loop() {
#ifndef USB_SERIAL
  if (true == xferSerial.available()) {
    parseState = xferInterface.ParseCommand(xferSerial.readString());
  }
#else
  if (xferSerial.available()) {
    char tmpCmdBytes[7];
    size_t receivedBytes = xferSerial.readBytes((uint8_t *)tmpCmdBytes, 7);

    if (0 != receivedBytes) {
      parseState = xchgCommand.ParseCommand(tmpCmdBytes);
    }
#ifdef DEBUG
    if (PARSE_OK != parseState)
      xferSerial.println("PARSERR");
    else
      xferSerial.println("PARSEOK"); 
#endif
  }
#endif

  switch (globalState) {
    /**
     * ACTIVE Global State
     * Waiting for a command message from a head device to run a control command
     */
    case STATE_ACTIVE: 
      pinMode(LED_PIN, LOW);

        if (PARSE_OK == parseState) {
          
          parseState = PARSE_UNKNOWN;
        }
    break;
    /**
     * STDBY Global State
     * Waiting for an initial message from a head device to run an active state
     */
    case STATE_STDBY: 
        toggle.pauseCnt++;

        if (PAUSE_CNT_MAX == toggle.pauseCnt) {
          toggle.ledCnt++;
          toggle.pauseCnt = 0;
          pinMode(LED_PIN, toggle.ledCnt & 0x1);
        }

        if (PARSE_OK == parseState) {
          parseState = PARSE_UNKNOWN;
        }    
    break;
    
    default:
    break;

  };

  delay(TIMEOUT);
}