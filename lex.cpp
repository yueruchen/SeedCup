#include"lex.h"

//关键字
string keyword[8]={"if","while","do","int","break","else","for","printf"};
int keyword_num[9]={IF,WHILE,DO,INT,BREAK,ELSE,FOR,PRINTF};

//部分运算符，定界符等
char symbol[9]={'*','/',';','(',')','{','}',','};
int symbol_num[9]={MATHOP,MATHOP,SEMICOLON,LC,RC,LBC,RBC,COMMA};

//添加Token
Position AddToken(string name,int type,int line  ){
    Position NewToken = (Position)malloc(LEN);
    NewToken->name = name;
    NewToken->type = type;
    NewToken->line_number = line;
    return NewToken;
}
//判断是否为字母
bool IsLetter(char ch) {
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_'))
        return true;
    return false;
}

//判断是否为数字
bool IsDigit(char ch) {
    if(ch>='0'&&ch<='9')
        return true;
    return false;
}

//判断是否为定界符等
int IsSymbol(char ch) {
    for(int i=0;i<12;i++) {
        if(ch==symbol[i])
            return i;
    }
    return -1;
}

//判断是否为关键字
int IsKeyword(string str) {
    for(int i=0;    i<8;    i++) {
        if(str==keyword[i]) {
            return i;
        }
    }
    //不是关键字即为8
    return 8;
}
//词法分析器
Position lex() {
    Position HEAD,Last,Temp;
    HEAD = (Position)malloc(LEN);
    Last = HEAD;
    HEAD->prev = NULL;
    char instr[255]={}; //接收输入字符串
    bool flag=false; //多行注释标志,false为未处于注释区域
    string Token;//存放字符串
    freopen("/Users/yifan/seedcup/SeedCup/test.cpp","r",stdin);
    int line=1;
    int exc_flag = false; //记录!=
//    freopen("result.txt","w",stdout); //此行注释后，控制台输出，
    //否则文本输出
    while((fgets(instr,256,stdin))!=NULL) {
        for(int i=0;i<strlen(instr);i++){
            if((*(instr+i)==' ') || (*(instr+i)=='\t') ){
                i++;
            }
            if( (*(instr+i)=='/') && (*(instr+i+1)=='/')){
                break;
            }
            if(*(instr+i)=='/'){
                if(*(instr+i+1)=='*') {
                    i++;
                    flag=true;
                    continue;
                }
            }
            //注释处理: */,注释区域结束
            if(*(instr+i)=='*'&&flag){
                if(*(instr+i+1)=='/'){
                    flag=false;
                    i++;
                    continue;
                }
            }
            //标识符，关键词
            if(IsLetter(*(instr+i))&&(!flag)){
                while(IsLetter(*(instr+i))||IsDigit(*(instr+i))||*(instr+i)=='_'){
                    Token+=*(instr+i);
                    i++;
                }
                if(IsKeyword(Token)!=8){
                    Temp = AddToken(Token,keyword_num[IsKeyword(Token)],line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                }else {
                    Temp = AddToken(Token,VAR,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                }
                Token="";
            }
            if(IsDigit(*(instr+i))&&(!flag)){
                while(IsDigit(*(instr+i))){
                    Token+=*(instr+i);
                    i++;
                }
                Temp = AddToken(Token,NUMBER,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                Token="";
            }
            //!=
            if((*(instr+i)=='!') && (*(instr+i+1)=='=') &&(!flag)){
                Temp = AddToken("!=",COMOP,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                i++;
                exc_flag = true;
            }
            //+
            if((*(instr+i)=='+') && (*(instr+i+1)!='+')&&(!flag)){
                Temp = AddToken("+",MATHOP,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
            }
            //++
            if((*(instr+i)=='+') && (*(instr+i+1)=='+')&&(!flag)){
                Temp = AddToken("++",INC,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                i++;
            }
            // -或者-1,或者-a(a为变量)
            if((*(instr+i)=='-')){
                if(((*(instr+i-1)=='+') || (*(instr+i-1)=='*') || (*(instr+i-1) == '/')) && (!flag)){
                    Token += '-';
                }else if((*(instr + i) == '-') && ((*(instr + i + 1)) != '-') &&(!flag)) {
                    Temp = AddToken("-", MATHOP, line);
                    Last->next = Temp;
                    Temp->prev = Last;
                    Last = Temp;
                }
            }
            //--
            if(*(instr+i)=='-'&&(*(instr+i+1))=='-'&&(!flag)){
                Temp = AddToken("--",DEC,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                i++;
            }
            if(*(instr+i)=='\"'&&(!flag)){
                char *temp_string =NULL;
                temp_string = instr;
                int s =i;
                while(*(instr+s)!='\0'){
                    s++;
                    if(*(instr+s)=='\"'){
                        temp_string = (instr+s);
                    }
                }
                while((temp_string != (instr+i))&&(!flag)){
                    Token+=*(instr+i);
                    i++;
                }
                Token+=*(instr+i);
                i++;
                Temp = AddToken(Token,STRING,line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                Token="";
            }
            if(*(instr+i)=='='&&(!flag)&&(!exc_flag)){
                if(*(instr+i+1)=='='){
                    Temp = AddToken("==",COMOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    i++;
                }else{
                    Temp = AddToken("=",ASSIGN,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                }
            }
            //<,<=,<>
            if(*(instr+i)=='<'&&(!flag)){
                if(*(instr+i+1)=='='){
                    Temp = AddToken("<=",COMOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    i++;
                } else {
                    Temp = AddToken("<",COMOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                }
            }
                //>,>=
            else if(*(instr+i)=='>'&&(!flag)){
                if(*(instr+i+1)=='=') {
                    Temp = AddToken(">=",COMOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    i++;
                } else {
                    Temp = AddToken(">",COMOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                }
            }
                //余下定界符等
            else if(IsSymbol(*(instr+i))!=-1&&(!flag)){
                Token+=*(instr+i);
                Temp = AddToken(Token,symbol_num[IsSymbol(*(instr+i))],line);
                Last->next= Temp;
                Temp->prev = Last;
                Last=Temp;
                Token="";
            }
        }
        line++;
        exc_flag = false;
    }
    return HEAD;
}

int main(){
    Position Last = lex();
    printf("\n\n\n");
    Last = Last->next;
    while(Last->next!=NULL){
        printf("Token {%d,   line:%d,     %s }\n",Last->type,Last->line_number,Last->name.c_str());
        Last = Last->next;
    }
    printf("Token {%d,   line:%d,     %s }\n",Last->type,Last->line_number,Last->name.c_str());
    return 0;
}
