#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Signal code used to trigger specific function
 */
enum sig
{
    SIG_SHOW_MYSELF = 42, /* Show/Hide this module */
    SIG_HIDE_PROC,
    SIG_SET_USER
} sig;

/**
 * Code to protect command (as password)
 * Or to do multiple actions
 */
enum code
{
    SHOW_MYSELF = 666 /* Show/Hide this module */
} code;

#define PREFIX "this."

#endif