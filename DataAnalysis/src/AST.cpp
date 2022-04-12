#include "AST.h"
#include "Visitor.h"
#include <iostream>

using namespace std;


ASTNode::ASTNode(const Loc & loc)
:m_loc(loc)
{
}

Stmt::Stmt(const Loc & loc)
:ASTNode(loc)
{
}

StmtInNestedBlock::StmtInNestedBlock(const Loc & loc)
:Stmt(loc)
{ 
}

Expr::Expr(const Loc & loc)
:ASTNode(loc)
{
}

Decl::Decl(const string & name, const Loc & loc)
:ASTNode(loc) , m_name(name)
{ 
}


Identifier::Identifier(const string & name, const Loc & loc)
:Expr(loc) , m_name(name)
{
}

Identifier::~Identifier()
{
}

Identifier * Identifier::Accept(Visitor & v)
{
    return v.visit(this);
}

void Identifier::PrintOut(ostream & os) 
{
    os << m_name;
}

Number::Number(int value, const Loc & loc)
:Expr(loc) , m_value(value)
{ 
}

Number::~Number()
{
}

Number * Number::Accept(Visitor & v)
{
    return v.visit(this);
}

void Number::PrintOut(ostream & os) 
{
    os << m_value;
}

Null::Null(const Loc & loc)
:Expr(loc)
{
}

Null::~Null()
{
}

Null * Null::Accept(Visitor & v)
{
    return v.visit(this);
}

void Null::PrintOut(ostream & os) 
{
    os << "null";
}

BinaryOp::BinaryOp(BinaryOperator * op, Expr * left, Expr * right,const Loc & loc)
:Expr(loc) , m_operator(op) , m_left(left) , m_right(right)
{

}

BinaryOp::~BinaryOp()
{
    delete m_left;
    delete m_right;
}

BinaryOp * BinaryOp::Accept(Visitor & v)
{
    return v.visit(this);
}

void BinaryOp::PrintOut(ostream & os) 
{
    m_left->PrintOut(os);
    os << " ";
    m_operator->PrintOut(os);
    os << " ";
    m_right->PrintOut(os);
}

int Plus::Apply(int left, int right)
{
    return left + right;
}

void Plus::PrintOut(ostream & os) 
{
    os << "+";
}

int Minus::Apply(int left, int right)
{
    return left - right;
}

void Minus::PrintOut(ostream & os) 
{
    os << "-";
}

int Times::Apply(int left, int right)
{
    return left * right;
}

void Times::PrintOut(ostream & os) 
{
    os << "*";
}

int Divide::Apply(int left, int right)
{
    return left / right;
}

void Divide::PrintOut(ostream & os) 
{
    os << "/";
}

int Equal::Apply(int left, int right)
{
    return left == right;
}

void Equal::PrintOut(ostream & os) 
{
    os << "==";
}

int GreaterThan::Apply(int left, int right)
{
    return left > right;
}

void GreaterThan::PrintOut(ostream & os) 
{
    os << ">";
}




CallFuncExpr::CallFuncExpr(Expr * targetFun, const vector<Expr*> arg, const Loc & loc)
:Expr(loc) , m_targetFun(targetFun) , m_arg(arg)
{
}

CallFuncExpr::~CallFuncExpr()
{
    for (uint16_t i = 0; i < m_arg.size(); i++)
        delete m_arg[i];
    delete m_targetFun;
}

CallFuncExpr * CallFuncExpr::Accept(Visitor & v)
{
    return v.visit(this);
}

void CallFuncExpr::PrintOut(ostream & os) 
{
    m_targetFun->PrintOut(os);
    os << "(";
    for (auto & x : m_arg)
    {
        x->PrintOut(os);
        if (x != m_arg.back())
            os << ", ";
    }
    os << ")";
}

Input::Input(const Loc & loc)
:Expr(loc)
{
}

Input::~Input()
{
}

Input * Input::Accept(Visitor & v)
{
    return v.visit(this);
}

void Input::PrintOut(ostream & os) 
{
    os << "input";
}

Alloc::Alloc(Expr * expr, const Loc & loc)
:Expr(loc) , m_expr(expr)
{
}

Alloc::~Alloc()
{
    delete m_expr;
}

Alloc * Alloc::Accept(Visitor & v)
{
    return v.visit(this);
}

