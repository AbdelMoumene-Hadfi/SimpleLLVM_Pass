# SimpleLLVM_Pass

LLVM Passes perform the transformations and optimizations that make up the compiler, they build the analysis results that are used by these transformations, and they are, above all, a structuring technique for compiler code.

 This pass is designed to simply print out the name of non-external functions that exist in the program being compiled. It does not modify the program at all, it just inspects it
## Build
```
mkdir build && cd build
cmake ../
make
```
## Test
1- generate bytecode from source file
```
clang -emit-llvm test.c -c -o test.bc
```
2- run the pass on it
```
opt -load-pass-plugin=./build/LLVMPassSample/libHelloNewPMPass.so -passes="hello-new-pm-pass" -S test.bc -o=test.ll
```
## Example :
* Source file :
```C
#include <stdio.h>
int foo() {
  return 0;
}
int bar() {
  return -1;
}
int main() {
  foo();
  bar();
}
```
* result :
```
Hello : foo
Hello : bar
Hello : main
```
