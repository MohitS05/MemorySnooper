# MemorySnooper
A program to access a running process's memory without stopping it and printing it to a file.

The program takes input as : 
"./prog <Proc_id_to_snoop> <heap/stack/other_memread>"

The program prints the whole stack/heap of another process running in the memory.It is required to be run with root privilidges. It uses the `process_vm_readv()` to do this. It achieves so without halting the running program. 

It first gets memory address range from "/proc/{pid}/maps" and then used process_vm_readv() to read that range. It then puts the data in a file which is then converted to hex dump. This data can be used to debug or for learning purposes of how memory is assigned to a process and where what kind of variables are stored and allocated.
