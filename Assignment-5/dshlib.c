#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

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

// Step 2 - Re-implement Your Main Loop and Parsing Code in exec_local_cmd_loop() to parse multiple commands by pipe
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0)
    {
        printf("exiting...\n");
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

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || strlen(cmd_line) == 0)
    {
        return WARN_NO_CMDS;
    }
    memset(clist, 0, sizeof(command_list_t));

    char *token = strtok(cmd_line, PIPE_STRING);
    while (token && clist->num < CMD_MAX)
    {
        build_cmd_buff(token, &clist->commands[clist->num]);
        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }

    if (token)
    {
        return ERR_TOO_MANY_COMMANDS;
    }
    return OK;
}

// 5-pipe-handling demo code
int execute_pipeline(command_list_t *clist)
{
    int pipes[CMD_MAX - 1][2];
    pid_t pids[CMD_MAX];
    int i;

    for (i = 0; i < clist->num - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }
    for (i = 0; i < clist->num; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork");
            return ERR_EXEC_CMD;
        }
        else if (pids[i] == 0)
        {
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < clist->num - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < clist->num - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(ERR_EXEC_CMD);
        }
    }

    for (i = 0; i < clist->num - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (i = 0; i < clist->num; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
    return OK;
}

int exec_local_cmd_loop()
{
    char *cmd_buff;
    command_list_t clist;

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

        if (build_cmd_list(cmd_buff, &clist) != OK)
        {
            fprintf(stderr, "Error: Failed to parse command\n");
            continue;
        }
        if (clist.num == 0)
        {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        }
        if (clist.num == 1 && exec_built_in_cmd(&clist.commands[0]) == BI_EXECUTED)
        {
            continue;
        }
        execute_pipeline(&clist);
    }
    free(cmd_buff);
    return OK;
}