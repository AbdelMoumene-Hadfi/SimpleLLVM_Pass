#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace llvm;

namespace {
  struct HelloNewPMPass : public PassInfoMixin<HelloNewPMPass> {
    std::map<std::string,int> opCounter;
    PreservedAnalyses run(Function &F,FunctionAnalysisManager &FAM) {
      if(F.hasName()) {
        errs() << "Function : " << F.getName() << "\n" ;
      }
      for(Function::iterator bb=F.begin(),eb=F.end();bb!=eb;++bb) {
        for(BasicBlock::iterator i=bb->begin(),e=bb->end();i!=e;++i) {
          if(opCounter.find(i->getOpcodeName())==opCounter.end()) {
            opCounter[i->getOpcodeName()]=1;
          }
          else {
            opCounter[i->getOpcodeName()]+=1;
          }
        }
      }
      for(std::map<std::string,int>::iterator i=opCounter.begin(),e=opCounter.end();i!=e;++i) {
        errs() << " " << i->first << ": " << i->second << "\n";
      }
      opCounter.clear();
      return PreservedAnalyses::all();
    }
  };
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "HelloNewPMPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef PassName, FunctionPassManager &FPM, ...) {
          if(PassName == "hello-new-pm-pass"){
            FPM.addPass(HelloNewPMPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}
