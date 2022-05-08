//compiler Project 1 
//By : Raghad AbuSharar 181044
    // Shahd Madhoun 181064
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<curses.h>

//functions declerations
int lookup(char s[]);
int insert(char s[],  int tok);

void parse();
//void expr();
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



//size of lexemes array 
#define STRMAX 999

//size of symtable
#define SYMMAX 100 

// buffer size
#define BSIZE 128 

//identefiers declerations
#define NUM 256
#define NONE -1
#define EOS '\0'

//operations
#define DIV 257
#define MOD 258
#define ID 259

//KeyWords
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




//value of token attribute 
int tokenval;
int lineno;

// input && output files declerations
FILE *input, *output ;

char ids[] ;
char *dataType = "int ";

//form of symbol table entry
struct entry { 
    char *lexptr;
    int token;
};

//symbol table 
struct entry symtable[]; 

//last used position in lexemes 
char lexemes [STRMAX];
int lastchar = -1; 

//last used position in symtable
struct entry symtable[SYMMAX];
int lastentry = 0; 


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


//returns position of entry for s 
int lookup(char s[])     
{
    int p;
    for (p = lastentry; p > 0; p = p - 1)
		if (strcmp( symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}

//returns position of entry for s 
int insert(char s[],  int tok) 
{
    int len;
    len = strlen(s); 
    // strlen computes length of s 
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


int lookahead;

//parses and translates expression list 
void parse()
{ 
   lookahead = lexan();
	match(PROGRAM);//or header function 
   // emit(ID, tokenval);
    match(ID);//the name of the project 
	match('(');
	match(INPUT);
	match(',');
	match(OUTPUT);
	match(')');
	match(';');
	fprintf(output,"#");
	fprintf(output,"include");
	fprintf(output,"<");
	fprintf(output,"iostream");
	fprintf(output,">\n");
	fprintf(output,"using ");
	fprintf(output,"namespace ");
	fprintf(output,"std");
	fprintf(output,";\n");
	Declarations();
	 fprintf(output,"int ");
	 fprintf(output,"main ");
	 fprintf(output,"(");
	 fprintf(output,"void");
	 fprintf(output,")");
	  
	        printf("for parse two \n");
	    	printf( "%d",lookahead);
	        printf("\n");
	   Block();
}
void Declarations(){
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
    printf( "\n ConstantDefinitions here");
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
    printf( "\n ConstantDefinition here");
    emit(ID, tokenval);
    match(ID);
    match('=');
    fprintf(output,"=");
    emit(NUM, tokenval);
    match(NUM);
    fprintf(output,";\n");

}

void VariableDeclarations() {
    printf( "\n VariableDeclarations here");
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
    fprintf(output, dataType);
    IdentifierList();
    match(':');
    //fprintf(output,";");
    Type();
    //printf(output,"bbb %s ", ids[0]);
    size_t len = strlen(ids);
    for(int i =0 ; i < len ;i++)
   	//fprintf(output,"%s ", ids[i]);
   	emit(ID , ids[i]);
    match(';');
    fprintf(output,";\n");
    
}


void IdentifierList(){
    printf( "\n IdentifierList here");

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
    //int t;
		switch (lookahead) {
		case INTEGER: case REAL : case CHAR: case BOOLEAN:
			emit(lookahead, NONE);
			match(lookahead);
        default:
            //error("syntax error in type");
            return;
		} 
}


void Block(){
    printf( "\n Block here");
    match(BEGIN);
    fprintf(output,"{\n");
    Statements();
    match(END);
    fprintf(output,"\n}");
}

void Statements(){
    printf( "\n Statements here");
    Statement();
    //fprintf(output, lookahead);
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
    printf( "\n Statement here");
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
    printf( "\n ElseClause here");

    switch (lookahead) {
    case ELSE :
     emit(lookahead,NONE);
      match(ELSE);
      Statement();
      break;
    case EPSILON:
    default:
        return; 
    }
}


void ExpressionList(){
            printf( "\n ExpressionList here");

    Expression();
    while(1)
    switch (lookahead) {
    case ',' :
      match(',');
      Expression();
      break;
    case EPSILON:
    default:
    return; 
    }
} 

void Expression(){
        printf( "\n Expression here");

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


void SimpleExpression() { // nested switch or if ? 
    printf( "\n simpleExpression here");
    switch (lookahead) {
    case  ID : case NUM : case '(': case NOT:
        Term();
        while(1)
        if (lookahead == '+' ||lookahead == '-'||lookahead ==OR)
            {
           emit(lookahead,NONE);
		   match(lookahead);
            Term();
			}
        else 
        return ; 
    case '+': case '-': case OR :
    	
       emit(lookahead,NONE);
	   match(lookahead);
       Term();
        
        while(1)
        if (lookahead == '+' ||lookahead == '-'||lookahead ==OR)
            {
           emit(lookahead,NONE);
		   match(lookahead);
            Term();
			}
        else 
           return ;
  
    }
    
}

void Term(){
    printf( "\n term here");
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
    printf( "\n factor here");
    switch (lookahead) {
        case ID:
            emit(ID, tokenval);
            match(ID);
            break;
        case NUM:
        printf( "\n  hereeee");
            emit(NUM, tokenval);
            match(NUM);
            printf( "\n  hereee");
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
    printf( "\n  lexan hereee");

    int t;
    while (1) {
        printf( "\n  lexan 2 hereee");
        t = fgetc(input);
        if (t == '#') { printf( "\n  lexan 3 hereee");
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
            printf( "\n  lexan 4 hereee");
            return NUM;
        }
        else if (isalpha(t)) {  
            printf( "\n  lexan 5 hereee");
            int p, b = 0;
            while (isalnum(t)) { 
                lexbuf[b] = t;
                t = fgetc(input);
                b = b + 1;
                //printf( "\n  lexan 6 hereee");
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
            printf( "\n  lexan 7 hereee");
            printf( "\n test" + symtable[p].token);
            return symtable[p].token;
        }
		else if (t == EOF){
			printf("word");
			printf( "\n  lexan 8 hereee");
			return END;
			
		}
        else {
            tokenval = NONE;
            printf( "\n  lexan 9 hereee");
            return t;
        }
        printf( "\n  lexan 10 hereee");

    }
    printf( "\n  lexan 11 hereee");
}
//loads keywords into symtable
void init() 
{
    printf( "\n  init hereee");
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
		//fprintf(output,"int "); 
		break;
    case REAL:
        dataType = "real ";
		//fprintf(output,"float ");
		break;
    case CHAR:
        dataType = "char ";
		//fprintf(output,"char "); 
		break;
	case BOOLEAN:
        dataType = "bool ";
		//fprintf(output,"bool "); 
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
            printf( "\n  main 1 hereee");

	if(__argc==3){
      inputfile=__argv[1];
	  outputfile=__argv[2];
	  printf( "\n  main 2 hereee");

	}
		input = fopen (inputfile, "r" ) ;
            if ( input == NULL )
                {    printf( "\n  main 3 hereee");
                    puts ( "Cannot open source file" ) ;
			system("pause");
                exit(0 ) ;      
                }

			output = fopen ( outputfile,"w" ) ; 
            if ( output == NULL )        
                {           
                    printf( "\n  main 4 hereee");
					puts ( "Cannot open target file" ) ; 
					system("pause");
                   exit(0 ) ;               
			}  
			printf( "\n  main 5 hereee");
      init();
      printf( "\n  main 6 hereee");
      parse();
      fclose(output);
      fclose(input);

   
	system("pause");
	return 0;
}