/*
Author: Joseph Erlinger (jerling2@uoregon.edu)
*/
#ifndef TERMINAL_H
#define TERMINAL_H
#include <unistd.h>
#include "MCP.h"

typedef enum Terminal {
    XFCE4,
    GNOME,
    NOTERM = -1
} Terminal;

#define XFCE4_PATH "/usr/bin/xfce4-terminal"
#define GNOME_PATH "/usr/bin/gnome-terminal"
#define XFCE4_COMMAND "--command=./topscript.sh"
#define GNOME_COMMAND "./topscript.sh"
#define SUPPORTED_TERMS 2
#define TOPSCRIPT "./topscript.sh"

extern char terminals[SUPPORTED_TERMS][25];

Terminal whichterm ();

int displayprocs (queue *procqueue);

int openterm (Terminal type);

void createtopscript (queue *procqueue);

#endif