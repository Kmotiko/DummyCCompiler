#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "lexer.hpp"
#include "AST.hpp"
#include "parser.hpp"
#include "codegen.hpp"
using namespace llvm;


/**
 * オプション切り出し用クラス
 */
class OptionParser
{
	private:
		std::string InputFileName;
		std::string OutputFileName;
		int Argc;
		char **Argv;

	public:
		OptionParser(int argc, char **argv):Argc(argc), Argv(argv){}
		void printHelp();
		std::string getInputFileName(){return InputFileName;} 		//入力ファイル名取得
		std::string getOutputFileName(){return OutputFileName;} 	//出力ファイル名取得
		bool parseOption();
};


/**
 * ヘルプ表示
 */
void OptionParser::printHelp(){
	fprintf(stdout, "Compiler for DummyC...\n" );
	fprintf(stdout, "試作中なのでバグがあったらご報告を\n" );
}


/**
 * オプション切り出し
 * @return 成功時：true　失敗時：false
 */
bool OptionParser::parseOption(){
	if(Argc < 2){
		fprintf(stderr,"引数が足りません\n");
		return false;
	}


	for(int i=1; i<Argc; i++){
		if(Argv[i][0]=='-' && Argv[i][1] == 'o' && Argv[i][2] == '\0'){
			//output filename
			OutputFileName.assign(Argv[++i]);
		}else if(Argv[i][0]=='-' && Argv[i][1] == 'h' && Argv[i][2] == '\0'){
			printHelp();
			return false;
		}
		else{
			//inputfilename
			InputFileName.assign(Argv[i]);
		}
	}

	//OutputFileName
	std::string ifn = InputFileName;
	int len = ifn.length();
	if (OutputFileName.empty() && (len > 2) &&
		ifn[len-3] == '.' &&
		((ifn[len-2] == 'd' && ifn[len-1] == 'c'))) {
		OutputFileName = std::string(ifn.begin(), ifn.end()-3); 
		OutputFileName += ".s";
	} else if(OutputFileName.empty()){
		OutputFileName = ifn;
		OutputFileName += ".s";
	}

	return true;
}


/**
 * main関数
 */
int main(int argc, char **argv) {
	InitializeNativeTarget();
	sys::PrintStackTraceOnErrorSignal();
	PrettyStackTraceProgram X(argc, argv);

	EnableDebugBuffering = true;

	OptionParser opt(argc, argv);
	if(!opt.parseOption())
	  exit(1);

	//lex and parse
	Parser *parser=new Parser(opt.getInputFileName());
	if(!parser->doParse()){
		fprintf(stderr, "err at parser or lexer\n");
		SAFE_DELETE(parser);
		exit(1);
	}

	//get AST
	TranslationUnitAST &tunit=parser->getAST();
	if(tunit.empty()){
		fprintf(stderr,"TranslationUnit is empty");
		SAFE_DELETE(parser);
		exit(1);
	}

	CodeGen *codegen=new CodeGen();
	if(!codegen->doCodeGen(tunit, opt.getInputFileName())){
		fprintf(stderr, "err at codegen\n");
		SAFE_DELETE(parser);
		SAFE_DELETE(codegen);
		exit(1);
	}

	//get Module
	Module &mod=codegen->getModule();
	if(tunit.empty()){
		fprintf(stderr,"Module is empty");
		SAFE_DELETE(parser);
		SAFE_DELETE(codegen);
		exit(1);
	}

	PassManager pm;

	//SSA化
	pm.add(createPromoteMemoryToRegisterPass());

	//出力
	std::string error;
	raw_fd_ostream raw_stream(opt.getOutputFileName().c_str(), error);
	pm.add(createPrintModulePass(&raw_stream));
	pm.run(mod);
	raw_stream.close();
	mod.dump();

	//delete
	SAFE_DELETE(parser);
	SAFE_DELETE(codegen);
  
	return 0;
}
