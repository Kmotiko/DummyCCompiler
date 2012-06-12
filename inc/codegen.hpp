#ifndef CODEGEN_HPP 
#define CODEGEN_HPP


#include<cstdio>
#include<cstdlib>
#include<map>
#include<string>
#include<vector>
#include<llvm/DerivedTypes.h>
#include<llvm/LLVMContext.h>
#include<llvm/Module.h>
#include<llvm/Support/Casting.h>
#include<llvm/Support/IRBuilder.h>
#include"APP.hpp"
#include"AST.hpp"
using namespace llvm;


/**
  * コード生成クラス
  */
class CodeGen{
	private:
		std::map<std::string, Value*> ValueMap;
		Module *Mod;
		IRBuilder<> *Builder;

	public:
		CodeGen();
		~CodeGen();
		bool doCodeGen(TranslationUnitAST &tunit, std::string name);
		Module &getModule();


	private:
		Value *generateVariable(VariableAST *var);
		Value *generateVariableDeclaration(VariableDeclAST *vdecl);
		Value *generateCallExpression(CallExprAST *call_expr);
		Value *generateJumpStatement(JumpStmtAST *jump_stmt);
		Value *generateBinaryExpression(BinaryExprAST *bin_expr);
		Value *generateStatement(BaseAST *stmt);
		Value *generateFunctionStatement(FunctionStmtAST *func_stmt);
		Function *generatePrototype(PrototypeAST *proto, Module *mod);
		Function *generateFunctionDefinition(FunctionAST *func, Module *mod);
		bool generateTranslationUnit(TranslationUnitAST &tunit, std::string name);


};


#endif
