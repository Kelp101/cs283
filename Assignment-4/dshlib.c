#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include "dsh_cli.c"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

// Step 2 Implement cd
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0)
    {
        exit(0);
    }
    else if (strcmp(cmd->argv[0], "cd") == 0)
    {
        if (cmd->argc == 1)
        {
            return BI_EXECUTED;
        }
        if (chdir(cmd->argv[1]) != 0)
        {
            perror("cd failed");
        }
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

// Step 3 - Re-implement Your Main Loop and Parsing Code in exec_local_cmd_loop()
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (cmd_line == NULL || strlen(cmd_line) == 0)
    {
        return WARN_NO_CMDS;
    }
    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    while (*cmd_line == ' ') // trim spaces
    {
        cmd_line++;
    }

    char *end = cmd_line + strlen(cmd_line) - 1; // white space

    while (end > cmd_line && (*end == ' '))
    {
        *end = '\0';
        end--;
    }

    char *token = cmd_line;
    bool in_quotes = false;
    char *arg_start = NULL;

    while (*token) // quotes and quoted spaces
    {
        while (*token == ' ' && !in_quotes)
        {
            token++;
        }
        if (*token == '"')
        {
            in_quotes = !in_quotes;
            token++;
            arg_start = token;
        }
        else
        {
            arg_start = token;
        }
        while (*token && (in_quotes || (*token != ' ' && *token != '"')))
        {
            if (*token == '"' && in_quotes)
            {
                *token = '\0';
                in_quotes = false;
                break;
            }
            token++;
        }
        if (*token)
        {
            *token = '\0';
            token++;
        }
        if (cmd_buff->argc >= CMD_ARGV_MAX - 1)
        {
            return ERR_TOO_MANY_COMMANDS;
        }
        cmd_buff->argv[cmd_buff->argc++] = arg_start;
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP

    cmd_buff = malloc(ARG_MAX * sizeof(char));

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // IMPLEMENT THE REST OF THE REQUIREMENTS
        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
        if (strlen(cmd_buff) == 0)
        {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        }
        if (build_cmd_buff(cmd_buff, &cmd) != OK)
        {
            fprintf(stderr, "Error: Failed to parse command\n");
            continue;
        }
        if (cmd.argc == 0)
        {
            continue;
        }
        if (exec_built_in_cmd(&cmd) == BI_EXECUTED)
        {
            continue;
        }
        int f_result, c_result;
        f_result = fork();

        if (f_result < 0)
        {
            perror("fork");
            continue;
        }
        else if (f_result == 0)
        {
            execvp(cmd.argv[0], cmd.argv);
            perror("execvp");
            exit(ERR_EXEC_CMD);
        }
        else
        {
            wait(&c_result);
        }
    }
    free(cmd_buff);
    return OK;
}