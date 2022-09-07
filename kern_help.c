#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>

mach_port_t return_ktp(int type) {
    // implements host_get_special_port(4) and task_for_pid(0) and if hgsp fails it will try tfp
    // if both fail exit with -1

    task_t kernel_task;
    kern_return_t kr;
    int pid;

    // try use tfp0 before hgsp4 as its supported on more ios 10 devices
    pid = 0;
    kr = task_for_pid(mach_task_self(), pid, &kernel_task);

    // if tfp doesnt return a KERN_SUCCESS try hgsp4
    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] %d -> %x [ error code: %d ]\n", pid, kernel_task, kr);
        printf("[ >>> ] trying hgsp4 ...\n");

        pid = 4;
        kr = host_get_special_port(mach_task_self(), HOST_LOCAL_NODE, pid, &kernel_task);

        // if tpf0 doesnt return KERN_SUCCESS either exit with error code -1 
        if (kr != KERN_SUCCESS) {
            printf("[ >>> ] %d -> %x [ error code: %d ]\n", pid, kernel_task, kr);
            printf("[ >>> ] hgsp4 failed too, are you entitled?\n");
            return -1;
        }

        // otherwise return task port from tfp
        printf("[ >>> ] %d -> %x [%d]\n", pid, kernel_task, kr);
        return kernel_task;

    }
    // on KERN_SUCCESS of tfp return task port
    printf("[ >>> ] %d -> %x [%d]\n", pid, kernel_task, kr);
    return kernel_task;
}

int has_aslr() {
    const struct mach_header *mach;
    mach = _dyld_get_image_header(0);
    
    if (mach->flags & MH_PIE) {
        return 1;
    } else {
        return -1;
    }
}

uint64_t ret_kslide(mach_port_t ktp) {
    kern_return_t task_i;
    task_dyld_info_data_t info;
    uint32_t count = TASK_DYLD_INFO_COUNT;

    task_i = task_info(ktp, TASK_DYLD_INFO, (task_info_t)&info, &count);

    if(task_i != KERN_SUCCESS) {
        return 0;
    }

    return info.all_image_info_size;
}






