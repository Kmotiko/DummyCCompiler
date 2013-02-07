#include<cstdio>
#include<llvm/Pass.h>
#include<llvm/Function.h>
using namespace llvm;


class EasyPass : public FunctionPass{
	public:
		static char ID;
		EasyPass() : FunctionPass(ID){}
		~EasyPass(){}

		virtual bool runOnFunction(Function &F);
};
