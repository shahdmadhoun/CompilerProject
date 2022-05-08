//compiler Project 1 
//By : Raghad AbuSharar 181044
    // Shahd Madhoun 181064
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<curses.h>

int lookup(char s[]);
int insert(char s[],  int tok);
void parse();
void Term();
void Factor();
void match(int t);
int lexan();
void error(char *m);
void emit(int t,int tval);
void init();
void Declarations();
void ConstantDefinitions();
void VariableDeclarations();
void ConstantDefinition();
void VariableDeclaration();
void IdentifierList();
void Type();
void Block();
void Statements();
void SimpleExpression();
void Statement();
void ElseClause();
void ExpressionList();
void Expression();


#define STRMAX 999
#define SYMMAX 100 
#define BSIZE 128 
#define NUM 256
#define NONE -1
#define EOS '\0'
#define DIV 257
#define MOD 258
#define ID 259
#define BEGIN 260
#define END 261
#define PROGRAM 262
#define INFIX 263
#define POSTFIX 264
#define VAR 265
#define EPSILON 266
#define CONST 267
#define INTEGER 268
#define REAL 269
#define CHAR 270
#define BOOLEAN 271
#define IF 272
#define THEN 273
#define ELSE 274
#define NOT 275
#define DO 276
#define RELOP 277
#define ADDOP 278
#define MULOP 279
#define INPUT 280
#define OUTPUT 281
#define WHILE 282
#define WRITELN 283
#define AND 284
#define OR 285

FILE *input, *output ;
int tokenval;
int lineno;

char ids[] ;
char *dataType = "int ";

struct entry { 
    char *lexptr;
    int token;
};

struct entry symtable[]; 

char lexemes [STRMAX];
int lastchar = -1; 
struct entry symtable[SYMMAX];
int lastentry = 0; 
int lookahead;


struct entry keywords[] = {
    "div", DIV,
    "mod", MOD,
	"PROGRAM", PROGRAM,
	"BEGIN", BEGIN,
	"END" , END,
	"infix",INFIX,
	"postfix",POSTFIX,
    "VAR", VAR,
	"EPSILON" , EPSILON,
    "CONST", CONST,
    "INTEGER", INTEGER,
	"REAL", REAL,
	"CHAR", CHAR,
	"BOOLEAN", BOOLEAN,
    "IF", IF,
	"ELSE", ELSE,
    "AND",AND,
	"OR",OR,
    "NOT", NOT,
    "RELOP", RELOP,
	"ADDOP", ADDOP,
	"MULOP", MULOP,
    "NUM", NUM,
    "INPUT", INPUT,
	"OUTPUT" , OUTPUT,
    "THEN", THEN,
    "DO", DO,
    "WHILE", WHILE,
    "WRITELN", WRITELN,
     0, 0
	
};


