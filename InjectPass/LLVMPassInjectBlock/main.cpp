#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
using namespace llvm;

namespace {
  struct InjectBlockPass : public PassInfoMixin<InjectBlockPass> {
    void injectBlock(Module &M,Function *F) {
      auto &Ctx = F->getContext();
      BasicBlock &EntryBlock = F->getEntryBlock();
      BasicBlock *PrintBlock = BasicBlock::Create(Ctx,"PrintBlock",EntryBlock.getParent(),&EntryBlock);
      Function *printfFunction = M.getFunction("printf");

      IRBuilder<> Builder(PrintBlock);
      std::vector<Value *> printArgs;
      Value *FormatStr = Builder.CreateGlobalStringPtr("Block Injected\n");
      printArgs.push_back(FormatStr);
      Builder.CreateCall(printfFunction,printArgs);
      Builder.CreateBr(&EntryBlock);
    }
    PreservedAnalyses run(Module &M,ModuleAnalysisManager &MAM) {
      Function *MainFunction = M.getFunction("main");
      injectBlock(M,MainFunction);
      return PreservedAnalyses::all();
    }
  };
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "InjectBlockPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef PassName, ModulePassManager &MPM, ...) {
          if(PassName == "injectblock-pass"){
            MPM.addPass(InjectBlockPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}
