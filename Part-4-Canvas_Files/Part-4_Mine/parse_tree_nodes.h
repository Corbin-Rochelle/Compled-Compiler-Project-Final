//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Spring 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>


using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

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
ostream& operator<<(ostream& os, AssignmentStmtNode& an);
ostream& operator<<(ostream& os, ExpressionNode& en);
ostream& operator<<(ostream& os, SimpleExpressionNode& sen);
ostream& operator<<(ostream& os, TermNode& tn);
ostream& operator<<(ostream& os, FactorNode& fn);

// ---------------------------------------------------------------------
// <program> -> ID ; <block>
class ProgramNode {
public:
    int _level = 0;          // recursion level of this node
    BlockNode* block = nullptr;

    ProgramNode(int level) { _level = level - 1; }
    ~ProgramNode() {
        cout << "Deleting ProgramNode" << endl;
        delete block;
        block = nullptr;
    }
};
ostream& operator<<(ostream& os, ProgramNode& pn); 
// ---------------------------------------------------------------------
// <block> -> {{ VAR ID : Type ; }} BEGIN {{ <statement> ; }} END
class BlockNode {
public:
    int _level = 0;              // recursion level of this node
    StatementNode* statement = nullptr;

    BlockNode(int level) { _level = level - 1; }
    ~BlockNode() {
        cout << "Deleting BlockNode" << endl;
        delete statement;
        statement = nullptr;
    }
};
ostream& operator<<(ostream& os, BlockNode& bn);
// ---------------------------------------------------------------------
class StatementNode {
public:
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
    
    AssignmentStmtNode(int level) { _level = level - 1; }
    ~AssignmentStmtNode() {
        cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
        delete expression;
        expression = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(assignment_stmt ( " << _var << " := )";
        os << *(expression);
        os << endl << indent(_level) << "assignment_stmt)";
    }
};
// ---------------------------------------------------------------------
// class CompoundStmtNode
class CompoundStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    vector<StatementNode*> restStatements;
    
    CompoundStmtNode(int level) { _level = level - 1; }
    ~CompoundStmtNode() {
        cout << "Deleting StatementNode:CompoundStmtNode" << endl;
        int length = restStatements.size();
        for (int i =0; i < length; i++) {
            delete restStatements[i];
            restStatements[i] = nullptr;
        }
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(compound_stmt";
        int length = restStatements.size();
        for (int i =0; i < length; i++) {
            os << *(restStatements[i]);
        }
        os << endl << indent(_level) << "compound_stmt)";
    }
};
// ---------------------------------------------------------------------
// class IfStmtNode
class IfStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    StatementNode* firstStatement = nullptr;
    StatementNode* secondStatement = nullptr;
    
    IfStmtNode(int level) { _level = level - 1; }
    ~IfStmtNode() {
        cout << "Deleting StatementNode:IfStmtNode" << endl;
        delete expression;
        expression = nullptr;
        delete firstStatement;
        firstStatement = nullptr;
        delete secondStatement;
        secondStatement = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(if_stmt ";
        os << *(expression);
        os << endl << indent(_level) << "(then ";
        os << *(firstStatement);
        os << endl << indent(_level) << "then) ";
        if (secondStatement != nullptr) {
            os << endl << indent(_level) << "(else ";
            os << *(secondStatement);
            os << endl << indent(_level) << "else) ";
        }
        os << endl << indent(_level) << "if_stmt)";
    }
};
// ---------------------------------------------------------------------
// class WhileStmtNode
class WhileStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    StatementNode* statement = nullptr;
    
    WhileStmtNode(int level) { _level = level - 1; }
    ~WhileStmtNode() {
        cout << "Deleting StatementNode:WhileStmtNode" << endl;
        delete expression;
        expression = nullptr;
        delete statement;
        statement = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(while_stmt ";
        os << *(expression);
        os << *(statement);
        os << endl << indent(_level)  << "while_stmt)";
    }
};
// ---------------------------------------------------------------------
// class ReadStmtNode
class ReadStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    ReadStmtNode(int level) { _level = level - 1; }
    ~ReadStmtNode() { cout << "Deleting StatementNode:ReadStmtNode" << endl; }
    void printTo(ostream &os) {
        os << endl << indent(_level)  << "(read_stmt ";
        os << "( " << _input << " )";
        os << endl << indent(_level) << "read_stmt)";
    }
};
// ---------------------------------------------------------------------
// class WriteStmtNode
class WriteStmtNode : public StatementNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    WriteStmtNode(int level) { _level = level - 1; }
    ~WriteStmtNode() { cout << "Deleting StatementNode:WriteStmtNode" << endl; }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(write_stmt ";
        os << "( " << _input << " )";
        os << endl << indent(_level) << "write_stmt)";
    }
};
// ---------------------------------------------------------------------
// <expr> -> {{ (( = || < || > || <> )) <simple_expression> }}
class ExpressionNode {
public:
    int _level = 0;          // recursion level of this node
    SimpleExpressionNode* simpleExpression = nullptr;
    vector<string> restExpressionOps;
    SimpleExpressionNode* simpleExpression2 = nullptr;

