#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>

#define ANSI_COLOR1 "\33[38;5;0;48;5;255m"
#define ANSI_COLOR2 "\33[38;5;255;48;5;0m"
#define ANSI_RESET  "\33[m"

mach_port_t return_tp(int pid) {
    task_t task;
    kern_return_t kr;

    kr = task_for_pid(mach_task_self(), pid, &task);

    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] %d -> %x [%d]\n", pid, task, kr);
        return -1;
    }

    printf("[ >>> ] %d -> %x [%d]\n", pid, task, kr);
    return task;
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

uint64_t ret_slide(mach_port_t tp, int pid) {
    kern_return_t task_i;
    task_dyld_info_data_t info;
    uint32_t count = TASK_DYLD_INFO_COUNT;

    task_i = task_info(tp, TASK_DYLD_INFO, (task_info_t)&info, &count);

    if(task_i != KERN_SUCCESS) {
        return 0;
    }

    return info.all_image_info_size;
}

int print_mem(uint64_t addr, uint64_t slide, int task_port){
    kern_return_t kr;
    int sz = 16;
    unsigned char data[sz];

    addr = addr + slide;

    kr = vm_read_overwrite(task_port,(vm_address_t)addr,sz,(vm_address_t)&data,(vm_size_t *)&sz);

    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] failed to read! - %s\n",mach_error_string(kr));
        return 0;
    }

    for (int i=0; i < 16; i++) {
        if (i == 0) {
            printf("%.08llx ", addr+0x1);
        } else {
            if (i == 8){
                printf(" ");
            }
            if (data[i] == 0) { // if nullbyte print 00 inverted
                printf(" " ANSI_COLOR1 "00" ANSI_RESET);
            } else {
                printf(" %.02x", data[i]);
            }
            if (i == 15) {
                // print ascii.
                for (int j=0; j < 16; j++) {
                    if (j == 0) {
                        printf(" |");
                    } else {
                        if (data[j] <= 32) {
                            printf(".");
                        } else if (data[j] < 127) {
                            printf("%c", data[j]);
                        } else {
                            printf(".");
                        }
                        if (j == 15) {
                            printf("|\n");
                        }
                    }
                }
            }
        }
    }

    return 1;
}

void print_multiple(int bytes, uint64_t addr, uint64_t slide, int task_port){
    int i;
    int ret;

    if(bytes<=16 || bytes == 0){
        bytes=1;
    } else {
        bytes=bytes/16;
    }

    for(i = 0; i < bytes; i++){
        ret = print_mem(addr, slide, task_port);
        if(!ret){
            printf("failed in print_mem()\n");
        }
        addr+=bytes;
    }

}

int write_mem(uint64_t addr, uint64_t slide, uint64_t data, int task_port){
    /*
        a project in the works, soon, this wont work with newer devices that want
        to write to kernel memory, A10 and up, because of ktrr, but anything 
        below should work with tpf0 in theory, i think, but just writing to normal
        processes it should be fine.
    */
    return 1;
}