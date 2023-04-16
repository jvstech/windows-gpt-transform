#if !defined(JVS_WINDOWS_GPT_TRANSFORM_H_)
#define JVS_WINDOWS_GPT_TRANSFORM_H_

#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>

namespace jvs
{

struct WindowsGPTPass : llvm::PassInfoMixin<WindowsGPTPass>
{
  llvm::PreservedAnalyses run(llvm::Module& m, llvm::ModuleAnalysisManager& am);
};

}  // namespace jvs


#endif  // !JVS_WINDOWS_GPT_TRANSFORM_H_
