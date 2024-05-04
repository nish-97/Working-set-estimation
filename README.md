# CS695 Project

This project is built to estimate the working set size in a Simple KVM based virtualization design.

## How to run

Performing ```make``` or ```make run``` will compile the program and run the program only Long mode. Otherwise the VM can be run in specifically using the following commands:

````bash
make simple-kvm
./simple-kvm
./simple-kvm -l
````

Performing a ```make clean``` will remove the executable and the object files.

### VM configurations
The VM runs with a size of 1GB by default, having its page table and memory mappings done accordingly. The entries on the third and last levels of the page table are filled completely.

## Workloads tested for working set estimation
The guest.c code contains few synthetic workloads for which the working set has been estimated using the idea of number of pages dirtied by the VM. The dirty tracking is done by ioctl call using API **KVM_GET_DIRTY_LOG**. The workloads are:

````bash
mat mul of two int arrays of large size
mat mul of two unsigned long long arrays of large size
sequential access of memory pages
access to memory pages based on a random loop value 
````
These four setups are created both for a 1GB and 2MB VM, but by default only the 1GB VM runs. Inorder to get make the 2MB VM run, uncomment codes in guest.c and simple-kvm.c which are marked - setup for 2MB; and comment code for 1GB part.

## Output numbers
The output numbers showcase the total number of dirtied pages by the VM. There are two instances of this count, which are after two specific hypercalls(HC_numExitsByType) in the guest code(this could be changed as per user's wish). The first count shows the number of dirty pages when making the hypercall, which have been dirtied until now. The second count shows the number of dirty pages from the first hypercall up until the second hypercall. The workload code has been written accordingly in guest.c to generate dirty page counts.
