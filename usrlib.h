#ifndef USRLIB_H
#define USRLIB_H

/* Define format of messages */
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

/* Display messages only if debug is active */
#if DEBUG
#define info(...) pr_info(__VA_ARGS__)
#define warn(...) pr_warn(__VA_ARGS__)
#else
#define info(...)
#define warn(...)
#endif

/* Get the 1st arg of a `struct pt_regs` */
#define ARG1(regs, cast) (cast) regs->di
/* Get the 2nd arg of a `struct pt_regs` */
#define ARG2(regs, cast) (cast) regs->si
/* Get the 3rd arg of a `struct pt_regs` */
#define ARG3(regs, cast) (cast) regs->dx
/* Get the 4th arg of a `struct pt_regs` */
#define ARG4(regs, cast) (cast) regs->r10
/* Get the 5th arg of a `struct pt_regs` */
#define ARG5(regs, cast) (cast) regs->r8
/* Get the 6th arg of a `struct pt_regs` */
#define ARG6(regs, cast) (cast) regs->r9

#endif /* USRLIB_H */