# charon
A fileless malware loader.

Charon is a minimal fileless malware loader that downloads payloads over TCP and executes instructions in memory, thwarting EDRs and related disk-based detection/forensic techniques. Once a TCP connection is established, the Acheron server sends the size of the code in a 64 bit unsigned long. Charon allocates a buffer based on the supplied size and sends back the size in order to start the payload transfer. After the payload is written to the buffer, Charon changes the memory permissions of the buffer, recasts the buffer pointer to a function pointer and calls the function.

A number of improvements could be made such as:
1. Implementing the program in assembly in order to avoid the overhead that C compilers introduce.
2. Calling syscalls directly instead of calling kernel32 functions in order to avoid hooks setup by modern EDRs.
3. A cooler name.

Shellcode is created by handwritting assembly and assembling with [NASM](https://www.nasm.us/) using the ```bin``` format.
