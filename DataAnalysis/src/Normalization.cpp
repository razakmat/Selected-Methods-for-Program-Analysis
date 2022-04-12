#include "Normalization.hpp"
#include "AST.h"

using namespace std;

Normalization::Normalization() 
{
    m_idName = 1;
    m_currentName = 1;
    m_col = 0;
}

void Normalization::CheckAndCreateNewExpr(Expr *& e)
{
    if (!m_SExp)
    {
        m_newExpr.push_back(e);
        e = new Identifier("_t" + to_string(m_idName),{0,m_col});
        m_newVar.push_back("_t" + to_string(m_idName++));
    }
}

void Normalization::AddNewExprs(vector<StmtInNestedBlock*> & stmts)
{
    for (auto & x : m_newExpr)
    {
        stmts.push_back(new AssignStmt(new Identifier("_t"+ to_string(m_currentName++),{0,m_col}),x,{0,m_col}));
    }
    m_newExpr.clear();
}

Number * Normalization::visit(Number * integer) 
{
    m_SExp = true;
    return new Number(integer->m_value,{0,m_col});
}

Null * Normalization::visit(Null * null) 
{
    m_SExp = true;
    return new Null({0,m_col});
}

Identifier * Normalization::visit(Identifier * id) 
{
    m_SExp = true;
    return new Identifier(id->m_name,{0,m_col});
}

BinaryOp * Normalization::visit(BinaryOp * Binop) 
{
    Expr * left = Binop->m_left->Accept(*this);
    CheckAndCreateNewExpr(left);
    Expr * right = Binop->m_right->Accept(*this);
    CheckAndCreateNewExpr(right);
    m_SExp = false;
    return new BinaryOp(Binop->m_operator,left,right,{0,m_col});
}

CallFuncExpr * Normalization::visit(CallFuncExpr * call) 
{
    Expr * target = call->m_targetFun->Accept(*this);
    CheckAndCreateNewExpr(target);
    vector<Expr *> arg;
    for (auto & x : call->m_arg)
    {
        Expr * e = x->Accept(*this);
        CheckAndCreateNewExpr(e);
        arg.push_back(e);
    }
    m_SExp = false;
    return new CallFuncExpr(target,arg,{0,m_col});
}

Input * Normalization::visit(Input * input) 
{
    m_SExp = false;
    return new Input({0,m_col});
}

Alloc * Normalization::visit(Alloc * alloc) 
{
    Expr * e = alloc->m_expr->Accept(*this);
    CheckAndCreateNewExpr(e);
    m_SExp = false;
    return new Alloc(e,{0,m_col});
}

VarRef * Normalization::visit(VarRef * varRef) 
{
    Identifier * id = varRef->m_id->Accept(*this);
    m_SExp = true;
    return new VarRef(id,{0,m_col});
}

Deref * Normalization::visit(Deref * deref) 
{
    Expr * e = deref->m_pointer->Accept(*this);
    CheckAndCreateNewExpr(e);
    m_SExp = false;
    return new Deref(e,{0,m_col});
}

RecordField * Normalization::visit(RecordField * recordField) 
{
    Expr * e = recordField->m_expr->Accept(*this);
//    CheckAndCreateNewExpr(e);
//    Not sure if recordFields should be only id
    return new RecordField(recordField->m_name,e,{0,m_col});
}

Record * Normalization::visit(Record * record) 
{
    vector<RecordField*> rec;
    for (auto & x : record->m_fields)
    {
        rec.push_back(x->Accept(*this));
    }
    m_SExp = false;
    return new Record(rec,{0,m_col});
}

FieldAccess * Normalization::visit(FieldAccess * fieldAccess) 
{
    Expr * e = fieldAccess->m_record->Accept(*this);
    CheckAndCreateNewExpr(e);
    m_SExp = false;
    return new FieldAccess(e,fieldAccess->m_field,{0,m_col});
}

AssignStmt * Normalization::visit(AssignStmt * assignStmt) 
{
    Expr * left = assignStmt->m_left->Accept(*this);
    if (!dynamic_cast<Identifier*>(left))
        CheckAndCreateNewExpr(left);
    Expr * right = assignStmt->m_right->Accept(*this);
    return new AssignStmt(left,right,{0,m_col});
}

