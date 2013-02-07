#include "easypass.hpp"

bool EasyPass::runOnFunction(Function &F){
	fprintf(stderr, "Function Name : %s\n", F.getName().str().c_str());
	return false;
}

char EasyPass::ID=0;
static RegisterPass<EasyPass> X("easypass", "easy pass:only print function name", false , false);
