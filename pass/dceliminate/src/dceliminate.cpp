#include "dceliminate.hpp"

bool DCEliminatePass::runOnFunction(llvm::Function &F){
	//対象の命令を格納するlist
	std::list<llvm::Instruction*> works;


	//最初に全ての命令を調べる
	bool change=false;

	llvm::inst_iterator inst_it = llvm::inst_begin(F);
	llvm::inst_iterator inst_it_end = llvm::inst_end(F);
	for(;inst_it != inst_it_end; inst_it++)
			works.push_back(&*inst_it);

	// これでもInstructionを辿れる
    /*
	llvm::Function::iterator bb_it = F.begin();
	llvm::Function::iterator bb_it_end = F.end();
	for(;bb_it != bb_it_end; bb_it++){
		llvm::BasicBlock::iterator inst_it = bb_it->begin();
		llvm::BasicBlock::iterator inst_it_end = bb_it->end();
		for(;inst_it != inst_it_end; inst_it++)
			works.push_back(&*inst_it);
	}*/

	//worksに追加した命令からdead codeになった物がないか調べる
	//worksが空になるまで繰り返す
	llvm::Instruction *inst = NULL;
	while(!works.empty()){
		//命令をpop
		inst = works.front();
		works.pop_front();
		
		//この命令の値を使用している命令がある場合は生きているとする
		//load/store命令、call命令、終端命令(returnやbr)は生きている命令にする
		if( !inst->use_empty() || llvm::isa<llvm::TerminatorInst>(inst)  || 
				llvm::isa<llvm::CallInst>(inst) || 
				llvm::isa<llvm::LoadInst>(inst) || llvm::isa<llvm::StoreInst>(inst) )
			continue;


		
		//削除する命令のオペランドがdead codeになる可能性があるので
		//オペランドをworksに追加する
		llvm::User::op_iterator op_it = inst->op_begin();
		llvm::User::op_iterator op_it_end = inst->op_end();
		for(; op_it!=op_it_end; op_it++){
			//既にworksに登録済みなら何もしない
			llvm::Instruction *op_inst = llvm::dyn_cast<llvm::Instruction>(*op_it);
			if(op_inst)
				if(std::find( works.begin(), works.end(),  &*op_inst ) == works.end() )
					works.push_back(op_inst);
		}


		//削除する
		change=true;
		inst->eraseFromParent();
	}

	return change;
}

void DCEliminatePass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	//CFGに変更を加えない
	AU.setPreservesCFG();
}

char DCEliminatePass::ID=0;
static llvm::RegisterPass<DCEliminatePass> X("dceliminate", "eliminate dead code", false , false);


