#include "AST.h"
#include "Visitor.h"
#include "Memory.h"
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

Object * Identifier::Accept(Visitor & v)
{
    return v.visit(this);
}

Number::Number(int value, const Loc & loc)
:Expr(loc) , m_value(value)
{ 
}

Number::~Number()
{
}

Object * Number::Accept(Visitor & v)
{
    return v.visit(this);
}

Null::Null(const Loc & loc)
:Expr(loc)
{
}

Null::~Null()
{
}

Object * Null::Accept(Visitor & v)
{
    return v.visit(this);
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

Object * BinaryOp::Accept(Visitor & v)
{
    return v.visit(this);
}

int Plus::Apply(int left, int right)
{
    return left + right;
}

int Minus::Apply(int left, int right)
{
    return left - right;
}

int Times::Apply(int left, int right)
{
    return left * right;
}

int Divide::Apply(int left, int right)
{
    return left / right;
}

int Equal::Apply(int left, int right)
{
    return left == right;
}

int GreaterThan::Apply(int left, int right)
{
    return left > right;
}




CallFuncExpr::CallFuncExpr(Expr * targetFun, const vector<Expr*> arg, const Loc & loc)
:Expr(loc) , m_targetFun(targetFun) , m_arg(arg)
{
}

CallFuncExpr::~CallFuncExpr()
{
    for (uint i = 0; i < m_arg.size(); i++)
        delete m_arg[i];
    delete m_targetFun;
}

Object * CallFuncExpr::Accept(Visitor & v)
{
    return v.visit(this);
}

Input::Input(const Loc & loc)
:Expr(loc)
{
}

Input::~Input()
{
}

Object * Input::Accept(Visitor & v)
{
    return v.visit(this);
}

Alloc::Alloc(Expr * expr, const Loc & loc)
:Expr(loc) , m_expr(expr)
{
}

Alloc::~Alloc()
{
    delete m_expr;
}

Object * Alloc::Accept(Visitor & v)
{
    return v.visit(this);
}

VarRef::VarRef(Identifier * id, const Loc & loc)
:Expr(loc) , m_id(id)
{
}

VarRef::~VarRef()
{
    delete m_id;
}

Object * VarRef::Accept(Visitor & v)
{
    return v.visit(this);
}

Deref::Deref(Expr * pointer, const Loc & loc)
:Expr(loc) , m_pointer(pointer)
{
}

Deref::~Deref()
{
    delete m_pointer;
}

Object * Deref::Accept(Visitor & v)
{
    return v.visit(this);
}

RecordField::RecordField(const string & name, Expr * expr, const Loc & loc)
:ASTNode(loc) , m_name(name) , m_expr(expr)
{
}

RecordField::~RecordField()
{
    delete m_expr;
}

Object * RecordField::Accept(Visitor & v)
{
    return v.visit(this);
}

Record::Record(const vector<RecordField*> & fields,const Loc & loc)
:Expr(loc) , m_fields(fields)
{ 
}

Record::~Record()
{
    for (uint i = 0; i < m_fields.size(); i++)
        delete m_fields[i];
}

Object * Record::Accept(Visitor & v)
{
    return v.visit(this);
}


FieldAccess::FieldAccess(Expr * record, const string & field, const Loc & loc)
:Expr(loc) , m_record(record) , m_field(field)
{
}

FieldAccess::~FieldAccess()
{
    delete m_record;
}

Object * FieldAccess::Accept(Visitor & v)
{
    return v.visit(this);
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

Object * AssignStmt::Accept(Visitor & v)
{
    return v.visit(this);
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
    for (uint i = 0; i < m_body.size(); i++)
        delete m_body[i];
}

Object * NestedBlockStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

IdentifierDecl::IdentifierDecl(const string & name, const Loc & loc)
:Decl(name,loc)
{ 
}

IdentifierDecl::~IdentifierDecl()
{
}

Object * IdentifierDecl::Accept(Visitor & v)
{
    return v.visit(this);
}

VarStmt::VarStmt(const vector<IdentifierDecl*> & decls, const Loc & loc)
:Stmt(loc) , m_decls(decls)
{
}

VarStmt::~VarStmt()
{
    for (uint i = 0; i < m_decls.size(); i++)
        delete m_decls[i];
}

Object * VarStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

ReturnStmt::ReturnStmt(Expr * expr, const Loc & loc)
:Stmt(loc) , m_expr(expr)
{
}

ReturnStmt::~ReturnStmt()
{
    delete m_expr;
}

Object * ReturnStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

FunBlockStmt::FunBlockStmt(const vector<VarStmt*> & vars,
                 const vector<StmtInNestedBlock*> & stmts, 
                 ReturnStmt * ret,const Loc & loc)
:Block(loc) , m_vars(vars) , m_stmts(stmts) , m_ret(ret)
{
}

FunBlockStmt::~FunBlockStmt()
{
    for (uint i = 0; i < m_vars.size(); i++)
        delete m_vars[i];
    for (uint i = 0; i < m_stmts.size(); i++)
        delete m_stmts[i];
    delete m_ret;
}

Object * FunBlockStmt::Accept(Visitor & v)
{
    return v.visit(this);
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

Object * IfStmt::Accept(Visitor & v)
{
    return v.visit(this);
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

Object * WhileStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

OutputStmt::OutputStmt(Expr * expr, const Loc & loc)
:StmtInNestedBlock(loc) , m_expr(expr)
{ 
}

OutputStmt::~OutputStmt()
{
    delete m_expr;
}

Object * OutputStmt::Accept(Visitor & v)
{
    return v.visit(this);
}

FunDecl::FunDecl(const vector<IdentifierDecl*> params, FunBlockStmt * block,
            const string & name, const Loc & loc)
:Decl(name,loc) , m_params(params) , m_block(block)
{
}

FunDecl::~FunDecl()
{
    for (uint i = 0; i < m_params.size(); i++)
        delete m_params[i];
    delete m_block;
}

Object * FunDecl::Accept(Visitor & v)
{
    return v.visit(this);
}

Program::Program(vector<FunDecl*> & funs,const Loc & loc)
:ASTNode(loc) , m_funs(funs)
{
}

Program::~Program()
{
    for (uint i = 0; i < m_funs.size(); i++)
        delete m_funs[i];
}

Object * Program::Accept(Visitor & v)
{
    return v.visit(this);
}