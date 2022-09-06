#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <mach/mach.h> 
#include <mach/vm_map.h>
#include <mach-o/loader.h>

mach_port_t return_ktp();
int has_aslr();
uint64_t kern_slide();

int main(void) {
    mach_port_t ktp;
    uint64_t kslide;
    int hasaslr = 1;

    if (getuid() != 0) {
        printf("[ >>> ] notice: not running as root, continuing ...\n");
    }

    ktp = return_ktp();

    if ((ktp) == -1) {
        printf("[ >>> ] failed exit code -1\n");
        return -1;
    }

    printf("\n[ >>> ] stage 2, doing stuffs with task port\n");

    if ((hasaslr = has_aslr()) != -1) {
        printf("[ >>> ] task port %x has KASLR, ofcourse, finding slide ...\n", ktp);

        kslide = ret_kslide(ktp);

        if (kslide == -1) {
            printf("[ >>> ] failed getting kernel slide: -1\n");
            return -1;
        }

        printf("[ >>> ] found kernel slide: 0x%llu\n", kslide);
        return 0;
    }

    return 0;
}