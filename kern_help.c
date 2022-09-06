#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>

mach_port_t return_ktp() {
    // implements host_get_special_port(4) and task_for_pid(0) and if hgsp fails it will try tfp
    // if both fail exit with -1

    mach_port_t kernel_task;
    kern_return_t kr;
    int pid;

    // try use hgsp4 before tfp0 as its supported on more ios 10 devices
    pid = 4;
    kr = host_get_special_port(mach_host_self(), HOST_LOCAL_NODE, pid, &kernel_task);

    // if hgsp4 doesnt return a KERN_SUCCESS try tfp0
    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] %d -> %x [ error code: %d ]\n", pid, kernel_task, kr);
        printf("[ >>> ] trying tpf0 ...\n");

        pid = 0;
        kr = task_for_pid(current_task(), pid, &kernel_task);

        // if tpf0 doesnt return KERN_SUCCESS either exit with error code -1 
        if (kr != KERN_SUCCESS) {
            printf("[ >>> ] %d -> %x [ error code: %d ]\n", pid, kernel_task, kr);
            printf("[ >>> ] tpf0 failed too, are you entitled?\n");
            return -1;
        }

        // otherwise return task port from tfp
        printf("[ >>> ] %d -> %x [%d]\n", pid, kernel_task, kr);
        return kernel_task;

    }
    // on KERN_SUCCESS of hgsp return task port
    printf("[ >>> ] %d -> %x [%d]\n", pid, kernel_task, kr);
    return kernel_task;
}

int has_aslr() {
    const struct mach_header *mach;
    mach = _dyld_get_image_header(0);
    
    if (mach->flags & MH_PIE) {
        return 0;
    } else {
        return -1;
    }
}

uint64_t ret_kslide(mach_port_t ktp) {
    // fuck i dont even know anymore
}






