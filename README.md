# RootHell - Linux Kernel Module Rootkit
RootHell is a Linux Kernel Module (LKM) rootkit developped to learn how rootkits works and how Linux kernel runs. 

<ins>__Important Disclaimer__</ins>: This project is developped for educational and academic purposes. The software is provided "as is" and the author is not responsible for any damage or mishaps that may occur during its use. Do not attempt tu use it to violate the law. Misuse of the provided software and informaton may result in criminal charges

## Summary
- [Compilation](#compilation)
- [Usage](#usage)
- [Functionnalities](#functionnalities)
- [Credits](#credits)


## Compilation
Install the kernel headers. 
Using apt: 
`apt install linux-headers-$(uname -r)`
using yum:
`yum install kernel-headers`
or using pacman: 
`pacman -S linux-headers`

NOTE: will be implemented after, now only available for very specific env.
<!-- Once done, clone the project and use make: 
```sh
git clone https://github.com/BusyBox13/RootHell.git
cd RootHell
make
``` -->

This gives you a ko kernelobject file (main.ko). It must be loaded using `insmod main.ko`.
To remove it, use `rmmod main.ko`. Beware that module must not be hidden. 

## Usage

There're 6 major files 

### main.c
It contains the main component, with the init and exit function to create and destroy our module. 

`c static int __init hook_init(void)` contains the code to initiate the module and add defined hooks in `hooks.h`. 
`c static void __exit hook_exit(void)` will restore the hooked functions and unload module. 

### hooks.h 
It's the main point for all hooks, that are all declared inside the `struct ftrace_hook hooked[]` object. 
Every hooks defined in this object will be applied when module launched. 

### ftrace_helper.h
Is a file used to simplify the different hooking using ftrace in different version of kernel. 
It contains the definition of `HOOK` and `HOOK_NOSYS`. 

`HOOK` is used to define a hook to a syscall (e.g: sys_kill). 
`HOOK_NOSYS` is used to hook others calls (e.g: urandom_read to manipulate the output of the file `/dev/urandom`)

### commands.h
Contains all the constants used to communicate between the module and the user. 
As an exemple, using `kill -42 666` will trigger the hook (signal = 42 used to verify) and 666 is defined as command to (un)hide ourself. 

### usrlib.h 
An interface to recreate user functions to simplify interactions or programming. 

## Functionnalities 

## Credits

