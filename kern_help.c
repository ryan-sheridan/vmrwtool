#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>

int hex_to_int(char c);
int hex_to_ascii(char c, char d);
void print_string(const char* st);

mach_port_t return_ktp(int type) {
    // implements host_get_special_port(4) and task_for_pid(0) and if tfp fails it will try hgsp
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

int read_mem(uint64_t addr, uint64_t kslide, int task_port){
    kern_return_t kr;
    int sz = 8;
    int a = 0;
    unsigned char buffer[sz];

    addr = addr + kslide;

    kr = vm_read_overwrite(task_port,(vm_address_t)addr,sz,(vm_address_t)&buffer,(vm_size_t *)&sz);

    if (kr != KERN_SUCCESS) {
        printf("[ >>> ] failed to read! - %s\n",mach_error_string(kr));
        return -1;
    }

    for (int i = 0; i < sz; i+=8) {
        printf("0x%.08llx: %.02x %.02x %.02x %.02x %.02x %.02x %.02x %.02x",addr+i,buffer[a],buffer[a+1],buffer[a+2],buffer[a+3],buffer[a+4],buffer[a+5],buffer[a+6],buffer[a+7]); //@bellis1000
        a+=8;
    }

    const char * new_buffer = (const char *)buffer;
    print_string(new_buffer);

    return 0;
}

void read_multiple(int times, uint64_t addr, uint64_t kslide, int task_port){
    int i;

    for(i = 0; i < times; i++){

        if (read_mem(addr, kslide, task_port) != 0) {
            printf("[ >>> ] read_mem error, failed on %d\n", i);

            return;
        }

        addr += 16;
    }
}

int write_mem(uint64_t addr, uint64_t kslide, uint64_t data, int task_port){
    addr = addr + kslide;
    kern_return_t kr = vm_write(task_port,(vm_address_t)addr,(vm_address_t)&data,sizeof(data));

    if (kr != KERN_SUCCESS){
        printf("[ >>> ] write_mem failed [%s]\n", mach_error_string(kr));
        return 1;
    }

    read_multiple(16, addr, kslide, task_port);
    return 0;
}