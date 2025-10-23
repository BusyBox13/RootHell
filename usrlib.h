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

#endif /* USRLIB_H */