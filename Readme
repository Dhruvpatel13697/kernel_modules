# Kernel Modules Project

This repository contains multiple Linux kernel modules for different functionalities. Below is a brief description of each module:

## Modules
1. **hello.c** - A simple "Hello, World!" kernel module.
2. **k_prob.c** - A kernel probe module used for debugging system calls.
3. **sct.c** - A module that hooks the `kill` system call and replaces it with `newkill`.

## Prerequisites
- Ensure you have installed all required **kernel headers**.
- Kernel headers path: `/usr/include` (cannot use user-space headers in kernel mode).

## Installation Steps

### Step 1: Install Kernel Headers
```sh
sudo apt-get install linux-headers-$(uname -r)
```

### Step 2: Compile the Modules
```sh
make
```

### Step 3: Insert a Module into the Kernel
```sh
sudo insmod file_name.ko
```
`.ko` files are kernel object files generated after compilation.

### Step 4: Verify Loaded Modules
```sh
lsmod | grep file_name
```
This lists all loaded modules. The module details can also be found in `/proc/modules`.

### Step 5: Check Kernel Logs
```sh
sudo dmesg
```
This helps in debugging and verifying module initialization messages.

### Step 6: Remove a Module from the Kernel
```sh
sudo rmmod file_name
```

## Note
If **Secure Boot** is enabled on your machine, you may need to disable it to run these modules without signing.

