#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "./include/kern_help.h"

mach_port_t tp;
uint64_t slide;
uint64_t addr; // = 0xFFFFFFF007755C40;
uint64_t data;
char bytes_char[8];
long bytes;
int pid = 0;
int hasaslr = 1;
bool kw = false;
bool multiple = false;
bool arg_handle_fail = false;

void print_help(){
    printf("usage:\n\t-w --write      [addr] [data] [pid]     write to an address in memory to pid\n\t-r --read       [addr] [bytes] [pid]    read an address in memory from pid\n\t-h --help       print this help.\n");
    return;
}

int main(int argc, char *argv[]) {
    char zero_x[2] = "0x";

    if (argc == 1) {
        print_help();
        return 1;
    }
    if ((strcmp(argv[1],"-w")==0) || (strcmp(argv[1],"--write")==0)) {
        if (argc != 4) {
            print_help();
        } else if (argc == 5) {
            kw = true;
            sscanf(argv[2], "%llx", &addr);
            sscanf(argv[3], "%llx", &data);
            sscanf(argv[4], "%d", &pid);
            return 0;
        } else {
            return 1;
        }
        
    }
    if ((strcmp(argv[1],"-r")==0) || (strcmp(argv[1],"--read")==0)) {
        if (argc == 5) {
            multiple = true;
            if(strlen(argv[3]) <= 8){
                strcpy(bytes_char, argv[3]);
                if(bytes_char[1] == zero_x[1]){
                    sscanf(argv[2], "%llx", &addr);
                    sscanf(argv[3], "%lx", &bytes);
                    sscanf(argv[4], "%d", &pid);
                } else {
                    sscanf(argv[2], "%llx", &addr);
                    sscanf(argv[3], "%ld", &bytes);
                    sscanf(argv[4], "%d", &pid);
                }    
            }
        } else {
            printf("hello world ?\n");
            return 1;
        }

    }
    if ((strcmp(argv[1],"-h")==0) || (strcmp(argv[1],"--help")==0)) {
        print_help();
        return 1;
    }

    tp = return_tp(pid);

    if ((tp) == -1) {
        printf("[ >>> ] failed [%s]\n", mach_error_string(tp));
    }

    if ((hasaslr = has_aslr()) == 1) { // check for kaslr (kernel address space layout randomization) if true, find slide and print
            if (pid != 0) {
                slide = ret_slide(tp, pid);
                printf("[ >>> ] found proc slide: 0x%llx\n", slide);
            } else {
                slide = ret_slide(tp, pid);
                printf("[ >>> ] found kernel slide: 0x%llx\n", slide);
            }
            if (slide == -1) {
                printf("[ >>> ] failed getting kernel slide: -1\n");
                return -1;
            }
            
    }

    addr = addr-0x1;

    if (kw == true) { // if kw == true
        if (write_mem(addr, slide, tp) != 0){
            printf("[ >>> ] back to main, write_mem failed.\n");
            return -1;
        }

        print_mem(addr, slide, tp);
        return 0; // x
    }

    if (multiple == true) {
        print_multiple(bytes, addr, slide, tp);
        return 0;
    }

    // kw and multiple reads is false, so must be read, right?????
    print_mem(addr, slide, tp);

    return 0;
}