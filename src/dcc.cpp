#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/IRReader.h"
#include "llvm/CodeGen/LinkAllAsmWriterComponents.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/Config/config.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
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
	public:

	protected:

	private:
		std::string InputFilename;
		std::string OutputFilename;
		int Argc;
		char **Argv;

	public:
		OptionParser(int argc, char **argv);
		void PrintHelp();
		std::string GetInputFilename();
		std::string GetInputFilenameRoot();
		std::string GetOutputFilename();
		bool ParseOption();

	protected:

	private:

};



/**
 *コンストラクタ
 * @param argc
 * @param argv
 */
OptionParser::OptionParser(int argc, char **argv){
	Argc=argc;
	Argv=argv;
}


/**
 * ヘルプ表示
 */
void OptionParser::PrintHelp(){
	fprintf(stdout, "help\n" );
}


/**
 * オプション切り出し
 * @return 成功時：true　失敗時：false
 */
bool OptionParser::ParseOption(){
	if(Argc < 2){
		fprintf(stderr,"引数が足りません\n");
		return false;
	}

	//inputfilename
	InputFilename.assign(Argv[1]);

	for(int i=2; i<Argc; i++){
		if(Argv[i][0]=='-' && Argv[i][1] == 'o' && Argv[i][2] == '\0'){
			//output filename
			OutputFilename.assign(Argv[++i]);
		}
	}

	//OutputFilename
	std::string ifn = InputFilename;
	int len = ifn.length();
	if (OutputFilename.empty() && (len > 2) &&
		ifn[len-3] == '.' &&
		((ifn[len-2] == 'd' && ifn[len-1] == 'c'))) {
		OutputFilename = std::string(ifn.begin(), ifn.end()-3); 
		OutputFilename += ".s";
	} else if(OutputFilename.empty()){
		OutputFilename = ifn;
		OutputFilename += ".s";
	}

	return true;
}


/**
* 入力ファイル名取得
* @return 入力ファイル名
*/
std::string OptionParser::GetInputFilename(){
	return InputFilename;
}


/**
* 出力ファイル名取得
* @return 出力ファイル名
*/
std::string OptionParser::GetOutputFilename(){
	return OutputFilename;
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
	if(!opt.ParseOption())
	  exit(1);

	//lex and parse
	Parser *parser=new Parser(opt.GetInputFilename());
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
	if(!codegen->doCodeGen(tunit, opt.GetInputFilename())){
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
	raw_fd_ostream raw_stream(opt.GetOutputFilename().c_str(), error);
	pm.add(createPrintModulePass(&raw_stream));
	pm.run(mod);
	raw_stream.close();
	mod.dump();

	//delete
	SAFE_DELETE(parser);
	SAFE_DELETE(codegen);
  
	return 0;
}
