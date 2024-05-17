/*
Joseph Erlinger (jerling2@uoregon.edu)

This file loads a file of commands into a queue, then delegates each command to
its own subprocess.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <signal.h>
#include "MCP.h"
#include "color.h"
#include "terminal.h"


void handler(int num) {
    return;
}

/**
 * @brief Launch a pool of subprocesses to handle commands.
 * 
 * This function creates a n-processes given a queue of size n. The parent
 * process (aka the MCP) manages the queue so that each child process gets one
 * command. Each child process attempts to execute their assigned command. If
 * the child fails, then they release all of their resources and call exit.
 * 
 * @param[in,out] cmdqueue that contains n-commands. Note: this queue will be
 * consumed/emptied by this function.
 * @return procqueue that contains k-processes.
 */
queue *createpool (queue *cmdqueue)
{
    queue *procqueue;    // A queue to store processes in.
    cmd *command;        // The command that will be executed by a subprocess.
    pid_t *pid;          // The pid output of fork.
    int numcmds;         // The size of the cmdqueue.
    int i;               // The ith command.
    sigset_t sigset;
    int sig;
    
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    procqueue = newqueue();
    numcmds = cmdqueue->size;
    for (i = 0; i < numcmds; i++) {
        pid = (pid_t *)malloc(sizeof(pid_t));
        command = (cmd *)dequeue(cmdqueue);
        if (command->path == NULL) {       // Handle poor constructed commands.
            freecmd(command);
            continue;
        }
        if ((*pid = fork()) == -1) {                            // Fork process.
            criticalMsg("createpool: MCP unable to fork() process.");
            continue;                         // We don't want the MCP to exit.
        }
        if (*pid > 0) {                                            // MCP Logic.
            enqueue(procqueue, pid);
            freecmd(command);
            createMsg(*pid);
            continue;
        }
        // sleep(10);
        printf("no longer sleeping\n");
        sigwait(&sigset, &sig);
        if (execvp(command->path, command->argv) == -1) {       // Child logic.
            fprintf(stderr, "Could not execute '%s'. ", command->path);
            perror("execvp");
            freecmd(command);
            free(pid);
            freequeue(procqueue, free);
            return NULL;
        }
    }
    return procqueue;
}


/**
 * @usuage ./part1 <filename>
 */
int main (int argc, char *argv[]) 
{
    queue *cmdqueue = NULL;     // Queue of commands.
    queue *procqueue = NULL;    // Queue of running processes.
    int numchild;               // Number of children.
    int i;                      // The ith child.
    pid_t mcppid;               // Process ID of the Master Control Program.
    pid_t *pid;                 // A child's pid.
    node *cnode = NULL;         // Node representing the currrent node.
    
    struct sigaction sa;
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    if (argc != 2) {                                       // Input validation.
        fprintf(stderr, "Error: invalid usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((cmdqueue = readfile(argv[1])) == NULL) {
        goto cleanup;                            // Error: Could not read file.
    }
    if ((procqueue = createpool(cmdqueue)) == NULL) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }
    if (displayprocs(procqueue) == -1) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }
    
    infoMsg("MCP is waiting for x seconds.");
    mcppid = getpid();
    
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        setpgid(*pid, mcppid);
    }
    
    infoMsg("MCP is sending SIGUSR1 to all its children.");
    kill(0, SIGUSR1);

    infoMsg("MCP is now waiting.");
    numchild = procqueue->size;
    for (i = 0; i < numchild; i++) {
        pid_t child = wait(NULL);     // Wait for each child process to finish.
        terminateMsg(child);
    }

    cleanup:
    freequeue(cmdqueue, (void *)freecmd);
    freequeue(procqueue, free);
    exit(EXIT_SUCCESS);
}
