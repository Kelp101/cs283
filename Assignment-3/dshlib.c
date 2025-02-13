#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if(cmd_line == NULL || strlen(cmd_line) == 0){
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));

    char *token = strtok(cmd_line, PIPE_STRING);
    while(token != NULL){
        if(clist->num >= CMD_MAX){
            return ERR_TOO_MANY_COMMANDS;
        }
        while(*token == ' '){
            token++;
        }
        
        char *end = token + strlen(token) - 1;          //white space
        
        while(end > token && (*end == ' ')){
            *end = '\0';
            end--;
        }
        
        char *arg_ptr = strchr(token, SPACE_CHAR);      //arguments

        if(arg_ptr){
            *arg_ptr = '\0';
            arg_ptr++;

            while (*arg_ptr == ' '){                    //trim spaces
                arg_ptr++;
            }
        }

        if(strlen(token) >= EXE_MAX || (arg_ptr && strlen(arg_ptr) >= ARG_MAX)){
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strcpy(clist->commands[clist->num].exe, token);

        if(arg_ptr){
            strcpy(clist->commands[clist->num].args, arg_ptr);
        }

        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }
    return OK;
}