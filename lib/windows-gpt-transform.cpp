#include <windows-gpt-transform/windows-gpt-transform.h>

#include <vector>

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
  for (llvm::Function& f : m)
  {
    if (f.getName() != "main")
    {
      continue;
    }

    for (llvm::Instruction& inst : llvm::instructions(f))
    {
      for (auto& op : inst.operands())
      {
        if (auto* intVal = llvm::dyn_cast<llvm::ConstantInt>(op))
        {
          if (intVal->isOne())
          {
            // Change the 1 to a 0.
            auto* zeroVal = llvm::ConstantInt::get(intVal->getType(), 0);
            inst.setOperand(op.getOperandNo(), zeroVal);
            modified = true;
          }
        }
      }
    }
  }

  if (!modified)
  {
    return llvm::PreservedAnalyses::all();
  }

  return llvm::PreservedAnalyses::allInSet<llvm::CFGAnalyses>();
}
