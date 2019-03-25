#include "lexer.hpp"



/**
 * トークン切り出し関数
 * @param 字句解析対象ファイル名
 * @return 切り出したトークンを格納したTokenStream
 */
TokenStream *LexicalAnalysis(std::string input_filename){
	TokenStream *tokens=new TokenStream();
	std::ifstream ifs;
	std::string cur_line;
	std::string token_str;
	int line_num=0;
	bool iscomment=false;
	int indent=0;//indent level
	int indent_length=2;

	ifs.open(input_filename.c_str(), std::ios::in);
	if(!ifs)
		return NULL;
	while(ifs && getline(ifs, cur_line)){
		char next_char;
		std::string line;
		Token *next_token;
		int index=0;
		int length=cur_line.length();
		int ishead=true;

		while(index<length){
			next_char = cur_line.at(index++);
			// printf("%c\n", next_char);
			//コメントアウト読み飛ばし
			if(iscomment){
				if( (length-index) < 2
					||	(cur_line.at(index) != '*')
					|| (cur_line.at(index++) != '/') ){
					continue;
				}else{
					iscomment=false;
				}
			}

			//EOF
			if(next_char == EOF){
				token_str = EOF;
				next_token = new Token(token_str, TOK_EOF, line_num);
			//Space or INDENT
			}else if(isspace(next_char)){
				if(!ishead)continue;
					token_str += next_char;
					while(isspace(cur_line.at(index))){
						token_str += next_char;
						next_char = cur_line.at(index++);
					}
					if(!indent)indent_length=token_str.size();
					//インデントが増えていればINDENT,減っていればDEDENT, そのままなら無視
					if(token_str.size() > indent*indent_length){
						next_token = new Token("INDENT", TOK_INDENT, line_num);
						indent++;
						ishead = false;
					}else if(indent && token_str.size() < indent*indent_length){
						printf("%d\n", indent*indent_length);
						printf("%d\n", token_str.size());

						next_token = new Token("DEDENT", TOK_DEDENT, line_num);
						indent--;
						ishead = false;
					}else{
						ishead = false;
						token_str.clear();
						continue;
					}
				// Indentレベル１からのDEDENT
			}else if(!isspace(next_char) && ishead && indent){
					next_token = new Token("DEDENT", TOK_DEDENT, line_num);
					indent--;
					while(indent){
						tokens->pushToken(next_token);
						indent--;
					}
					ishead=false;
					index--;//今選択している文字をまきもどし
				//IDENTIFIER
				}else if(isalpha(next_char)){
					while(isalnum(next_char)){
						token_str += next_char;
						if(index==length)
							break;
						next_char = cur_line.at(index++);
					}
					if(!isalnum(next_char))
					    index--;
				if(token_str == "int"){
					next_token = new Token(token_str, TOK_INT, line_num);
					ishead = false;
				}else if(token_str == "return"){
					next_token = new Token(token_str, TOK_RETURN, line_num);
					ishead = false;
				}else{
					next_token = new Token(token_str, TOK_IDENTIFIER, line_num);
					ishead = false;
				}

			//数字
			}else if(isdigit(next_char)){
				if(next_char=='0'){
					token_str += next_char;
					next_token = new Token(token_str, TOK_DIGIT, line_num);
					ishead = false;
				}else{
					while(isdigit(next_char)){
						token_str += next_char;
					    if(index==length)
						    break;
						next_char = cur_line.at(index++);
					}
					if(!isdigit(next_char))index--;
					next_token = new Token(token_str, TOK_DIGIT, line_num);
					ishead = false;
				}

			//コメント or '/'
			}else if(next_char == '/'){
				token_str += next_char;
				next_char = cur_line.at(index++);

				//コメントの場合
				if(next_char == '/'){
						break;

				//コメントの場合
				}else if(next_char == '*'){
					iscomment=true;
					continue;

				//DIVIDER('/')
				}else{
					index--;
					next_token = new Token(token_str, TOK_SYMBOL, line_num);
					ishead = false;
				}

			//それ以外(記号)
			}else{
				if(next_char == '*' ||
						next_char == '+' ||
						next_char == '-' ||
						next_char == '=' ||
						next_char == ';' ||
						next_char == ',' ||
						next_char == '(' ||
						next_char == ')' ||
						next_char == '{' ||
						next_char == '}'){
					token_str += next_char;
					next_token = new Token(token_str, TOK_SYMBOL, line_num);
					ishead = false;

				//解析不能字句
				}else{
					fprintf(stderr, "unclear token : %c", next_char);
					SAFE_DELETE(tokens);
					return NULL;
				}
			}

			//Tokensに追加
			tokens->pushToken(next_token);
			token_str.clear();
		}
		if(!ishead)tokens->pushToken(new Token("NEWLINE", TOK_NEWLINE, line_num));
		token_str.clear();
		line_num++;
	}


	//EOFの確認
	if(ifs.eof()){
		while(indent){
			tokens->pushToken(
				new Token("DEDENT", TOK_DEDENT, line_num)
			);
			indent--;
		}
		tokens->pushToken(
				new Token(token_str, TOK_EOF, line_num)
				);
	}

	//クローズ
	ifs.close();
	return tokens;
}



/**
  * デストラクタ
  */
TokenStream::~TokenStream(){
	for(int i=0; i<Tokens.size(); i++){
		SAFE_DELETE(Tokens[i]);
	}
	Tokens.clear();
}


/**
  * トークン取得
  * @return CureIndex番目のToken
  */
Token TokenStream::getToken(){
	return *Tokens[CurIndex];
}


/**
  * インデックスを一つ増やして次のトークンに進める
  * @return 成功時：true　失敗時：false
  */
bool TokenStream::getNextToken(){
	int size=Tokens.size();
	if(--size==CurIndex){
		return false;
	}else if( CurIndex < size ){
		CurIndex++;
		return true;
	}else{
		return false;
	}
}


/**
  * インデックスをtimes回戻す
  */
bool TokenStream::ungetToken(int times){
	for(int i=0; i<times;i++){
		if(CurIndex == 0)
			return false;
		else
			CurIndex--;
	}
	return true;
}


/**
  * 格納されたトークン一覧を表示する
  */
bool TokenStream::printTokens(){
	std::vector<Token*>::iterator titer = Tokens.begin();
	while( titer != Tokens.end() ){
		fprintf(stdout,"%d:", (*titer)->getTokenType());
		if((*titer)->getTokenType()!=TOK_EOF)
			fprintf(stdout,"%s\n", (*titer)->getTokenString().c_str());
		++titer;
	}
	return true;
}

// int main(int argc, char** argv){
//   std::string inputFile;
//   inputFile=argv[1];
//   TokenStream *Test = LexicalAnalysis(inputFile);
//   Test->printTokens();
//   return 0;
// }
