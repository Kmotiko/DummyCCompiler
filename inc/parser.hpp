#ifndef PARSER_HPP
#define PARSER_HPP

#include<algorithm>
#include<cstdio>
#include<cstdlib>
#include<map>
#include<string>
#include<vector>
#include"APP.hpp"
#include"AST.hpp"
#include"lexer.hpp"
//using namespace llvm;


/**
  * 構文解析・意味解析クラス
  */
typedef class Parser{
	public:

	private:
		TokenStream *Tokens;
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
		bool visitTranslationUnit();
		bool visitExternalDeclaration(TranslationUnitAST *tunit);
		PrototypeAST *visitFunctionDeclaration();
		FunctionAST *visitFunctionDefinition();
		PrototypeAST *visitPrototype();
		FunctionStmtAST *visitFunctionStatement(PrototypeAST *proto);
		VariableDeclAST *visitVariableDeclaration();
		BaseAST *visitStatement();
		BaseAST *visitExpressionStatement();
		BaseAST *visitJumpStatement();
		BaseAST *visitAssignmentExpression();
		BaseAST *visitAdditiveExpression(BaseAST *lhs);
		BaseAST *visitMultiplicativeExpression(BaseAST *lhs);
		BaseAST *visitPostfixExpression();
		BaseAST *visitPrimaryExpression();


	protected:


}Parser;

#endif
