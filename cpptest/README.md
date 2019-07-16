## Guide to setting up an environment appropriate to build and run Linux kernel Unit Tests with CPPtest.
###### The current version is just a dump of my notes on bootstraping my own dev env

### This guide assumes:
* Cpptest 10.4 Professional is installed
* a git clone of the Linux kernel ( tag v4.12 tested for this guide )
* a git clone of Cpptest klibc fork ( this project )

### In Linux kernel project:
* `make menuconfig`
* `make headers_install`
* `cpptesttrace make`
    
 ### In klibc project:
 * `cp -r cpptest/* {path to Cpptest install}/10.4`
 * `ln -s {path to linux dir}/usr/include usr/include/linux`
 * `ln -s {path to linux dir}/usr/include/asm usr/include/asm`
 * `ln -s {path to linux dir}/usr/include/asm-generic usr/include/asm-generic`
 
 ### In CppTest GUI:
 * New project from CPPtest bdf ( use the bdf generated from cpptesttrace
 * Modify Project Properties -> Parasoft -> C/C++test -> Build Settings:
   * "Compiler options" append with `-isystem {path to klibc}/usr/include -isystem {path to klibc}/usr/include/arch/x86_64 -isystem {path to klibc}/usr/include/bits64`
   * "Linker options" append with `TBD`
