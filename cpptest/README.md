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
 #### Linux project setup:
 * New project from CPPtest bdf ( use the bdf generated from cpptesttrace )
 * Modify Project Properties -> Parasoft -> C/C++test -> Build Settings:
   * "Compiler options" append with `-isystem {path to klibc}/usr/include -isystem {path to klibc}/usr/include/arch/x86_64 -isystem {path to klibc}/usr/include/bits64`
   * "Linker options" append with `TBD`
 #### Make user defined version of, File Scoped, Test Configuration for Stub Generation:
 * In the top navigation bar click on Parasoft -> Test Configurations...
  * In the left panel navigate to the item: Builtin -> Unit Testing -> File Scope -> Generate Stubs (File Scope)
  * Right click on Generate Stubs (File Scope) -> Duplicate
  * The duplacte will reside in User-defined -> Unit Testing -> File Scope -> {name of new test configuration}
  * click on the new Test Configuration:
    * Find "Executaion" tab -> "Symbols" tab -> "Use extra symbols from file found in:" dialog box.
    * Remove the string "${cpptest:cfg_dir}/safestubs;"
 #### Get started Unit Testing a source file:
 * In Navigator right click on a source file and drill down to: Parasoft -> Test Using -> Builtin -> Unit Testing -> Generate Test Suite
 * In Test Case Explorer right click on TestSuite_{source file name} and drill down to: Add New -> Test Case Using Editor...
 * Open the new test case in the editor.
  * Pick a function from the "Create test case for" dropdown menu.
  * populate the testcase by pressing the green arrow to the right of your function selection.
  * Save the test case.
* In the Test Case explorer right click on the new Test Case and drill down to: Test Using -> User Defined -> Unit Testing -> File Scoped -> {name of new test configuration}
   
