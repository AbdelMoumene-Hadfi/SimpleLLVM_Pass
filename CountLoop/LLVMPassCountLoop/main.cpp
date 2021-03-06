#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  struct CountLoopPass : public PassInfoMixin<CountLoopPass> {
    void countBlockInLoop(Loop *L,int level) {
      int bbCounter=0;
      for(Loop::block_iterator bb=L->block_begin(),be=L->block_end();bb!=be;++bb) {
        bbCounter++;
      }
      std::vector<Loop*> subLoops = L->getSubLoops();
      for(Loop::iterator j=subLoops.begin(),f=subLoops.end();j!=f;++j) {
        countBlockInLoop(*j,level+1);
      }
      errs() << "LoopLevel : " << level << " Block : "<< bbCounter << "\n" ;
    }
    PreservedAnalyses run(Function &F,FunctionAnalysisManager &FAM) {
      if(F.hasName()) {
        errs() << "Function : " << F.getName() << "\n" ;
      }
      auto &LI = FAM.getResult<LoopAnalysis>(F);
      for(LoopInfo::iterator i=LI.begin(),e=LI.end();i!=e;++i) {
        countBlockInLoop(*i,0);
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
          if(PassName == "countloop-pass"){
            FPM.addPass(CountLoopPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}
