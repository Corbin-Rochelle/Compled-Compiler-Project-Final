//*****************************************************************************
// (part 4 example)
// purpose: Node classes used while building a parse tree for the arithmetic expression.
//          Added interpret methods for each node type.
// version: Fall 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ExprNode; 
class TermNode;
class FactorNode;

// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
// <factor> -> ID || INTLIT || ( <expr> )
class FactorNode {
public:
  int _level = 0;                        // recursion level of this node
  virtual float interpret() = 0;         // also pure virtual method
  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
                                         // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
  string* id = nullptr;

  IdNode(int level, string name);
  ~IdNode();
  void printTo(ostream & os);
  float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
  float int_literal = 0;

  IntLitNode(int level, float value);
  ~IntLitNode();
  void printTo(ostream & os);
  float interpret();
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
  ExprNode* exprPtr = nullptr;

  NestedExprNode(int level, ExprNode* en);
  ~NestedExprNode();
  void printTo(ostream & os);
  float interpret();
};
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / )) <factor> }}
class TermNode {
public:
  int _level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;
    
  TermNode(int level);
  ~TermNode();
  float interpret();
};
ostream& operator<<(ostream&, TermNode&); // print operator
// ---------------------------------------------------------------------
// <expr> -> <term> {{ (( + || - )) <term> }}
class ExprNode {
public:
  int _level = 0;          // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
  vector<TermNode*> restTerms;
    
  ExprNode(int level);
  ~ExprNode();
  float interpret();
};
ostream& operator<<(ostream&, ExprNode&); // print operator

#endif /* PARSE_TREE_NODES_H */