int lookup(char s[])     
{
    int p;
    for (p = lastentry; p > 0; p = p - 1)
		if (strcmp( symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}

int insert(char s[],  int tok) 
{
    int len;
    len = strlen(s); 
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}



void parse()
{ 
   lookahead = lexan();
	match(PROGRAM);
    match(ID); 
	match('(');
	match(INPUT);
	match(',');
	match(OUTPUT);
	match(')');
	match(';');
	fprintf(output,"#include<iostream>\nusing namespace std;\n\n");
	Declarations();
	fprintf(output,"\nint main(void)");
	Block();
}

void Declarations(){
    printf("from Declarations\n");
	while(1){
	    switch(lookahead){
        case CONST:
            emit(lookahead,NONE);
            match(CONST);
            fprintf(output , "double ");
            ConstantDefinitions();
            while(1){
                lookahead = lexan();
                switch (lookahead) {
                case VAR:
                //emit(lookahead,NONE);
                match(VAR);
                VariableDeclarations();
                break;
                default:
                break;
                }
                return;
            }
            break;
        case VAR:
            //emit(lookahead,NONE);
            match(VAR);
            VariableDeclarations();
            break;
        default:
        return ;
        }  
	} 
}


void ConstantDefinitions() {
    printf( "from ConstantDefinitions\n");
    ConstantDefinition();
    while(1){
        switch (lookahead) {
        case ID:
            match(ID);
            ConstantDefinition();
	        continue;
        default:
        return;
        }
    }
}


void ConstantDefinition() {
    printf( "from ConstantDefinition\n");
    emit(ID, tokenval);
    match(ID);
    match('=');
    fprintf(output,"=");
    emit(NUM, tokenval);
    match(NUM);
    fprintf(output,";\n");

}

void VariableDeclarations() {
    printf( "from VariableDeclarations\n");
    VariableDeclaration();
    while(1)
    switch (lookahead) {
        case ID:
            match(ID);
            VariableDeclaration();
	        continue;
        default:
        return;
    }
}

void VariableDeclaration(){
    printf( "from VariableDeclaration\n");
    fprintf(output, dataType);
    IdentifierList();
    match(':');
    Type();
    size_t len = strlen(ids);
    for(int i =0 ; i < len ;i++)
   	emit(ID , ids[i]);
    match(';');
    fprintf(output,";\n");
    
}

void IdentifierList(){
    printf( "from IdentifierList\n");
	int i = 1;
    ids[0]=ID;
    fprintf(output,"%s ", symtable[tokenval].lexptr);
	match(ID);
	while(1)
    switch (lookahead) {
        case ',':
         match(',');
        fprintf(output ,", ");
         emit(ID, tokenval);
        ids[i]=ID;
        i=i+1;
	     match(ID);
	     continue;
        default:
        return;
    }
}


void Type(){
        printf( "from Type\n");

		switch (lookahead) {
		case INTEGER: case REAL : case CHAR: case BOOLEAN:
			emit(lookahead, NONE);
			match(lookahead);
            break;
        default:
        return;
		} 
}


void Block(){
    printf( "from Block\n");
    match(BEGIN);
    fprintf(output,"{\n");
    Statements();
    match(END);
    fprintf(output,"\n}");
}

void Statements(){
    printf( "from Statements\n");
    Statement();
    while(1)
    switch (lookahead) {
        case ';':
            match(';');
            fprintf(output,";\n");
            Statement();
            continue;
        default: 
		return;
    }
}


void Statement(){ 
    printf( "from Statement\n");
    char t;
    switch (lookahead) {
        case ID:
            emit(ID, tokenval);
            match(ID);
            match(':');
            match('=');
            fprintf(output,"=");
            Expression();
            break;
        case BEGIN: 
            Block();
          break;
        case IF:
        	emit(lookahead,NONE);
            match(IF);
            fprintf(output,"(");
            Expression();
            fprintf(output,") ");
            match(THEN);
            Statement();
            fprintf(output,";\n");
            ElseClause();
            break;
        case WHILE:
            match(WHILE);
            fprintf(output,"while(");
            Expression();
            fprintf(output,") {\n");
            match(DO);
            Statement();
            fprintf(output,"\n}\n");
            break;
        case WRITELN:
            match(WRITELN);
            fprintf(output,"cout");
            match('(');
            fprintf(output,"<<");
            SimpleExpression();
            match(')');
            fprintf(output,"<<endl;");
            break;
        default:
        return;
    }
}




void ElseClause(){
    printf( "from ElseClause\n");
    switch (lookahead) {
    case ELSE :
        emit(lookahead,NONE);
        match(ELSE);
        Statement();
        break;
    default:
    return; 
    }
}


void ExpressionList(){
    printf( "from ExpressionList\n");
    Expression();
    while(1)
        switch (lookahead) {
        case ',' :
            match(',');
            Expression();
            break;
        default:
        return; 
        }
} 

void Expression(){
    printf( "from Expression\n");
    SimpleExpression();
     switch (lookahead) {
        case '=' : case '>' : case '<': case '>=' : case '<=' :case '<>':
            emit(lookahead,NONE);
		    match(lookahead);
            SimpleExpression();
         break;
        default:
        return;
     }
}


void SimpleExpression() {
    printf( "from simpleExpression\n");
    switch (lookahead) {
    case ID: case NUM : case '(': case NOT:
        Term();
        while(1)
        if (lookahead == '+' ||lookahead == '-'||lookahead ==OR) {
            emit(lookahead,NONE);
            match(lookahead);
            Term();
			}
        else 
        return; 

    case '+': case '-': case OR :
       emit(lookahead,NONE);
	   match(lookahead);
       Term();
        while(1)
        if (lookahead == '+' ||lookahead == '-'||lookahead ==OR) {
            emit(lookahead,NONE);
            match(lookahead);
            Term();
		}
        else 
           return ;
    }
}

void Term(){
	int t;
    Factor();
    while(1)
    switch (lookahead) {
    case '*' : case '/' : case DIV: case MOD: case AND:
    	t=lookahead;
        match(lookahead);
        Factor();
        emit(t ,NONE);
        match(lookahead);
        Factor();
        break;
    default:
    return;
    }
}


void Factor(){
    switch (lookahead) {
        case ID:
            emit(ID, tokenval);
            match(ID);
            break;
        case NUM:
            emit(NUM, tokenval);
            match(NUM);
            break;
        case '(':
            match('(');
            fprintf(output,"(");
            Expression();
            match(')');
            fprintf(output,")");
            break;
        case NOT:
		    emit(NOT ,NONE);
            match(NOT);
            Factor();
            break;
        default:
            return;
    }
}



void match(int t)
{
    if (lookahead == t)
        lookahead = lexan();
	
    else error("syntax error");
}



char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;

//lexical analyzer
int lexan() 
{

    int t;
    while (1) {
        t = fgetc(input);
        if (t == '#') { 
            //read all lines
            t = getc(input);
            while (t != '\n') {
                t = getc(input);
            }
        }
        if (t == ' ' || t == '\t') 
            ; 
        else if (t == '\n')
            lineno = lineno + 1;
        else if (isdigit(t)) { 
            ungetc(t, input);
            fscanf(input,"%d", &tokenval);
            return NUM;
        }
        else if (isalpha(t)) {  
            int p, b = 0;
            while (isalnum(t)) { 
                lexbuf[b] = t;
                t = fgetc(input);
                b = b + 1;
                if (b >= BSIZE)
                    error("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF)
                ungetc(t, input);
            p = lookup(lexbuf);
            if (p == 0)
                p = insert(lexbuf, ID);
            tokenval = p;
            return symtable[p].token;
        }
		else if (t == EOF){
			return END;
		}
        else {
            tokenval = NONE;
            return t;
        }
    }
}

void init() 
{
    struct entry *p;
    for (p = keywords; p->token; p++)
        insert(p->lexptr, p->token);
}

void error(char *m) 
{
fprintf(stderr, "line %d: %s\n", lineno, m);
exit(1); 
}

void emit(int t,int tval)  
{
    switch(t) {
    case '+': case '-': case '*': case '\\': case '%': case '=' : case '>' : case '<': case '>=' : case '<=' :case '<>':
        fprintf(output ,"%c ", t); break;
    case DIV:
        fprintf(output,"DIV "); break;
	case PROGRAM:
        fprintf(output,"program"); break;
	case BEGIN:
        fprintf(output,"BEGIN"); break;
	case END:
        fprintf(output,"end"); break;
	case INFIX:
        fprintf(output,"infix"); break;
	case POSTFIX:
        fprintf(output,"postfix"); break;
    case MOD:
        fprintf (output,"MOD "); break;
    case NUM:
        fprintf(output,"%d ", tval); break;
    case ID:
		fprintf(output,"%s ", symtable[tval].lexptr); break;
    case AND:
		fprintf(output,"AND "); break;
    case OR:
		fprintf(output,"OR "); break;
    case CONST:
		fprintf(output,"const "); break;
    case VAR:
		fprintf(output,"var "); break;
	case INTEGER:
	    dataType = "int ";
		break;
    case REAL:
        dataType = "real ";
		break;
    case CHAR:
        dataType = "char ";
		break;
	case BOOLEAN:
        dataType = "bool ";
		break;
    case IF:
		fprintf(output,"if "); break;
	case ELSE :
		fprintf(output,"else "); break;

    default:
        fprintf(output,"token %d,tokenval %d ", t, tval);
    }
}

int  main(int __argc,char *__argv[]){
	char * inputfile="input.txt";
		char * outputfile="output.txt";
            printf( "from main ");

	if(__argc==3){
      inputfile=__argv[1];
	  outputfile=__argv[2];

	}
		input = fopen (inputfile, "r" ) ;
            if ( input == NULL )
                {
                    printf ( "Cannot open source file" );
			        system("pause");
                    exit(0);      
                }

			output = fopen ( outputfile,"w" ) ; 
            if ( output == NULL )        
                {           
					printf ( "Cannot open target file" ) ; 
					system("pause");
                   exit(0);               
			}  
      init();
      parse();
      fclose(output);
      fclose(input);

   
	system("pause");
	return 0;
}