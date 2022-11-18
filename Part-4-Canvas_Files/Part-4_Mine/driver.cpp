//*****************************************************************************
// purpose: Driver for Project Part 2 (parser for TIPS)
// version: Fall 2022
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include "parser.h"
#include "lexer.h"

ostream& operator<<(ostream& os, ProgramNode& pn);
ostream& operator<<(ostream& os, BlockNode& bn);
ostream& operator<<(ostream& os, StatementNode& sn);
ostream& operator<<(ostream& os, ExpressionNode& en);
ostream& operator<<(ostream& os, SimpleExpressionNode& sen);
ostream& operator<<(ostream& os, TermNode& tn);
ostream& operator<<(ostream& os, FactorNode& fn);
//-----------------------------------------------------
ostream& operator<<(ostream& os, ProgramNode& pn) {
    os << endl << indent(pn._level) << "(program ";
    os << *(pn.block);
    os << endl << indent(pn._level) << "program) ";
    return os;
}
ostream& operator<<(ostream& os, BlockNode& bn) {
    os << endl << indent(bn._level) << "(block ";
    os << *(bn.statement);
    os << endl << indent(bn._level)  << "block) ";
    return os;
}
StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& sn) {
    sn.printTo(os);
    return os;
}
ostream& operator<<(ostream& os, ExpressionNode& en) {
    os << endl << indent(en._level) << "(expression ";
    os << *(en.simpleExpression);
    if (en.simpleExpression2 != nullptr) {
        os << endl << indent(en._level) << en.restExpressionOps[0] << " ";
        os << *(en.simpleExpression2);
    }
    os << endl << indent(en._level) << "expression) ";
    return os;
}
ostream& operator<<(ostream& os, SimpleExpressionNode& sen) {
    os << endl << indent(sen._level) << "(simple_exp ";
    os << *(sen.firstTerm);
    int length = sen.restTerms.size();
    for (int i =0; i < length; i++) {
        os << endl << indent(sen._level) << sen.restSimpleExpressionOps[i] << " ";
        os << *(sen.restTerms[i]);
    }
    os << endl << indent(sen._level) << "simple_exp) ";
    return os;
}
ostream& operator<<(ostream& os, TermNode& tn) {
    os << endl << indent(tn._level) << "(term ";
    os << *(tn.firstFactor);
    int length = tn.restFactor.size();
    for (int i =0; i < length; i++) {
        os << endl << indent(tn._level) << tn.restTermOps[i] << " ";
        os << *(tn.restFactor[i]);
    }
    os << endl << indent(tn._level) << "term) ";
    return os;
}
FactorNode::~FactorNode() {}
ostream& operator<<(ostream& os, FactorNode& fn) {
    fn.printTo(os);
    return os;
}
//-----------------------------------------------------

using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

    // Set the input stream
    if (argc > 1) {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
  
    if (!yyin) {
        printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;

    // Set the root node
    ProgramNode* root = nullptr;
    
    // Get the first token
    nextToken = yylex();

    // Fire up the parser!
    try {
        // Process the expected production
        root = program();
        //factor(); // Build bottom-up to <expression>

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near " << yytext << ", error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  
    // Print the saved tree
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;

    // Print out the symbol table
    cout << endl << "*** User Defined Symbols ***" << endl;
    set<string>::iterator it;
    for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
    
    // Delete the tree
    cout << "*** Delete the Tree ***" << endl;
    delete root;
    root = nullptr;
    


    return EXIT_SUCCESS;
}

