
//*****************************************************************************
// (part 3)
// purpose: node classes used while building a parse tree for
//              the arithmetic expression
// version: Fall 2022
//  author: Joe Crumpton / Ed Swan
//*****************************************************************************

#include "parse_tree_nodes.h"

bool printDelete = false;   // shall we print deleting the tree?

symbolTableT symbolTable;
// Determine if a symbol is in the symbol table
bool inSymbolTable(string idName) {
  symbolTableT::iterator it;
  it = symbolTable.find( idName );
  // If idName is missing, will be set to the end
  return !(it == symbolTable.end());
}

// ---------------------------------------------------------------------
// <program> -> ID ; <block>
ProgramNode::ProgramNode(int level) { _level = level - 1; }
ProgramNode::~ProgramNode() {
    cout << "Deleting ProgramNode" << endl;
    delete block;
    block = nullptr;
}
void ProgramNode::interpret() {
        block->interpret();
}
// ---------------------------------------------------------------------
// <block> -> {{ VAR ID : Type ; }} BEGIN {{ <statement> ; }} END
BlockNode::BlockNode(int level) { _level = level - 1; }
BlockNode::~BlockNode() {
    cout << "Deleting BlockNode" << endl;
    delete statement;
    statement = nullptr;
}
void BlockNode::interpret() {
    statement->interpret();
}
// ---------------------------------------------------------------------
AssignmentStmtNode::AssignmentStmtNode(int level) { _level = level - 1; }
AssignmentStmtNode::~AssignmentStmtNode() {
    cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
    delete expression;
    expression = nullptr;
}
void AssignmentStmtNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(assignment_stmt ( " << _var << " := )";
    os << *(expression);
    os << endl << indent(_level) << "assignment_stmt)";
}
void AssignmentStmtNode::interpret() {
    float varValue = expression->interpret();
    symbolTable[_var] = varValue;
}
// ---------------------------------------------------------------------
// class CompoundStmtNode
CompoundStmtNode::CompoundStmtNode(int level) { _level = level - 1; }
CompoundStmtNode::~CompoundStmtNode() {
    cout << "Deleting StatementNode:CompoundStmtNode" << endl;
    int length = restStatements.size();
    for (int i =0; i < length; i++) {
        delete restStatements[i];
        restStatements[i] = nullptr;
    }
}
void CompoundStmtNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(compound_stmt";
    int length = restStatements.size();
    for (int i =0; i < length; i++) {
        os << *(restStatements[i]);
    }
    os << endl << indent(_level) << "compound_stmt)";
}
void CompoundStmtNode::interpret() {
    int length = restStatements.size();
    for (int i = 0; i < length; ++i) {
        restStatements[i]->interpret();
    }
}
// ---------------------------------------------------------------------
// class IfStmtNode
IfStmtNode::IfStmtNode(int level) { _level = level - 1; }
IfStmtNode::~IfStmtNode() {
    cout << "Deleting StatementNode:IfStmtNode" << endl;
    delete expression;
    expression = nullptr;
    delete firstStatement;
    firstStatement = nullptr;
    delete secondStatement;
    secondStatement = nullptr;
}
void IfStmtNode::printTo(ostream &os) {
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
void IfStmtNode::interpret() {
    if(expression->interpret()) firstStatement->interpret();
    else if(secondStatement != nullptr) secondStatement->interpret();
}
// ---------------------------------------------------------------------
// class WhileStmtNode
WhileStmtNode::WhileStmtNode(int level) { _level = level - 1; }
WhileStmtNode::~WhileStmtNode() {
    cout << "Deleting StatementNode:WhileStmtNode" << endl;
    delete expression;
    expression = nullptr;
    delete statement;
    statement = nullptr;
}
void WhileStmtNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(while_stmt ";
    os << *(expression);
    os << *(statement);
    os << endl << indent(_level)  << "while_stmt)";
}
void WhileStmtNode::interpret() {
    while (expression->interpret()) {
        statement->interpret();
    }
}
// ---------------------------------------------------------------------
// class ReadStmtNode
ReadStmtNode::ReadStmtNode(int level) { _level = level - 1; }
ReadStmtNode::~ReadStmtNode() { cout << "Deleting StatementNode:ReadStmtNode" << endl; }
void ReadStmtNode::printTo(ostream &os) {
    os << endl << indent(_level)  << "(read_stmt ";
    os << "( " << _input << " )";
    os << endl << indent(_level) << "read_stmt)";
}
void ReadStmtNode::interpret() {
    float user_input;
    cin >> user_input;

    if (!inSymbolTable(_input)) {
        throw("Symbol Not Found");
    }
    symbolTable[_input] = user_input;
}
// ---------------------------------------------------------------------
// class WriteStmtNode
WriteStmtNode::WriteStmtNode(int level) { _level = level - 1; }
WriteStmtNode::~WriteStmtNode() { cout << "Deleting StatementNode:WriteStmtNode" << endl; }
void WriteStmtNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(write_stmt ";
    os << "( " << _input << " )";
    os << endl << indent(_level) << "write_stmt)";
}
void WriteStmtNode::interpret() {
    if(_input[0] == '\'' && _input[_input.size()-1] == '\'' && _input.size() > 1) {
        cout << _input.substr(1, _input.size() - 2) << endl;
    }
    else cout << symbolTable[_input] << endl;
}
// ---------------------------------------------------------------------
// <expr> -> {{ (( = || < || > || <> )) <simple_expression> }}
ExpressionNode::ExpressionNode(int level) { _level = level - 1; }
ExpressionNode::~ExpressionNode() {
    cout << "Deleting ExpressionNode" << endl;
    delete simpleExpression;
    simpleExpression = nullptr;
    delete simpleExpression2;
    simpleExpression2 = nullptr;
}
float ExpressionNode::interpret() {
    float returnValue = 0;
    float check1 = simpleExpression->interpret();

    if (simpleExpression2 != nullptr) {
        if(restExpressionOps[0].compare("=") == 0) {
            if(!truth(check1-simpleExpression2->interpret())) returnValue = 1;
        }
        else if(restExpressionOps[0].compare("<") == 0) {
            //cout << "im here" << endl;
            if(check1 < simpleExpression2->interpret()) returnValue = 1;
        }
        else if(restExpressionOps[0].compare(">") == 0) {
            if(check1 > simpleExpression2->interpret()) returnValue = 1;
        }
        else if(restExpressionOps[0].compare("<>") == 0) {
            if(truth(check1-simpleExpression2->interpret())) returnValue = 1;
        }
    }
    if (simpleExpression2 == nullptr) return check1;
    return returnValue;
}
// ---------------------------------------------------------------------
// <simple_expression> -> <term> {{ (( + || - || OR )) <term> }}
SimpleExpressionNode::SimpleExpressionNode(int level) { _level = level - 1; }
SimpleExpressionNode::~SimpleExpressionNode() {
    cout << "Deleting SimpleExpNode" << endl;
    delete firstTerm;
    firstTerm = nullptr;
    int length = restTerms.size();
    for (int i =0; i < length; i++) {
        delete restTerms[i];
        restTerms[i] = nullptr;
    }
}
float SimpleExpressionNode::interpret() {
    float returnValue = firstTerm->interpret();

    for (int i = 0; i < restTerms.size(); i++) {
        if(restSimpleExpressionOps[i] == "+") returnValue = returnValue + restTerms[i]->interpret();
        else if(restSimpleExpressionOps[i] == "-") returnValue = returnValue - restTerms[i]->interpret();
        else if(restSimpleExpressionOps[i] == "OR") returnValue = returnValue || restTerms[i]->interpret();;
    }

    return returnValue;
}
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / || AND )) <factor> }}
TermNode::TermNode(int level) { _level = level - 1; }
TermNode::~TermNode() {
    cout << "Deleting TermNode" << endl;
    delete firstFactor;
    firstFactor = nullptr;
    int length = restFactor.size();
    for (int i =0; i < length; i++) {
        delete restFactor[i];
        restFactor[i] = nullptr;
    }
}
float TermNode::interpret() {
    float returnValue = firstFactor->interpret();

    for (int i = 0; i < restTermOps.size(); i++) {
        if(restTermOps[i] == "*") returnValue = returnValue * restFactor[i]->interpret();
        else if(restTermOps[i] == "/") returnValue = returnValue / restFactor[i]->interpret();
        else if(restTermOps[i] == "AND") returnValue = returnValue && restFactor[i]->interpret();
    }

    return returnValue;
}
// ---------------------------------------------------------------------
// class IntNode
IntNode::IntNode(int level, string input) {
    _level = level - 1;
    _input = input;
}
IntNode::~IntNode() { cout << "Deleting FactorNode:IntLitNode" << endl; }
void IntNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor ";
    os << "( INTLIT: " << _input << " ) ";
    os << endl << indent(_level) << "factor) ";
}
float IntNode::interpret() {
    return stof(_input);
}
// ---------------------------------------------------------------------
// class FloatNode
FloatNode::FloatNode(int level, float input) {
    _level = level - 1;
    _input = input;
}
FloatNode::~FloatNode() { cout << "Deleting FactorNode:FloatLitNode" << endl; }
void FloatNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor ";
    os << "( FLOATLIT: " << _input << " ) ";
    os << endl << indent(_level) << "factor) ";
}
float FloatNode::interpret() {
    return _input;
}
//---------------------------------------------------------------------
// class FloatNode
IdentNode::IdentNode(int level, string input) {
    _level = level - 1;
    _input = input;
}
IdentNode::~IdentNode() { cout << "Deleting FactorNode:IdNode" << endl; }
void IdentNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor ";
    os << "( IDENT: " << _input << " ) ";
    os << endl << indent(_level) << "factor) ";
}
float IdentNode::interpret() {
    return symbolTable[_input];
}
// ---------------------------------------------------------------------
// class NestedExpNode
NestedExpNode::NestedExpNode(int level) { _level = level - 1; }
NestedExpNode::~NestedExpNode() {
    cout << "Deleting FactorNode:NestedExprNode" << endl;
    delete expression;
    expression = nullptr;
}
void NestedExpNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor ( ";
    os << *(expression);
    os << ")" << endl << indent(_level) << "factor) ";
}
float NestedExpNode::interpret() {
    return expression->interpret();
}
// ---------------------------------------------------------------------
// class NotNode
NotNode::NotNode(int level) { _level = level - 1; }
NotNode::~NotNode() {
    cout << "Deleting FactorNode:NotNode" << endl;
    delete factor;
    factor = nullptr;
}
void NotNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor (NOT ";
    os << *(factor);
    os << ") " << endl << indent(_level) << "factor) ";
}
float NotNode::interpret() {
    if(factor->interpret()) return 0;
    else return 1;
}
// ---------------------------------------------------------------------
// class MinusNode
MinusNode::MinusNode(int level) { _level = level - 1; }
MinusNode::~MinusNode() {
    cout << "Deleting FactorNode:MinusNode" << endl;
    delete factor;
    factor = nullptr;
}
void MinusNode::printTo(ostream &os) {
    os << endl << indent(_level) << "(factor (- ";
    os << *(factor);
    os << ") " << endl << indent(_level) << "factor) ";
}
float MinusNode::interpret() {
    return -1 * factor->interpret();
}


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