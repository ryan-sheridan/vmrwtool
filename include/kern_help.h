#include <mach/mach.h>

mach_port_t return_tp();
int has_aslr();
uint64_t ret_slide(mach_port_t ktp, int pid);

int print_mem(uint64_t addr, uint64_t kslide, int task_port);
int write_mem(uint64_t adrr, uint64_t kslide, int task_port);
void print_multiple(int times, uint64_t addr, uint64_t kslide, int task_port);