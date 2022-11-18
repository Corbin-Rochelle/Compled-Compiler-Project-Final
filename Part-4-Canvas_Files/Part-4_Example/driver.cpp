//*****************************************************************************
// (part 4 example)
// purpose: driver file for interpreter example
// version: Spring 2022
// author: Joe Crumpton / Ed Swan
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;   // input stream
  extern FILE *yyout;  // output stream
  extern int yyleng;   // length of current lexeme
  extern char *yytext; // text of current lexeme
  extern int yylex();  // the generated lexical analyzer
  extern int yylex_destroy(); // deletes memory allocated by yylex
}

int main( int argc, char* argv[] )
{
  // Whether to print these items
  bool printTree = false; 
  bool printSymbolTable = false;

  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true;
    }
    // -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }

  // Open the input data file and process its contents 
  if ((yyin = fopen("front.in", "r")) == NULL) {
    cout << "ERROR - cannot open front.in" << endl;
    return(EXIT_FAILURE);
  }
  // If uncommented, this line allows typing in an expression interactively
  // If so, to end must type "Ctrl-D" (generate EOF character)
  yyin = stdin;

  // Create the root of the parse tree
  ExprNode* root = nullptr;

  lex();  // prime the pump (get first token)
  do {
    root = expr(); // start symbol is <expr>
  } while(nextToken != TOK_EOF);

  if (yyin)
    fclose(yyin);
    
  yylex_destroy();

  // Printing, Interpreting, and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  if(printTree) {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }

  cout << "*** Interpret the Tree ***" << endl;
  cout << root->interpret() << endl << endl;

  if(printSymbolTable)
  {
    cout << "*** Print the Symbol Table ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it )
      cout << setw(8) << it->first << ": " << it->second << endl;
  }
  
  if(printDelete)
    cout << "*** Delete the Tree ***" << endl;
  delete root;
  root = nullptr;
    
  return(EXIT_SUCCESS);
}
