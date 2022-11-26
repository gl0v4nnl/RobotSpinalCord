#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * The table of commands
 * 
 * +---------+---------+---------+---------+---------+---------+---------+
 * |  BYTE6  |  BYTE5  |  BYTE4  |  BYTE3  |  BYTE2  |  BYTE1  |  BYTE0  |
 * +---------+---------+---------+---------+---------+---------+---------+
 * | COMMAND | ENTITY  |GEN.PARAM|AUX.PARAM|  DATA2  |  DATA1  |  DATA0  |
 * +---------+---------+---------+---------+---------+---------+---------+
 * |               CHARFIELD               |          INTFIELD           |
 * +---------------------------------------+-----------------------------+
 * 
 * +----+----------------------------------------------------+
 * |--- COMMANDS ---                                         |
 * |S - SET  - set some entity to needed state               |
 * |G - GET  - get state of needed entity, this is a request |                              
 * |I - INFO - command, addressed from Mega to RPI           |
 * +---------------------------------------------------------+
 * |--- ENTITIES ---                                         |
 * |T - TRANSMISSION - global command                        |
 * |W - WHEEL - addressed to wheel (left or right)           |
 * +---------------------------------------------------------+
 * |--- GENERAL PARAMETERS ---                               |
 * |R - RIGHT                                                |
 * |L - LEFT                                                 |
 * +---------------------------------------------------------+
 * |--- AUXILARY PARAMETERS ---                              |
 * |U - UP                                                   |
 * |D - DOWN                                                 |
 * +---------------------------------------------------------+
 * |--- DATA ---                                             |
 * |Numeric data from 0 to 999                               |
 * +---------------------------------------------------------+
 * 
 */

#define COMMAND_WHOLE_LENGTH 7
#define COMMAND_DATA_LENGTH 3
#define COMMAND_NAME_LENGTH (COMMAND_WHOLE_LENGTH - COMMAND_DATA_LENGTH)
#define COMMAND_COUNT 20

typedef enum {
    PARSE_OK = 0,
    PARSE_UNKNOWN = -10,
    PARSE_TOO_SMALL,
    PARSE_TOO_LONG,
    PARSE_WRONG_TYPE,
    PARSE_WRONG_ENTITY,
    PARSE_WRONG_GENPARAM,
    PARSE_WRONG_AUXPARAM,
    PARSE_WRONG_CHAR_PARAM,
    PARSE_WRONG_UINT_PARAM
} e_parse_state;


class XferCommand {

    typedef struct {
        char commandName[COMMAND_NAME_LENGTH];
        void (* function)(uint16_t);
    } t_commandCallback;

    t_commandCallback * commandList;
    uint8_t commandCount;    

    e_parse_state parseState;
    uint16_t data;

    // Pointer to the default handler function
    void (*defaultHandler)(const char *);

    char delim[2]; // null-terminated list of character to be used as delimeters for tokenizing (default " ")
    char term;     // Character that signals end of command (default '\n')

    char buffer[COMMAND_WHOLE_LENGTH]; // Buffer of stored characters while waiting for terminator character
    uint8_t bufPos;                        // Current position in the buffer
    char *last;                         // State variable used by strtok_r during processing
    
public:
    XferCommand();

    void AddCommand(char * cmdName, void (* function)(uint16_t));
    void SetDefaultHandler(void (*function)(const char *));   // A handler to call when no valid command received.
    void ClearBuffer();   // Clears the input buffer.
    char *Next(void);

    e_parse_state  ParseCommand(char * usrBytes, uint8_t length = COMMAND_WHOLE_LENGTH);
    e_parse_state  GetLastParseState(void) const;
};