#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * The table of commands
 * 
 * +----+------+----------+
 * |TYPE|ENTITY|SUBCOMMAND|
 * +----+------+----------+
 * 
 * +----+----------------------------------------------------+
 * |TYPES :                                                  |
 * |S - SET  - set some entity to needed state               |
 * |G - GET  - get state of needed entity, this is a request |                              
 * |I - INFO - command, addressed from Mega to RPI           |
 * +---------------------------------------------------------+
 * |ENTITIES :                                               |
 * |N - NONE  - no entity, global command                    |
 * |W - WHEEL - addressed to wheel (left or right)           |
 * +---------------------------------------------------------+
 * |SUBCOMMANDS : depend on TYPE and ENTITY                  |
 * +---------------------------------------------------------+
 * 
 */

#define COUNT_OF_CHAR_PARAMS 3
#define MAX_LENGTH 7

typedef enum {
    PARSE_OK = 0,
    PARSE_UNKNOWN = -10,
    PARSE_TOO_SMALL,
    PARSE_TOO_LONG,
    PARSE_WRONG_TYPE,
    PARSE_WRONG_ENTITY,
    PARSE_WRONG_SIDE,
    PARSE_WRONG_WHLDRCT,
    PARSE_WRONG_UINT_PARAM
} e_parse_state;

typedef enum {
    CMD_TYPE_UNKNOWN = -1,
    CMD_TYPE_GET,
    CMD_TYPE_SET,
    CMD_TYPE_INFO
} e_cmd_type;

/*
typedef enum {
    CMD_ENT_UNKNOWN = -1,
    CMD_ENT_NONE, // addressed to all system, contains 2 bytes (for example SNA - none|active)
    CMD_ENT_WHEEL // addressed to a wheel, contains 6 bytes (for example SWLU001 - wheel|left|up|001)
} e_cmd_ent;

typedef enum {
    CMD_SIDE_UNKNOWN = -1,
    CMD_SIDE_RIGHT,
    CMD_SIDE_LEFT
} e_cmd_side;

typedef enum {
    CMD_WHLDRCT_UNKNOWN = -1,
    CMD_WHLDRCT_UP,
    CMD_WHLDRCT_DOWN,
} e_cmd_whldrct;
*/

class xferInterf {
    const int maxLength = MAX_LENGTH;
    String message;
    e_parse_state parseState;
    e_cmd_type cmdType;
    char charParam[3];
    uint16_t uintParam;
public:
    xferInterf();

    e_parse_state ParseCommand(String usrString);
    e_parse_state ParseCommand(char * usrBytes, uint8_t length = MAX_LENGTH);
    e_parse_state GetLastParseState(void) const;
    e_cmd_type GetType(void) const;
    char GetCharParamByIndex(uint8_t usrIndex) const;
    uint8_t GetUintParam(void) const;
};