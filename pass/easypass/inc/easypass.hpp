#ifndef __EASYPASS_H__
#define __EASYPASS_H__

#include<cstdio>
#include<llvm/Function.h>
#include<llvm/Pass.h>
#include<llvm/Support/raw_ostream.h>
using namespace llvm;


class EasyPass : public FunctionPass{
    public:
        static char ID;
        EasyPass() : FunctionPass(ID){}
        ~EasyPass(){}

        virtual bool runOnFunction(Function &F);
        virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
