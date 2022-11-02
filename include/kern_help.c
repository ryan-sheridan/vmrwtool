#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>

#define ANSI_COLOR1 "\33[38;5;0;48;5;255m"
#define ANSI_COLOR2 "\33[38;5;255;48;5;0m"
#define ANSI_RESET  "\33[m"

mach_port_t return_ktp(int type) {
    task_t kernel_task;
    kern_return_t kr;
    int pid = 26378;

    kr = task_for_pid(mach_task_self(), pid, &kernel_task);

    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] %d -> %x [%d]\n", pid, kernel_task, kr);
        return -1;
    }

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

// uint64_t ret_slide(int pid){
//     mach_vm_address_t main_address;
//     if(find_main_binary(pid, &main_address) != KERN_SUCCESS) {
//         printf("Failed to find address of header!\n");
//         return -1;
//     }
// 
//     uint64_t aslr_slide;
//     if(get_image_size(main_address, pid, &aslr_slide) == -1) {
//         printf("Failed to find ASLR slide!\n");
//         return -1;
//     }
//     return aslr_slide;
// }

int print_mem(uint64_t addr, uint64_t kslide, int task_port){
    kern_return_t kr;
    int sz = 16;
    unsigned char data[sz];

    addr = addr + kslide;

    kr = vm_read_overwrite(task_port,(vm_address_t)addr,sz,(vm_address_t)&data,(vm_size_t *)&sz);

    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] failed to read! - %s\n",mach_error_string(kr));
        return -1;
    }

    for (int i=0; i < 16; i++) {
        if (i == 0) {
            printf("0x%.08llx:", addr);
        } else {
            if (data[i] == 0) { // if nullbyte print 00 inverted
                printf(" " ANSI_COLOR1 "00" ANSI_RESET);
            } else {
                printf(" %.02x", data[i]);
            }
            if (i == 15) {
                // print ascii.
                for (int j=0; j < 16; j++) {
                    if (j == 0) {
                        printf(" | ");
                    } else {
                        if (data[j] < 32) {
                            printf(ANSI_COLOR1 " " ANSI_RESET);
                        } else {
                            printf("%c", data[j]);
                        }
                        if (j == 15) {
                            printf("\n");
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void print_multiple(int times, uint64_t addr, uint64_t kslide, int task_port){
    int i;

    for(i = 0; i < times; i++){
        int rmr = print_mem(addr, kslide, task_port);
        if (rmr != 0) {
            printf("[ >>> ] print_mem error, failed on %d\n", i);
            return;
        }
        addr += 0x16;
    }
}

int write_mem(uint64_t addr, uint64_t kslide, uint64_t data, int task_port){
    addr = addr + kslide;
    kern_return_t kr = vm_write(task_port,(vm_address_t)addr,(vm_address_t)&data,sizeof(data));

    if (kr != KERN_SUCCESS){
        printf("[ >>> ] write_mem failed [%s]\n", mach_error_string(kr));
        return 1;
    }
    print_multiple(0x16, addr, kslide, task_port);
    return 0;
}