void Alloc::PrintOut(ostream & os) 
{
    os << "alloc ";
    m_expr->PrintOut(os);
}

VarRef::VarRef(Identifier * id, const Loc & loc)
:Expr(loc) , m_id(id)
{
}

VarRef::~VarRef()
{
    delete m_id;
}

VarRef * VarRef::Accept(Visitor & v)
{
    return v.visit(this);
}

void VarRef::PrintOut(ostream & os) 
{
    os << "&";
    m_id->PrintOut(os);
}

Deref::Deref(Expr * pointer, const Loc & loc)
:Expr(loc) , m_pointer(pointer)
{
}

Deref::~Deref()
{
    delete m_pointer;
}

Deref * Deref::Accept(Visitor & v)
{
    return v.visit(this);
}

void Deref::PrintOut(ostream & os) 
{
    os << "(*";
    m_pointer->PrintOut(os);
    os << ")";
}

RecordField::RecordField(const string & name, Expr * expr, const Loc & loc)
:ASTNode(loc) , m_name(name) , m_expr(expr)
{
}

RecordField::~RecordField()
{
    delete m_expr;
}

RecordField * RecordField::Accept(Visitor & v)
{
    return v.visit(this);
}

void RecordField::PrintOut(ostream & os) 
{
    os << m_name << ": ";
    m_expr->PrintOut(os);
}

Record::Record(const vector<RecordField*> & fields,const Loc & loc)
:Expr(loc) , m_fields(fields)
{ 
}

Record::~Record()
{
    for (uint16_t i = 0; i < m_fields.size(); i++)
        delete m_fields[i];
}

Record * Record::Accept(Visitor & v)
{
    return v.visit(this);
}

void Record::PrintOut(ostream & os) 
{
    os << "{";
    for (auto & x : m_fields)
    {
        x->PrintOut(os);
        if (x != m_fields.back())
            os << ", ";
    }
    os << "}";
}


FieldAccess::FieldAccess(Expr * record, const string & field, const Loc & loc)
:Expr(loc) , m_record(record) , m_field(field)
{
}

FieldAccess::~FieldAccess()
{
    delete m_record;
}

FieldAccess * FieldAccess::Accept(Visitor & v)
{
    return v.visit(this);
}

void FieldAccess::PrintOut(ostream & os) 
{
    m_record->PrintOut(os);
    os << "." << m_field;
}

AssignStmt::AssignStmt(Expr * left, Expr * right, const Loc & loc)
:StmtInNestedBlock(loc) , m_left(left) , m_right(right)
{
}

AssignStmt::~AssignStmt()
{
    delete m_left;
    delete m_right;
}

AssignStmt * AssignStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void AssignStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' );
    m_left->PrintOut(os);
    os << " = ";
    m_right->PrintOut(os);
    os << ";" << endl;
}

Block::Block(const Loc & loc)
:Stmt(loc)
{
}

NestedBlockStmt::NestedBlockStmt(const vector<StmtInNestedBlock*> & body, const Loc & loc)
:Block(loc) , m_body(body)
{
}

NestedBlockStmt::~NestedBlockStmt()
{
    for (uint16_t i = 0; i < m_body.size(); i++)
        delete m_body[i];
}

NestedBlockStmt * NestedBlockStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void NestedBlockStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "{" << endl;
    for (auto & x : m_body)
    {
        x->PrintOut(os);
    }
    os << string( m_loc.m_col, ' ' ) << "}" << endl;
}

IdentifierDecl::IdentifierDecl(const string & name, const Loc & loc)
:Decl(name,loc)
{ 
}

IdentifierDecl::~IdentifierDecl()
{
}

IdentifierDecl * IdentifierDecl::Accept(Visitor & v)
{
    return v.visit(this);
}

void IdentifierDecl::PrintOut(ostream & os) 
{
    os << m_name;
}

VarStmt::VarStmt(const vector<IdentifierDecl*> & decls, const Loc & loc)
:Stmt(loc) , m_decls(decls)
{
}

VarStmt::~VarStmt()
{
    for (uint16_t i = 0; i < m_decls.size(); i++)
        delete m_decls[i];
}

VarStmt * VarStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void VarStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "var ";
    for (auto & x : m_decls){
        x->PrintOut(os);
        if (x != m_decls.back())
            os << ", ";
        else
            os << ";";
    }
    os << endl;
}

