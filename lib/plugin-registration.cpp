/// @file pass-registration.cpp

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

#include <windows-gpt-transform/windows-gpt-transform.h>

#define DEBUG_TYPE "windows-gpt-transform"

namespace
{

llvm::PassPluginLibraryInfo getPluginInfo()
{
  return
  {
    LLVM_PLUGIN_API_VERSION,
    DEBUG_TYPE,
    LLVM_VERSION_STRING,
    [](llvm::PassBuilder& passBuilder)
    {
      passBuilder.registerPipelineParsingCallback(
        [](llvm::StringRef name, llvm::ModulePassManager& mpm,
          llvm::ArrayRef<llvm::PassBuilder::PipelineElement>)
        {
          if (name == DEBUG_TYPE)
          {
            mpm.addPass(jvs::WindowsGPTPass());
            return true;
          }

          return false;
        });
    }
  };
}

}  // namespace

extern "C" LLVM_ATTRIBUTE_WEAK auto llvmGetPassPluginInfo()
-> ::llvm::PassPluginLibraryInfo
{
  return getPluginInfo();
}
