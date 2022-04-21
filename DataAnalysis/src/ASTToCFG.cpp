#include "ASTToCFG.h"
#include <vector>
using namespace std;

CFGNode * ASTToCFG::CreateCFG(Program * program)
{
    if (program->m_funs.size() != 1)
        throw "Error : Program can have only one function.";
    FunDecl * fun = program->m_funs[0];

    CFGStart * start = new CFGStart(fun->m_name,fun->m_params,m_counter++);
    m_nodes.push_back(start);
    m_prev.push_back(start);

    CreateVars(fun->m_block);
    CreateStmts(fun->m_block->m_stmts);
    CreateReturnAndEnd(fun->m_block->m_ret,fun->m_name);

    return start;
}

vector<CFGNode*> & ASTToCFG::GetVectorNodes()
{
    return m_nodes;
}


void ASTToCFG::CreateVars(FunBlockStmt * funBlock)
{
    for (auto & x : funBlock->m_vars)
    {
        CFGVar * vars = new CFGVar(x->m_decls,m_counter++);
        m_nodes.push_back(vars);
        WritePrev(vars);
        m_prev.push_back(vars);
    }
}

void ASTToCFG::CreateStmts(vector<StmtInNestedBlock*> & block)
{
    for (auto & x : block)
    {
        CreateStmt(x);
    }
}

void ASTToCFG::CreateStmt(StmtInNestedBlock * stmt)
{
    if (AssignStmt * assign = dynamic_cast<AssignStmt*>(stmt))
    {
        CFGAssign * node = new CFGAssign(dynamic_cast<Identifier*>(assign->m_left),
                                        assign->m_right,m_counter++);
        m_nodes.push_back(node);
        if (node->m_left == nullptr)
            throw "Error : Left side of assignment has to be identifier.";
        node->m_right = assign->m_right;
        WritePrev(node);
        m_prev.push_back(node);
    }
    else if (OutputStmt * out = dynamic_cast<OutputStmt*>(stmt))
    {
        CFGOutput * node = new CFGOutput(out->m_expr,m_counter++);
        m_nodes.push_back(node);
        WritePrev(node);
        m_prev.push_back(node);
    }
    else if (IfStmt * ifStmt = dynamic_cast<IfStmt*>(stmt))
    {
        CFGExpr * node = new CFGExpr(ifStmt->m_guard,m_counter++);
        m_nodes.push_back(node);
        WritePrev(node);
        m_prev.push_back(node);
        CreateStmts(ifStmt->m_thenBranch->m_body);
        if (ifStmt->m_elseBranch != nullptr)
        {
            vector<CFGNode*> prevThen = m_prev;
            m_prev.clear();
            m_prev.push_back(node);
            CreateStmts(ifStmt->m_elseBranch->m_body);
            for (auto & x : prevThen)
                m_prev.push_back(x);
        }
        else
        {
            m_prev.push_back(node);
        }
    }
    else if (WhileStmt * whileStmt = dynamic_cast<WhileStmt*>(stmt))
    {
        CFGExpr * node = new CFGExpr(whileStmt->m_guard,m_counter++);
        m_nodes.push_back(node);
        WritePrev(node);
        m_prev.push_back(node);
        CreateStmts(whileStmt->m_block->m_body);
        WritePrev(node);
        m_prev.push_back(node);
    }
}

void ASTToCFG::CreateReturnAndEnd(ReturnStmt * ret, const string & name)
{
    CFGReturn * CFGret = new CFGReturn(ret->m_expr,m_counter++);
    m_nodes.push_back(CFGret);
    WritePrev(CFGret);
    CFGEnd * end = new CFGEnd(name,m_counter++);
    m_nodes.push_back(end);
    end->m_prev.push_back(CFGret);
    CFGret->m_succ.push_back(end);
}

void ASTToCFG::WritePrev(CFGNode * now)
{
    for (auto & x : m_prev)
    {
        now->m_prev.push_back(x);
        x->m_succ.push_back(now);
    }
    m_prev.clear();
}