    ExpressionNode(int level) { _level = level - 1; }
    ~ExpressionNode() {
        cout << "Deleting ExpressionNode" << endl;
        delete simpleExpression;
        simpleExpression = nullptr;
        delete simpleExpression2;
        simpleExpression2 = nullptr;
    }
};
// ---------------------------------------------------------------------
// <simple_expression> -> <term> {{ (( + || - || OR )) <term> }}
class SimpleExpressionNode {
public:
    int _level = 0;          // recursion level of this node
    TermNode* firstTerm = nullptr;
    vector<string> restSimpleExpressionOps;
    vector<TermNode*> restTerms;

    SimpleExpressionNode(int level) { _level = level - 1; }
    ~SimpleExpressionNode() {
        cout << "Deleting SimpleExpNode" << endl;
        delete firstTerm;
        firstTerm = nullptr;
        int length = restTerms.size();
        for (int i =0; i < length; i++) {
            delete restTerms[i];
            restTerms[i] = nullptr;
        }
    }
  };
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / || AND )) <factor> }}
class TermNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* firstFactor = nullptr;
    vector<string> restTermOps;
    vector<FactorNode*> restFactor;

    TermNode(int level) { _level = level - 1; }
    ~TermNode() {
        cout << "Deleting TermNode" << endl;
        delete firstFactor;
        firstFactor = nullptr;
        int length = restFactor.size();
        for (int i =0; i < length; i++) {
            delete restFactor[i];
            restFactor[i] = nullptr;
        }
    }
  };
// ---------------------------------------------------------------------
class FactorNode {
public:
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
    
    IntNode(int level, string input) {
        _level = level - 1;
        _input = input;
    }
    ~IntNode() { cout << "Deleting FactorNode:IntLitNode" << endl; }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor ";
        os << "( INTLIT: " << _input << " ) ";
        os << endl << indent(_level) << "factor) ";
    };
};
// ---------------------------------------------------------------------
// class FloatNode
class FloatNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    float _input = 0;
    
    FloatNode(int level, float input) {
        _level = level - 1;
        _input = input;
    }
    ~FloatNode() { cout << "Deleting FactorNode:FloatLitNode" << endl; }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor ";
        os << "( FLOATLIT: " << _input << " ) ";
        os << endl << indent(_level) << "factor) ";
    }
};
//---------------------------------------------------------------------
// class FloatNode
class IdentNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    string _input = "";
    
    IdentNode(int level, string input) {
        _level = level - 1;
        _input = input;
    }
    ~IdentNode() { cout << "Deleting FactorNode:IdNode" << endl; }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor ";
        os << "( IDENT: " << _input << " ) ";
        os << endl << indent(_level) << "factor) ";
    }
};
// ---------------------------------------------------------------------
// class NestedExpNode
class NestedExpNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    ExpressionNode* expression = nullptr;
    
    NestedExpNode(int level) { _level = level - 1; }
    ~NestedExpNode() {
        cout << "Deleting FactorNode:NestedExprNode" << endl;
        delete expression;
        expression = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor ( ";
        os << *(expression);
        os << ")" << endl << indent(_level) << "factor) ";
    }
};
// ---------------------------------------------------------------------
// class NotNode
class NotNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* factor = nullptr;
    
    NotNode(int level) { _level = level - 1; }
    ~NotNode() {
        cout << "Deleting FactorNode:NotNode" << endl;
        delete factor;
        factor = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor (NOT ";
        os << *(factor);
        os << ") " << endl << indent(_level) << "factor) ";
    }
};
// ---------------------------------------------------------------------
// class MinusNode
class MinusNode : public FactorNode {
public:
    int _level = 0;          // recursion level of this node
    FactorNode* factor = nullptr;
    
    MinusNode(int level) { _level = level - 1; }
    ~MinusNode() {
        cout << "Deleting FactorNode:MinusNode" << endl;
        delete factor;
        factor = nullptr;
    }
    void printTo(ostream &os) {
        os << endl << indent(_level) << "(factor (- ";
        os << *(factor);
        os << ") " << endl << indent(_level) << "factor) ";
    }
};

#endif /* PARSE_TREE_NODES_H */
