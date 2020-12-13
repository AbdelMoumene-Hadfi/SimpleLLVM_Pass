#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace llvm;

namespace {
  struct InjectBlockPass : public PassInfoMixin<InjectBlockPass> {
    Function* createFunc(Module &M) {
      auto &Ctx = M.getContext();
    }
    PreservedAnalyses run(Module &M,ModuleAnalysisManager &MAM) {
      Function *MainFunction = M.getFunction("main");
      createBlock(MainFunction,);
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
