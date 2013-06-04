#ifndef __LPCOUNTPASS_H__
#define __LPCOUNTPASS_H__

#include<cstdio>
#include"llvm/Analysis/LoopPass.h"
#include"llvm/Analysis/LoopInfo.h"
#include"llvm/Constants.h"
#include"llvm/Instructions.h"
#include"llvm/Operator.h"
#include"llvm/Function.h"
#include"llvm/Pass.h"
#include"llvm/Support/Casting.h"

class LPCountPass : public llvm::LoopPass{
	public:
		static char ID;
		LPCountPass() : llvm::LoopPass(ID),IterateTime(-1){}
		~LPCountPass(){}

		virtual bool runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
