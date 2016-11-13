# BadgerTrap
BadgerTrap is a tool to instrument x86-64 TLB misses. 

## About
It converts TLB misses to reserved page faults by using the reserved bit in a page table entry. Using this tool can help guide memory management unit (MMU) research for x86-64 machines. In the current form, it counts the number of TLB misses, but can be used to generate traces of TLB misses or can be used to perform any study on TLB misses by instrumenting TLB misses in the Linux kernel. 

## Using BadgerTrap
Currently this tool has been designed just to count the number of Data TLB misses. But can be applied to perform many other studies. Some example uses of BadgerTrap can be found in these two research papers listed below. Paper by Basu et al. used an earlier version of the code from which BadgerTrap evolved. Some other application of BadgerTrap can be found in the second paper, but they came up with their own implementation in parallel.

1. Arkaprava Basu, Jayneel Gandhi, Jichuan Chang, Mark D. Hill, and Michael M. Swift. 2013. [Efficient virtual memory for big memory servers](http://dl.acm.org/citation.cfm?id=2485943). In Proceedings of the 40th Annual International Symposium on Computer Architecture (ISCA '13). ACM, New York, NY, USA, 237-248.
2. Abhishek Bhattacharjee. 2013. [Large-reach memory management unit caches](http://dl.acm.org/citation.cfm?id=2540741). In Proceedings of the 46th Annual IEEE/ACM International Symposium on Microarchitecture (MICRO-46). ACM, New York, NY, USA, 383-394.

For any useful utility of the this tool you will have to change various fake_fault functions. Right now we are just counting the number of TLB misses, but you can perform your own task in those functions. There are 3 fake_fault functions:

1. For base 4KB pages: do_fake_page_fault in mm/memory.c file.
2. For base 2MB pages with transparent hugepages: transparent_fake_fault in mm/memory.c file.
3. For base 1GB/2MB pages with hugetlbfs: hugetlb_fake_fault in mm/hugetlb.c file.

You can instrument what to perform when the TLB miss happen in these functions.

## Requirements
1. Linux Kernel 3.12.13
2. x86-64 machine

## Repository Contents
1. Patch for Linux Kernel v3.12.13
2. A helper C program to setup and run badger trap
3. LICENSE and README files with COPYRIGHT information

## Installation

- Clone the Linux kernel tree.
```
git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
```
- Revert to the git tree to kernel version 3.12.13.
```
cd linux-stable
git checkout -b v3.12.13
git checkout -b local v3.12.13
git branch
```
- Download the package from the link.
```
wget http://research.cs.wisc.edu/multifacet/badger-trap/BadgerTrap_beta_v1.0.tar.gz
```
- Uncompress the package.
```
tar -xvzf BadgerTrap_beta_v1.0.tar.gz
```

- Apply the patch on the kernel
```
cd linux-stable
git apply --whitespace=nowarn ../BadgerTrap_beta_v1.0/badger_trap.patch
```

- Compile the kernel and install kernel with BadgerTrap.
```
make -j 32; make modules_install; make install
```
Reboot the machine. While rebooting check that it reboots with the kernel you installed (3.12.13+)

- There is an utility C-program provided with the package that helps you easily utilize BadgerTrap easily. Go to the directory BadgerTrap_beta_v1.0 in which you untared the package and look for badger-trap.c file.

- Compile the code.
```
gcc -o badger-trap badger-trap.c
```
- To run BadgerTrap, there are 4 options. Run BadgerTrap for the whole program runtime.
  - Attach BadgerTrap to a command.
  ```
  ./badger-trap command <command to run>
  ./badger-trap command ls
  ./badger-trap command ./a.out {arguments}
  ```
  - Attach BadgerTrap to a list of currently running processes.
  ```
  ./badger-trap pid <list of pids>
  ./badger-trap pid 123 4128 8096
  ./badger-trap pid 2468
  ```
  - Mark processes before launching to run with BadgerTrap.
  ```
  ./badger-trap name <list of process names>
  ./badger-trap name ls omp-csr blacksholes
  ./badger-trap name ls
  ```
  These processes will always run with BadgerTrap when launched.
  You can clear the process names by running 
  ```
  ./badger-trap name
  ```
  - There is one more way of running BadgerTrap. This requires changes to the program. This is helpful when you want to skip the initialization phase of the program and then start tracking TLB misses. You can introduce this line in your code.
   ```
   syscall(314,NULL,0,0)
   ```
  For more information on the syscall, please look at mm/badger-trap.c in linux source. 

- All output of BadgerTrap will be printed in kernel log as of now. You can either use journalctl -k or dmesg as per your system. You can change that if needed. You can change the behavior by writing to another file, change the printing behavior in kernel/exit.c. In case you want to print some other information, you can print it accordingly. 

## Features Supported
- 4KB page size
- 2MB page size with transparent hugepages
- 1GB/2MB page sizes with hugetlbfs
- Multiprogrammed Workloads
- Multiprocess Workloads
- Multithreaded Workloads
- Copy-on-write pages
- Start TLB tracking after warm-up in a workload
- Attach BadgerTrap to any running process
- Follow forked/launched sub-processes

## Features Not Supported
This is not a complete list. We may add support to these later.
- NUMA
- Kernel Samepage Merging
- Instruction TLB misses
- Kernel TLB misses

## Caveats
This software has only been tested with Linux Kernel v3.12.13. This patch may need to be tweaked accordingly if you change to the next/older kernel version. The steps provided above are generic but are only tested on a Ubuntu and Fedora based operating systems. We do not take any responsibility in case, this leads to crash of the system or loss of data. We hope you have some experience with Kernel Development before you use this software.

## Citations
This citation is particularly for this tool. This paper describes the mechanism behind the tool. If you are using this tool for your research or for any other reason, you can cite. 

Jayneel Gandhi, Arkaprava Basu, Mark D. Hill, and Michael M. Swift. 2014. [BadgerTrap: A Tool to Instrument x86-64 TLB Misses](http://www.cs.wisc.edu/multifacet/papers/can14_badgertrap.pdf). SIGARCH Computer Architecture News (CAN). 

## Authors
Jayneel Gandhi, Arkaprava Basu, Mark D. Hill, Michael M. Swift
Wisconsin Multifacet Project, University of Wisconsin-Madison

## Acknowledgements
We thank Christopher Feilbach, Sujith Surendran, Vasilis Karakostas and Your Name for their feedback on the tool. This work is supported in part by the National Science Foundation (CNS-0720565,CNS-0834473, CNS-0916725, CNS-1117280, CCF-1218323, and CNS-1302260 ), Google, and the University of Wisconsin (Kellett award and Named professorship to Hill).

## Copyright
Copyright of this software is held by Mark D. Hill, Michael M. Swift and David A. Wood from University of Wisconsin - Madison. All Rights Reserved.

## License
Since this tool is built on top of Linux Kernel, this software is licensed under GNU GPL v2. You can read more about the license [here](http://www.gnu.org/licenses/gpl-2.0.html). Read all the terms of the license before using this tool.
