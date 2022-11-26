#include "transportProtocol.h"
#include <WString.h>

#define _CMD_TYPE_SET         'S'
#define _CMD_TYPE_GET         'G'
#define _CMD_TYPE_INFO        'I'

#define _CMD_ENT_TRANSMISSION 'T'
#define _CMD_ENT_WHEEL        'W'

#define _CMD_GENPARAM_LEFT    'L'
#define _CMD_GENPARAM_RIGHT   'R'
#define _CMD_GENPARAM_STATE   'S'

#define _CMD_AUXPARAM_UP      'U'
#define _CMD_AUXPARAM_DOWN    'D'

XferCommand::XferCommand() : 
    commandList(NULL),
    commandCount(0),
    defaultHandler(NULL),
    term('\n'),           // default terminator for commands, newline character
    last(NULL)
{
  strcpy(delim, " "); // strtok_r needs a null-terminated string
  ClearBuffer();
}

e_parse_state CommandVerification(char * line, uint8_t length) {
    e_parse_state checkState = PARSE_OK;

    if (length < COMMAND_WHOLE_LENGTH) {
        checkState = PARSE_TOO_SMALL;
        return checkState;
    }
    if (length > COMMAND_WHOLE_LENGTH) {
        checkState = PARSE_TOO_LONG;
        return checkState;
    }

    if (((line[COMMAND_NAME_LENGTH]     < '0') || (line[COMMAND_NAME_LENGTH]     > '9')) || 
        ((line[COMMAND_NAME_LENGTH + 1] < '0') || (line[COMMAND_NAME_LENGTH + 1] > '9')) || 
        ((line[COMMAND_NAME_LENGTH + 2] < '0') || (line[COMMAND_NAME_LENGTH + 2] > '9'))) 
    {
        checkState = PARSE_WRONG_UINT_PARAM;
        return checkState;
    }

    if (((line[0] < 'A') || (line[0] > 'Z')) || 
        ((line[1] < 'A') || (line[1] > 'Z')) || 
        ((line[2] < 'A') || (line[2] > 'Z')) || 
        ((line[3] < 'A') || (line[3] > 'Z'))) 
    {
        checkState = PARSE_WRONG_CHAR_PARAM;
        return checkState;
    }

    return checkState;
}

e_parse_state XferCommand::ParseCommand(char * usrBytes, uint8_t length) {
    /**
     * Command checking
     */
    parseState = CommandVerification(usrBytes, length);
    if (PARSE_OK != parseState)
        return parseState;

    /**
     * Command parsing
     */
    String tempUintParam;
    tempUintParam.getBytes((uint8_t *)usrBytes, 4, 3);
    data = (uint16_t)tempUintParam.toInt();

    char *command = strtok_r(buffer, delim, &last);   // Search for command at start of buffer
    if (command != NULL) {
#ifdef ARDUINO
        int j = strlen(command);			// force commands to be upper-case [DSH]
        for (int i = 0; i < j; i++) {
            if ((command[i] >= 'a') && (command[i] <= 'z'))
                command[i] &= -1 - ' ';
        }
#endif
        boolean matched = false;
        for (int i = 0; i < commandCount; i++) {
#ifdef XFERCOMMAND_DEBUG
            Serial.print("Comparing [");
            Serial.print(command);
            Serial.print("] to [");
            Serial.print(commandList[i].command);
            Serial.println("]");
#endif
            // Compare the found command against the list of known commands for a match
            if (strncmp(command, commandList[i].commandName, COMMAND_NAME_LENGTH) == 0) {
#ifdef XFERCOMMAND_DEBUG
                Serial.print("Matched Command: ");
                Serial.println(command);
#endif
                // Execute the stored handler function for the command
                (*commandList[i].function)(data);
                matched = true;
                break;
            }
        }
        if (!matched && (defaultHandler != NULL)) {
            (*defaultHandler)(command);
        }
    }
    ClearBuffer();

    return parseState;
}

void XferCommand::ClearBuffer(void) {
    buffer[0] = '\0';
    bufPos = 0;
}

e_parse_state XferCommand::GetLastParseState(void) const {
    return parseState;
}

void XferCommand::AddCommand(char * cmdName, void (* function)(uint16_t)) {
  
    int i, j, k;	// MODIFIED BY DSH TO FORCE COMMANDS TO UPPER CASE
    commandList = (t_commandCallback *) realloc(commandList, (commandCount + 1) * sizeof(t_commandCallback));
    char *cpNew = commandList[commandCount].commandName;
    j = strlen(cmdName);
    for (i = 0; i < j; i++) {
        k = *cmdName;
        if ((k >= 'a') && (k <= 'z')) {
            *cpNew++ = *cmdName & (-1 - ' ');
        }
        else {
            *cpNew++ = *cmdName;  
        }
        *cpNew = 0;    
        cmdName++;
    }

    commandList[commandCount].function = function;

    #ifdef XFERCOMMAND_DEBUG
    Serial.print("Adding command (");
    Serial.print(commandCount);
    Serial.print("): ");
    char *cpShow = commandList[commandCount].command;
    Serial.println(cpShow);
    #endif

    commandCount++;
}

void XferCommand::SetDefaultHandler(void (*function)(const char *)) {
  buffer[0] = '\0';
  bufPos = 0;
}

char *XferCommand::Next() {
  return strtok_r(NULL, delim, &last);
}