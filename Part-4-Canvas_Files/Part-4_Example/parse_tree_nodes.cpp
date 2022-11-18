//*****************************************************************************
// (part 4 example)
// purpose: Node classes used while building a parse tree for the arithmetic expression.
//          Added interpret methods for each node type.
// version: Fall 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#include "parse_tree_nodes.h"

bool printDelete = false;   // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("| ");
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) 
{
  os << endl; indent(fn._level); os << "(factor ";
  fn.printTo(os);
  //os << endl; indent(fn._level); 
  os << "factor) ";
  return os;
}
// Use double dispatch to route to proper interpret method
float FactorNode::interpret() 
{
  return this->interpret();
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) 
{
  _level = level;
  id = new string(name);
}
IdNode::~IdNode() 
{
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode " << endl;
  delete id;
  id = nullptr;
}
void IdNode::printTo(ostream& os) 
{
  os << "(IDENT: " << *id << ") ";
}
float IdNode::interpret(){
  return 1;    // Use 1 for any identifier (in this example only)
               // In your interpreter, look up the current value in 
               //   the symbol table
} 
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, float value) 
{
  _level = level;
  // Although value is a float, and int_literal is also a float, 
  // the lexer will ensure that only integers are stored here.
  int_literal = value;
}
IntLitNode::~IntLitNode() 
{
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode " << endl;
  // Nothing to do since the only members are not pointers
}
void IntLitNode::printTo(ostream& os) 
{
  os << "(INTLIT: " << int_literal << ") ";
}
float IntLitNode::interpret()
{
  return int_literal;   
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) 
{
  _level = level;
  exprPtr = en;
}
NestedExprNode::~NestedExprNode() 
{
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode " << endl;
  delete exprPtr;
  exprPtr = nullptr;
}
void NestedExprNode::printTo(ostream& os) 
{
  os << *exprPtr;
}
float NestedExprNode::interpret()
{
  return exprPtr->interpret();
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) 
{
  _level = level;
}
TermNode::~TermNode() 
{
  if(printDelete)
    cout << "Deleting TermNode " << endl;
  delete firstFactor;
  firstFactor = nullptr;
  
  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    delete restFactors[i];
    restFactors[i] = nullptr;
  }
}
ostream& operator<<(ostream& os, TermNode& tn) 
{
  os << endl; indent(tn._level); os << "(term ";
  os << *(tn.firstFactor);

  int length = tn.restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    int op = tn.restFactorOps[i];
    if (op == TOK_MULT_OP) {
      os << endl; indent(tn._level); os << "* ";
    } else {
      os << endl; indent(tn._level); os << "/ ";
    }
    os << *(tn.restFactors[i]);
  }
  os << endl; indent(tn._level); os << "term) ";
  return os;
}
float TermNode::interpret()
{
  // a Term is a Factor followed by 0 or more Factors separated by * or / operations

  // get the value of the first Factor
  float returnValue = firstFactor->interpret();

  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restFactors[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restFactorOps[i]) {
      case TOK_MULT_OP:
        returnValue = returnValue * nextValue;
      break;
      case TOK_DIV_OP:
        returnValue = returnValue / nextValue;
      break;
    }
  }
  return returnValue;
}
// ---------------------------------------------------------------------
ExprNode::ExprNode(int level) 
{
  _level = level;
}
ExprNode::~ExprNode() 
{
  if(printDelete)
    cout << "Deleting ExprNode " << endl;
  delete firstTerm;
  firstTerm = nullptr;

  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    delete restTerms[i];
    restTerms[i] = nullptr;
  }
}
ostream& operator<<(ostream& os, ExprNode& en) 
{
  os << endl; indent(en._level); os << "(expr ";
  os << *(en.firstTerm);

  int length = en.restTermOps.size();
  for (int i = 0; i < length; ++i) {
    int op = en.restTermOps[i];
    if (op == TOK_ADD_OP) {
      os << endl; indent(en._level); os << "+ ";
    } else {
      os << endl; indent(en._level); os << "- ";
    }
    os << *(en.restTerms[i]);
  }
  os << endl; indent(en._level); os << "expr) ";
  return os;
}
float ExprNode::interpret()
{
  // an Expr is a Term followed by 0 or more Terms separated by + or - operations

  // get the value of the first Term
  float returnValue = firstTerm->interpret();

  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Term
    float nextValue = restTerms[i]->interpret();

    // perform the operation (+ or -) that separates the Terms
    switch(restTermOps[i]) {
      case TOK_ADD_OP:
        returnValue = returnValue + nextValue; 
      break;
      case TOK_SUB_OP:
        returnValue = returnValue - nextValue; 
      break;
    }
  }
  return returnValue;
}
