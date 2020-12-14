# SimpleLLVM_Pass
This pass is designed to inspects Loop in a Function and count the number of sub-block .
## Build
```
mkdir build && cd build
cmake ../
make
```
## Test
1- generate bytecode from source file
```bash
clang -emit-llvm test.c -c -o test.bc
```
2- run the pass on it
```bash
opt -load-pass-plugin=./build/LLVMPassCountLoop/libCountLoopPass.so -passes="countloop-pass" -S test.bc -o=test.ll
```
## Example :
* Source file :
```C
#include <stdio.h>
int main() {
	int t=0;
	for(int i=0;i<10;i++) {
		for(int  j=0;j<6;j++)	{
			t++;
		}
	}
	for(int i=0;i<4;i++) {
		t++;
	}
	printf("%d\n",t);
}
```
* CFG :
![Flag](../img/CFG.png)
* result :
```
Function : main
LoopLevel : 0 Block : 7
LoopLevel : 1 Block : 3
LoopLevel : 0 Block : 3
```
