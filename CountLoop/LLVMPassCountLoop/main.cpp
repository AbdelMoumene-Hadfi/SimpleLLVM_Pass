#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  struct CountLoopPass : public PassInfoMixin<CountLoopPass> {
    PreservedAnalyses run(Function &F,FunctionAnalysisManager &FAM) {
      if(F.hasName()) {
        errs() << "Function : " << F.getName() << "\n" ;
      }
      auto &LI = FAM.getResult<LoopAnalysis>(F);
      int loopCounter=0;
      for(LoopInfo::iterator i=LI.begin(),e=LI.end();i!=e;++i) {
        Loop *L=*i;
        int bbCounter=0;
        loopCounter++;
        for(Loop::block_iterator bb=L->block_begin(),be=L->block_end();bb!=be;++bb) {
          bbCounter++;
        }
        errs() << "LOOP : " ;
        errs() << loopCounter ;
        errs() << ": #BBS = " ;
        errs() << bbCounter ;
        errs() << "\n" ;
      }
      return PreservedAnalyses::all();
    }
  };
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "CountLoopPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef PassName, FunctionPassManager &FPM, ...) {
          if(PassName == "count-loop-pass"){
            FPM.addPass(CountLoopPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}
