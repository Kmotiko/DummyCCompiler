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
		bool generateTranslationUnit(TranslationUnitAST &tunit, std::string name);
		Function *generateFunctionDefinition(FunctionAST *func, Module *mod);
		Function *generatePrototype(PrototypeAST *proto, Module *mod);
		Value *generateFunctionStatement(FunctionStmtAST *func_stmt);
		Value *generateVariableDeclaration(VariableDeclAST *vdecl);
		Value *generateStatement(BaseAST *stmt);
		Value *generateBinaryExpression(BinaryExprAST *bin_expr);
		Value *generateCallExpression(CallExprAST *call_expr);
		Value *generateJumpStatement(JumpStmtAST *jump_stmt);
		Value *generateVariable(VariableAST *var);


};


#endif
