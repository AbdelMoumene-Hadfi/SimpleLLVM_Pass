# SimpleLLVM_Pass
This pass is designed to simply inject a block(printf instruction) to start of main function .
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
opt -load-pass-plugin=./build/LLVMPassInjectBlock/libInjectBlockPass.so -passes="injectblock-pass" -S test.bc -o=test.ll
```
## Example :
* Source file :
```C
#include <stdio.h>
int main() {
	printf("test \n");
}
```
* IR without pass :
```
@.str = private unnamed_addr constant [7 x i8] c"test \0A\00", align 1

define dso_local i32 @main() #0 {
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str, i64 0, i64 0))
  ret i32 0
}
```
* IR with pass :
```
@.str = private unnamed_addr constant [7 x i8] c"test \0A\00", align 1
@0 = private unnamed_addr constant [16 x i8] c"Block Injected\0A\00", align 1

define dso_local i32 @main() #0 {
PrintBlock:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0))
  br label %1

1:                                                ; preds = %PrintBlock
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str, i64 0, i64 0))
  ret i32 0
}
```
* result :
```
Block Injected
test
```
