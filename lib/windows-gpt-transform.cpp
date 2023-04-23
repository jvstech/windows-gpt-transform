#include <windows-gpt-transform/windows-gpt-transform.h>

#include <string>
#include <vector>

#include <llvm/ADT/SetVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Debug.h>

#define DEBUG_TYPE "windows-gpt-transform"

llvm::PreservedAnalyses jvs::WindowsGPTPass::run(
  llvm::Module& m, llvm::ModuleAnalysisManager& am)
{
  bool modified = false;
  llvm::SetVector<llvm::GlobalVariable*> stringGlobals;
  // Iterate over all the global variables in the program and find the ones which are
  // C strings.
  for (llvm::GlobalVariable& gv : m.globals())
  {
    if (!gv.isConstant() || !gv.hasInitializer())
    {
      continue;
    }

    if (auto* cda = llvm::dyn_cast<llvm::ConstantDataArray>(gv.getInitializer()))
    {
      if (cda->isCString())
      {
        stringGlobals.insert(&gv);
      }
    }
  }

  while (!stringGlobals.empty())
  {
    llvm::GlobalVariable* gv = stringGlobals.pop_back_val();
    auto* cda = llvm::cast<llvm::ConstantDataArray>(gv->getInitializer());
    std::string s = cda->getAsCString().str();
    std::reverse(s.begin(), s.end());
    auto* reversedStrConst = llvm::ConstantDataArray::getString(m.getContext(), s);
    auto* reversedGV = new llvm::GlobalVariable(m, gv->getValueType(), /*isConstant*/ true,
      gv->getLinkage(), reversedStrConst, gv->getName() + ".rev");
    modified = true;
    gv->replaceAllUsesWith(reversedGV);
    gv->eraseFromParent();
  }

  if (!modified)
  {
    return llvm::PreservedAnalyses::all();
  }

  return llvm::PreservedAnalyses::allInSet<llvm::CFGAnalyses>();
}
