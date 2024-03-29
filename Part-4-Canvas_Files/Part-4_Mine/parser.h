//**************************************************************************
 
// Corbin T. Rochelle

//**************************************************************************

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <iostream>
#include "lexer.h"
#include "parse_tree_nodes.h"

using namespace std;


bool inSymbolTable(string idName);

extern int nextToken;        // next token returned by lexer

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

// Function declarations
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
ExpressionNode* expression();
SimpleExpressionNode* simple_expression();
TermNode* term();
FactorNode* factor();

#endif /* PARSER_H */
