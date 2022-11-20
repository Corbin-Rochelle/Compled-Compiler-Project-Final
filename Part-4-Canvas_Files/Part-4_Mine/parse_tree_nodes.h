//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Spring 2022
//  author: Corbin T. Rochelle
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// Holds the symbols in the interpreted programm
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

#define EPSILON 0.001
static bool truth(float F) {
return !((EPSILON > F) && (F > -EPSILON));
}

static string indent(int level) {
    string output = "";
  for (int i = 0; i < level; i++)
    output = output + "|  ";
    return output;
}

// ---------------------------------------------------------------------
// Forward declaration of node types
class ProgramNode;
class BlockNode;
class StatementNode;
class AssignmentStmtNode;
class CompoundStmtNode;
class IfStmtNode;
class WhileStmtNode;
class ReadStmtNode;
class WriteStmtNode;
class ExpressionNode;
class SimpleExpressionNode;
class TermNode;
class FactorNode;
class IntNode;
class FloatNode;
class IdNode;
class NestedExpNode;
class NotNode;
class MinusNode;

ostream& operator<<(ostream& os, ProgramNode& pn);
ostream& operator<<(ostream& os, BlockNode& bn);
ostream& operator<<(ostream& os, StatementNode& sn);
ostream& operator<<(ostream& os, ExpressionNode& en);
ostream& operator<<(ostream& os, SimpleExpressionNode& sen);
ostream& operator<<(ostream& os, TermNode& tn);
ostream& operator<<(ostream& os, FactorNode& fn);

// <program> -> ID ; <block>
class ProgramNode {
public:
    int _level = 0;          // recursion level of this node
    BlockNode* block = nullptr;

    ProgramNode(int level);
    ~ProgramNode();
    void interpret();
};
ostream& operator<<(ostream& os, ProgramNode& pn); 
// ---------------------------------------------------------------------
// <block> -> {{ VAR ID : Type ; }} BEGIN {{ <statement> ; }} END
class BlockNode {
public:
    int _level = 0;              // recursion level of this node
    StatementNode* statement = nullptr;

    BlockNode(int level);
    ~BlockNode();
    void interpret();
};
ostream& operator<<(ostream& os, BlockNode& bn);
// ---------------------------------------------------------------------
class StatementNode {
public:
    virtual void interpret() = 0;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~StatementNode();                   // labeling the destructor as virtual allows
                                         // the subclass destructors to be called
};
ostream& operator<<(ostream& os, StatementNode& sn);
// ---------------------------------------------------------------------
// class AssignmentStmtNode
class AssignmentStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    string _var = "";
    ExpressionNode* expression = nullptr;
    
    AssignmentStmtNode(int level);
    ~AssignmentStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// class CompoundStmtNode
class CompoundStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    vector<StatementNode*> restStatements;
    
    CompoundStmtNode(int level);
    ~CompoundStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// class IfStmtNode
class IfStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    StatementNode* firstStatement = nullptr;
    StatementNode* secondStatement = nullptr;
    
    IfStmtNode(int level);
    ~IfStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// class WhileStmtNode
class WhileStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    StatementNode* statement = nullptr;
    
    WhileStmtNode(int level);
    ~WhileStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// class ReadStmtNode
class ReadStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    ReadStmtNode(int level);
    ~ReadStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// class WriteStmtNode
class WriteStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    WriteStmtNode(int level);
    ~WriteStmtNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
// <expr> -> {{ (( = || < || > || <> )) <simple_expression> }}
class ExpressionNode {
public:
    int _level = 0;          // recursion level of this node
    SimpleExpressionNode* simpleExpression = nullptr;
    vector<string> restExpressionOps;
    SimpleExpressionNode* simpleExpression2 = nullptr;

    ExpressionNode(int level);
    ~ExpressionNode();
    float interpret();
};
// ---------------------------------------------------------------------
// <simple_expression> -> <term> {{ (( + || - || OR )) <term> }}
class SimpleExpressionNode {
public:
    int _level = 0;          // recursion level of this node
    TermNode* firstTerm = nullptr;
    vector<string> restSimpleExpressionOps;
    vector<TermNode*> restTerms;

    SimpleExpressionNode(int level);
    ~SimpleExpressionNode();
    float interpret();
  };
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / || AND )) <factor> }}
class TermNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* firstFactor = nullptr;
    vector<string> restTermOps;
    vector<FactorNode*> restFactor;

    TermNode(int level);
    ~TermNode();
    float interpret();
  };
// ---------------------------------------------------------------------
class FactorNode {
public:
    virtual float interpret() = 0;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode();                   // labeling the destructor as virtual allows
                                         // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IntNode
class IntNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    IntNode(int level, string input);
    ~IntNode();
    void printTo(ostream &os);
    float interpret();
};
// ---------------------------------------------------------------------
// class FloatNode
class FloatNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    float _input = 0;
    
    FloatNode(int level, float input);
    ~FloatNode();
    void printTo(ostream &os);
    float interpret();
};
//---------------------------------------------------------------------
// class FloatNode
class IdentNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    IdentNode(int level, string input);
    ~IdentNode();
    void printTo(ostream &os);
    float interpret();
};
// ---------------------------------------------------------------------
// class NestedExpNode
class NestedExpNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    
    NestedExpNode(int level);
    ~NestedExpNode();
    void printTo(ostream &os);
    float interpret();
};
// ---------------------------------------------------------------------
// class NotNode
class NotNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* factor = nullptr;
    
    NotNode(int level);
    ~NotNode();
    void printTo(ostream &os);
    float interpret();
};
// ---------------------------------------------------------------------
// class MinusNode
class MinusNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* factor = nullptr;
    
    MinusNode(int level);
    ~MinusNode();
    void printTo(ostream &os);
    float interpret();
};

#endif /* PARSE_TREE_NODES_H */