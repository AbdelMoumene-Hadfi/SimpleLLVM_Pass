# SimpleLLVM_Pass
LLVM Passes perform the transformations and optimizations that make up the compiler, they build the analysis results that are used by these transformations, and they are, above all, a structuring technique for compiler code.

## Exapmle
 * [HelloPass](./HelloPass)
This pass is designed to simply print out the name of non-external functions that exist in the program being compiled. It does not modify the program at all, it just inspects it and count the number of times that each opcode appears in a given function .
 * [CountLoop](./CountLoop)
This pass is designed to inspects Loop in a Function and count the number of sub-block .
 * [InjectPass](./InjectPass)
This pass is designed to simply inject a block(printf instruction) to start of main function .
