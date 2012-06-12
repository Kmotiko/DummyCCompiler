#ifndef PARSER_HPP
#define PARSER_HPP

#include<cstdio>
#include<cstdlib>
#include<map>
#include<string>
#include<vector>
#include<llvm/DerivedTypes.h>
#include<llvm/Support/IRBuilder.h>
#include<llvm/Support/raw_ostream.h>
#include "APP.hpp"
#include "AST.hpp"
#include "lexer.hpp"
using namespace llvm;


/**
  * 構文解析・意味解析クラス
  */
typedef class Parser{
	public:

	private:
		TokenSet *Tokens;
		IRBuilder<> *Builder;
		TranslationUnitAST *TU;

		//意味解析用各種識別子表
		std::vector<std::string> VariableTable;
		std::map<std::string, int> PrototypeTable;
		std::map<std::string, int> FunctionTable;

	protected:

	public:
		Parser(std::string filename);
		~Parser(){SAFE_DELETE(TU);SAFE_DELETE(Tokens);}
		bool doParse();
		TranslationUnitAST &getAST();

	private:
		/**
		  各種構文解析メソッド
		  */
		BaseAST *visitPrimaryExpression();
		BaseAST *visitPostfixExpression();
		BaseAST *visitMultiplicativeExpression(BaseAST *lhs);
		BaseAST *visitAdditiveExpression(BaseAST *lhs);
		BaseAST *visitAssignmentExpression();
		FunctionStmtAST *visitFunctionStatement(PrototypeAST *proto);
		BaseAST *visitStatement();
		BaseAST *visitExpressionStatement();
		BaseAST *visitJumpStatement();
		bool visitExternalDeclaration(TranslationUnitAST *tunit);
		VariableDeclAST *visitVariableDeclaration();
		PrototypeAST *visitPrototype();
		PrototypeAST *visitFunctionDeclaration();
		FunctionAST *visitFunctionDefinition();
		bool visitTranslationUnit();


	protected:


}Parser;

#endif
