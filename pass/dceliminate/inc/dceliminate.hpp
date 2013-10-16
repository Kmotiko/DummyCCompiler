#ifndef __LPCOUNTPASS_H__
#define __LPCOUNTPASS_H__

#include<cstdio>
#include<list>
#include"llvm/Analysis/LoopPass.h"
#include"llvm/Analysis/LoopInfo.h"
#include"llvm/Constants.h"
#include"llvm/Instructions.h"
#include"llvm/Operator.h"
#include"llvm/Function.h"
#include"llvm/Pass.h"
#include"llvm/Support/Casting.h"
#include"llvm/Support/InstIterator.h"

class DCEliminatePass: public llvm::FunctionPass{
	public:
		static char ID;
		DCEliminatePass() : llvm::FunctionPass(ID){}
		~DCEliminatePass(){}

		virtual bool runOnFunction(llvm::Function &F);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
