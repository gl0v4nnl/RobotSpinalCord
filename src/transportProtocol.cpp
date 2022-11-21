#include "transportProtocol.h"
#include <WString.h>

#define TYPE_SET_STR  "S"
#define TYPE_GET_STR  "G"
#define TYPE_INFO_STR "I"

#define TYPE_SET_CHR  'S'
#define TYPE_GET_CHR  'G'
#define TYPE_INFO_CHR 'I'

xferInterf::xferInterf() {
    parseState = PARSE_OK;
    cmdType = CMD_TYPE_UNKNOWN;
    memset((void*)charParam, 0, sizeof(charParam));
    uintParam = 0; 
}

e_parse_state xferInterf::ParseCommand(String usrString) {
    parseState = PARSE_OK;

    if (usrString.length() < 2) {
        parseState = PARSE_TOO_SMALL;
        return parseState;
    }
    if (usrString.length() > 7) {
        parseState = PARSE_TOO_LONG;
        return parseState;
    }

    if (usrString.substring(0,0) == TYPE_SET_STR)
        cmdType = CMD_TYPE_SET;
    if (usrString.substring(0,0) == TYPE_GET_STR)
        cmdType = CMD_TYPE_GET;
    if (usrString.substring(0,0) == TYPE_INFO_STR)
        cmdType = CMD_TYPE_INFO;

    usrString.toCharArray(charParam, 1, 3);

    String tempUintParam;
    tempUintParam.getBytes((uint8_t *)usrString.begin(), 4, 3);
    uintParam = (uint16_t)tempUintParam.toInt();

    return parseState;
}

e_parse_state xferInterf::ParseCommand(char * usrBytes, uint8_t length) {
    parseState = PARSE_OK;

    if (length < 2) {
        parseState = PARSE_TOO_SMALL;
        return parseState;
    }
    if (length > 7) {
        parseState = PARSE_TOO_LONG;
        return parseState;
    }

    if (usrBytes[0] == TYPE_SET_CHR)
        cmdType = CMD_TYPE_SET;
    if (usrBytes[0] == TYPE_GET_CHR)
        cmdType = CMD_TYPE_GET;
    if (usrBytes[0] == TYPE_INFO_CHR)
        cmdType = CMD_TYPE_INFO;

    charParam[0] = usrBytes[1];
    charParam[1] = usrBytes[2];
    charParam[2] = usrBytes[3];

    String tempUintParam;
    tempUintParam.getBytes((uint8_t *)usrBytes, 4, 3);
    uintParam = (uint16_t)tempUintParam.toInt();

    return parseState;
}

e_parse_state xferInterf::GetLastParseState(void) const {
    return parseState;
}

e_cmd_type xferInterf::GetType(void) const {
    return cmdType;
}
char xferInterf::GetCharParamByIndex(uint8_t usrIndex) const {
    return charParam[usrIndex];
}

uint8_t xferInterf::GetUintParam(void) const {
    return uintParam;
}
