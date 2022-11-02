#include <mach/mach.h>

mach_port_t return_ktp();
int has_aslr();
uint64_t ret_kslide(mach_port_t ktp);
uint64_t ret_slide(int pid);

int print_mem(uint64_t addr, uint64_t kslide, int task_port);
int write_mem(uint64_t adrr, uint64_t kslide, int task_port);
void print_multiple(int times, uint64_t addr, uint64_t kslide, int task_port);