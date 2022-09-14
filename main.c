#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

mach_port_t return_ktp();
int has_aslr();
uint64_t ret_kslide(mach_port_t ktp);

int read_mem(uint64_t addr, uint64_t kslide, int task_port);
int write_mem(uint64_t adrr, uint64_t kslide, int task_port);
void read_multiple(int times, uint64_t addr, uint64_t kslide, int task_port);

mach_port_t ktp;
uint64_t kslide = 0;
uint64_t addr; // = 0xFFFFFFF007755C40;
uint64_t data;
int hasaslr = 1;
bool kw = false;

void print_help(){
    printf("usage:\n\t-w --write [addr]   write to an address in kmemory\n\t-h --help   print this help.\n");
    return;
}

int main(int argc, char *argv[]) {

    if ((strcmp(argv[1],"-w")==0) || (strcmp(argv[1],"--write")==0)) {
        // kernel write is true, pass addr into addr and if the number of args is less than 4, print help and exit
        // otherwise pass data arg3 to data
        // note argv 1 2 and 3 == argc of 4 because argv 0 is the program name
        kw = true;
        sscanf(argv[2], "%llx", &addr);

        if (argc != 4) {
            print_help();
            return 1;
        }

        sscanf(argv[3], "%llx", &data);
    }

    if ((strcmp(argv[1],"-r")==0) || (strcmp(argv[1],"--read")==0)) {
        // if argv is read pass argv2 into addr instead, if argc != than 3 exit and print help.
        if (argc != 3) {
            print_help();
            return 1;
        }
        kw = false;
        sscanf(argv[2], "%llx", &addr);
    }

    if ((strcmp(argv[1],"-h")==0) || (strcmp(argv[1],"--help")==0)) {
        // self explanatory huh
        print_help();
        return 1;
    }

    if (getuid()) {
        // problems will probably occur not running as root, but who cares do what you want
        printf("[ >>> ] notice: not running as root, continuing ...\n");
    }

    // now we start the real stuff ...

    ktp = return_ktp(1); // return kernel task port to ktp

    if ((ktp) == -1) { // if failed to return ktp exit with -1
        printf("[ >>> ] failed exit code -1\n");
        return -1;
    }

    printf("\n[ >>> ] stage 2, doing stuffs with task port\n");

    if ((hasaslr = has_aslr()) == 1) { // check for kaslr (kernel address space layout randomization) if true, find kslide and print
        printf("[ >>> ] task port %x has KASLR, ofcourse, finding slide ...\n", ktp);

        kslide = ret_kslide(ktp);

        if (kslide == -1) {
            printf("[ >>> ] failed getting kernel slide: -1\n");
            return -1;
        }

        printf("[ >>> ] found kernel slide: 0x%llx\n", kslide);
    }

    if (kw == true) { // if kw == true
        if (write_mem(addr, kslide, ktp) != 0){
            printf("[ >>> ] back to main, write_mem failed.\n");
            return -1;
        }

        read_mem(addr, kslide, ktp);
        return 0; // success
    }

    // kw is false, so must be read, right?????
    read_mem(addr, kslide, ktp);

    return 0;
}