ReturnStmt::ReturnStmt(Expr * expr, const Loc & loc)
:Stmt(loc) , m_expr(expr)
{
}

ReturnStmt::~ReturnStmt()
{
    delete m_expr;
}

ReturnStmt * ReturnStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void ReturnStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "return ";
    m_expr->PrintOut(os);
    os << ";" << endl;
}

FunBlockStmt::FunBlockStmt(const vector<VarStmt*> & vars,
                 const vector<StmtInNestedBlock*> & stmts, 
                 ReturnStmt * ret,const Loc & loc)
:Block(loc) , m_vars(vars) , m_stmts(stmts) , m_ret(ret)
{
}

FunBlockStmt::~FunBlockStmt()
{
    for (uint16_t i = 0; i < m_vars.size(); i++)
        delete m_vars[i];
    for (uint16_t i = 0; i < m_stmts.size(); i++)
        delete m_stmts[i];
    delete m_ret;
}

FunBlockStmt * FunBlockStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void FunBlockStmt::PrintOut(ostream & os) 
{
    os << "{" << endl;
    for (auto & x : m_vars){
        x->PrintOut(os);
    }
    for (auto & x : m_stmts){
        x->PrintOut(os);
    }
    m_ret->PrintOut(os);
    os << "}" << endl;
}

IfStmt::IfStmt(Expr * guard, NestedBlockStmt * thenBranch, 
           NestedBlockStmt * elseBranch,const Loc & loc)
:StmtInNestedBlock(loc) , m_guard(guard) , m_thenBranch(thenBranch) , m_elseBranch(elseBranch)
{ 
}

IfStmt::~IfStmt()
{
    delete m_guard;
    delete m_thenBranch;
    delete m_elseBranch;
}

IfStmt * IfStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void IfStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "if (";
    m_guard->PrintOut(os);
    os << ")" << endl;
    m_thenBranch->PrintOut(os);
    if (m_elseBranch != nullptr){
        os << string( m_loc.m_col, ' ' ) << "else" << endl;
        m_elseBranch->PrintOut(os);
    }
}

WhileStmt::WhileStmt(Expr * guard, NestedBlockStmt * block, const Loc & loc)
:StmtInNestedBlock(loc) , m_guard(guard) , m_block(block)
{
}

WhileStmt::~WhileStmt()
{
    delete m_guard;
    delete m_block;
}

WhileStmt * WhileStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void WhileStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "while (";
    m_guard->PrintOut(os);
    os << ")" << endl;
    m_block->PrintOut(os);
}

OutputStmt::OutputStmt(Expr * expr, const Loc & loc)
:StmtInNestedBlock(loc) , m_expr(expr)
{ 
}

OutputStmt::~OutputStmt()
{
    delete m_expr;
}

OutputStmt * OutputStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

void OutputStmt::PrintOut(ostream & os) 
{
    os << string( m_loc.m_col, ' ' ) << "output ";
    m_expr->PrintOut(os);
    os << ";" << endl;
}

FunDecl::FunDecl(const vector<IdentifierDecl*> params, FunBlockStmt * block,
            const string & name, const Loc & loc)
:Decl(name,loc) , m_params(params) , m_block(block)
{
}

FunDecl::~FunDecl()
{
    for (uint16_t i = 0; i < m_params.size(); i++)
        delete m_params[i];
    delete m_block;
}

FunDecl * FunDecl::Accept(Visitor & v)
{
    return v.visit(this);
}

void FunDecl::PrintOut(ostream & os) 
{
    os << m_name << "(";
    for (auto & x : m_params){
        x->PrintOut(os);
        if (x != m_params.back())
            os << ",";
        else
            os << ")";
    }
    os << endl;
    m_block->PrintOut(os);
}

Program::Program(vector<FunDecl*> & funs,const Loc & loc)
:ASTNode(loc) , m_funs(funs)
{
}

Program::~Program()
{
    for (uint16_t i = 0; i < m_funs.size(); i++)
        delete m_funs[i];
}

Program * Program::Accept(Visitor & v)
{
    return v.visit(this);
}

void Program::PrintOut(ostream & os) 
{
    for (auto & x : m_funs){
        x->PrintOut(os);
    }
}