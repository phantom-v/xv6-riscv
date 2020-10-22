#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    Log("Boot start");


    kinit();         // physical page allocator
    Log("After kinit");
    kvminit();       // create kernel page table
    Log("After kvminit");
    kvminithart();   // turn on paging
    Log("After kvminithart");
    procinit();      // process table
    Log("After procinit");
    trapinit();      // trap vectors
    Log("After trapinit");
    trapinithart();  // install kernel trap vector
    Log("After trapinithart");
    plicinit();      // set up interrupt controller
    Log("After plicinit");
    plicinithart();  // ask PLIC for device interrupts
    Log("After plicinithart");
    binit();         // buffer cache
    Log("After binit");
    iinit();         // inode cache
    Log("After iinit");
    fileinit();      // file table
    Log("After fileinit");

    // virtio_disk_init(); // emulated hard disk
    ramdisk_init();
    Log("After ramdisk_init");

    userinit();      // first user process
    Log("After userinit");

    __sync_synchronize();
    Log("After __sync_synchronize");

    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
