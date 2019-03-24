#ifndef CODEGEN_HPP
#define CODEGEN_HPP


#include<cstdio>
#include<cstdlib>
#include<map>
#include<string>
#include<vector>
#include<llvm/ADT/APInt.h>
#include<llvm/IR/Constants.h>
#include<llvm/ExecutionEngine/ExecutionEngine.h>
#include<llvm/ExecutionEngine/MCJIT.h>
#include<llvm/Linker/Linker.h>
#include<llvm/IR/LLVMContext.h>
#include<llvm/IR/Module.h>
#include<llvm/IR/Metadata.h>
#include<llvm/Support/Casting.h>
#include<llvm/IR/IRBuilder.h>
#include<llvm/IRReader/IRReader.h>
#include<llvm/IR/MDBuilder.h>
#include<llvm/IR/ValueSymbolTable.h>
#include"APP.hpp"
#include"AST.hpp"
//using namespace llvm;


/**
  * コード生成クラス
  */
class CodeGen{
	private:
		llvm::Function *CurFunc;		//現在コード生成中のFunction
		llvm::Module *Mod;				//生成したModule を格納
		llvm::IRBuilder<> *Builder;	//LLVM-IRを生成するIRBuilder クラス

	public:
		CodeGen();
		~CodeGen();
		bool doCodeGen(TranslationUnitAST &tunit, std::string name, std::string link_file, bool with_jit);
		llvm::Module &getModule();


	private:
		bool generateTranslationUnit(TranslationUnitAST &tunit, std::string name);
		llvm::Function *generateFunctionDefinition(FunctionAST *func, llvm::Module *mod);
		llvm::Function *generatePrototype(PrototypeAST *proto, llvm::Module *mod);
		llvm::Value *generateFunctionStatement(FunctionStmtAST *func_stmt);
		llvm::Value *generateVariableDeclaration(VariableDeclAST *vdecl);
		llvm::Value *generateStatement(BaseAST *stmt);
		llvm::Value *generateBinaryExpression(BinaryExprAST *bin_expr);
		llvm::Value *generateCallExpression(CallExprAST *call_expr);
		llvm::Value *generateJumpStatement(JumpStmtAST *jump_stmt);
		llvm::Value *generateVariable(VariableAST *var);
		llvm::Value *generateNumber(int value);
		bool linkModule(llvm::Module *dest, std::string file_name);
};


#endif
