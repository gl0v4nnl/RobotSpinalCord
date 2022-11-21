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

xferInterf xferInterface = xferInterf();

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
//static e_cmd_type commandType = CMD_TYPE_UNKNOWN;

void ImplementCommand(xferInterf usrXferInterface);

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
      parseState = xferInterface.ParseCommand(tmpCmdBytes);
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

          ImplementCommand(xferInterface);

          if (('G' == xferInterface.GetCharParamByIndex(0)) && 
              ('S' == xferInterface.GetCharParamByIndex(1))) {
            toggle.pauseCnt = 0;
            globalState = STATE_STDBY;
          }
          
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
          char tmpCharParam[3] = {
            xferInterface.GetCharParamByIndex(0),
            xferInterface.GetCharParamByIndex(1),
            xferInterface.GetCharParamByIndex(2)
          };

          xferSerial.write((uint8_t *)tmpCharParam, 3);
          xferSerial.write((int)xferInterface.GetUintParam());

          if (('G' == xferInterface.GetCharParamByIndex(0)) && 
              ('A' == xferInterface.GetCharParamByIndex(1))) {
            globalState = STATE_ACTIVE;
          }

          parseState = PARSE_UNKNOWN;
        }    
    break;
    
    default:
    break;

  };

  delay(TIMEOUT);
}

void ImplementCommand(xferInterf usrXferInterface) {
  e_cmd_type usrCmdType = usrXferInterface.GetType();

  if (CMD_TYPE_GET == usrCmdType) {

  }
  else if (CMD_TYPE_SET == usrCmdType) {

  }
  else if (CMD_TYPE_SET == usrCmdType) {

  }
  else {

  }
}