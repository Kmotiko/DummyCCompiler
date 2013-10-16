#include "easypass.hpp"

bool EasyPass::runOnFunction(Function &F){
	//fprintf(stderr, "Function Name : %s\n", F.getName().str().c_str());
	llvm::errs() << "Function Name" << F.getName().str() << "\n";
	return false;
}

void EasyPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	//変更を加えない
	AU.setPreservesAll();
}

char EasyPass::ID=0;
static RegisterPass<EasyPass> X("easypass", "easy pass:only print function name", false , false);