NestedBlockStmt * Normalization::visit(NestedBlockStmt * nestedBlockStmt) 
{
    vector<StmtInNestedBlock*> stmts;
    m_col += m_tab;
    for (auto & x : nestedBlockStmt->m_body)
    {
        StmtInNestedBlock * stmt = x->Accept(*this);
        AddNewExprs(stmts);
        stmts.push_back(stmt);
        
    }
    m_col -= m_tab;
    return new NestedBlockStmt(stmts,{0,m_col});
}

IdentifierDecl * Normalization::visit(IdentifierDecl * idDecl) 
{
    return new IdentifierDecl(idDecl->m_name,{0,m_col});
}

VarStmt * Normalization::visit(VarStmt * varStmt) 
{
    vector<IdentifierDecl*> decl;
    for (auto & x : varStmt->m_decls)
        decl.push_back(x->Accept(*this));
    return new VarStmt(decl,{0,m_col});
}

ReturnStmt * Normalization::visit(ReturnStmt * returnStmt) 
{
    Expr * e = returnStmt->m_expr->Accept(*this);
    CheckAndCreateNewExpr(e);
    return new ReturnStmt(e,{0,m_col});
}

FunBlockStmt * Normalization::visit(FunBlockStmt * funBlockStmt) 
{
    vector<StmtInNestedBlock*> stmts;
    m_col += m_tab;
    for (auto & x : funBlockStmt->m_stmts)
    {
        StmtInNestedBlock * stmt = x->Accept(*this);
        AddNewExprs(stmts);
        stmts.push_back(stmt);
    }
    ReturnStmt * ret = funBlockStmt->m_ret->Accept(*this);
    AddNewExprs(stmts);

    vector<IdentifierDecl*> newIdent;
    for (auto & x : m_newVar)
        newIdent.push_back(new IdentifierDecl(x,{0,m_col}));

    vector<VarStmt*> vars;
    vars.push_back(new VarStmt(newIdent,{0,m_col}));
    for (auto & x : funBlockStmt->m_vars)
        vars.push_back(x->Accept(*this));
    m_col -= m_tab;
    return new FunBlockStmt(vars,stmts,ret,{0,m_col});
}

IfStmt * Normalization::visit(IfStmt * ifStmt) 
{
    Expr * cond = ifStmt->m_guard->Accept(*this);
    NestedBlockStmt * thenStmt = ifStmt->m_thenBranch->Accept(*this);
    NestedBlockStmt * elseStmt = nullptr;
    if (ifStmt->m_elseBranch != nullptr)
        elseStmt = ifStmt->m_elseBranch->Accept(*this);
    CheckAndCreateNewExpr(cond);
    return new IfStmt(cond,thenStmt,elseStmt,{0,m_col});
}

WhileStmt * Normalization::visit(WhileStmt * whileStmt) 
{
    Expr * cond = whileStmt->m_guard->Accept(*this);
    NestedBlockStmt * block = whileStmt->m_block->Accept(*this);
    CheckAndCreateNewExpr(cond);
    return new WhileStmt(cond,block,{0,m_col});
}

OutputStmt * Normalization::visit(OutputStmt * output) 
{
    Expr * e = output->m_expr->Accept(*this);
    CheckAndCreateNewExpr(e);
    return new OutputStmt(e,{0,m_col});
}

FunDecl * Normalization::visit(FunDecl * funDecl) 
{
    vector<IdentifierDecl*> decl;
    for (auto & x : funDecl->m_params)
        decl.push_back(x->Accept(*this));

    FunBlockStmt * block = funDecl->m_block->Accept(*this);
    return new FunDecl(decl,block,funDecl->m_name,{0,m_col});
}

Program * Normalization::visit(Program * program) 
{
    vector<FunDecl*> funs;
    for (auto & x : program->m_funs)
    {
        funs.push_back(x->Accept(*this));
        m_newVar.clear();
        m_currentName = 1;
        m_idName = 1;
    }
    return new Program(funs,{0,m_col});
}
