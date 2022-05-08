#include "Andersen.hpp"
VarCell::VarCell(const string & str)
{
    m_name = str;
}

AllocCell::AllocCell(int num)
{
    m_name = "alloc-";
    m_name += to_string(num);
}

void CubicSolver::AddToken(Cell * t, Cell * x)
{
    if (x->m_sol.find(t) == x->m_sol.end())
    {
        x->m_sol.insert(t);
        m_worklist.push_back({t,x});
    }
}

void CubicSolver::AddEdge(Cell * x, Cell * y)
{
    if (x != y && x->m_succ.find(y) == x->m_succ.end())
    {
        x->m_succ.insert(y);
        for (auto & it : x->m_sol)
            AddToken(it,y);
    }
}

void CubicSolver::Propagate()
{
    while (!m_worklist.empty())
    {
        Cell * x = m_worklist.back().second;
        Cell * t = m_worklist.back().first;
        m_worklist.pop_back();
        auto cond = x->m_cond.find(t);
        if (cond != x->m_cond.end())
            for (auto & it : cond->second)
                AddEdge(it.first,it.second);
        for (auto & it : x->m_succ)
            AddToken(t,it);
    }
}

void CubicSolver::ConsAddT(Cell * t, Cell * x)
{
    AddToken(t,x);
    Propagate();
}

void CubicSolver::ConsAddE(Cell * y, Cell * z)
{
    AddEdge(y,z);
    Propagate();
}

void CubicSolver::ConsImpl(Cell * t, Cell * x, Cell * y, Cell * z)
{
    if (x->m_sol.find(t) != x->m_sol.end())
        ConsAddE(y,z);
    else{
        auto cond = x->m_cond.find(t);
        if (cond == x->m_cond.end()){
            x->m_cond.insert({t,map<Cell*,Cell*>()});
            cond = x->m_cond.find(t);
        }
        cond->second.insert({y,z});
    }
}

Andersen::Andersen(Program * program)
{
    if (program->m_funs.size() != 1)
        throw "Error : Analysis is only intraprocedural.";
    FunDecl * fun  = program->m_funs[0];
    FunBlockStmt * block = fun->m_block;
    m_stmts = block->m_stmts;

    for (auto & x : fun->m_params)
    {
        VarCell * var = new VarCell(x->m_name);
        m_varCells.insert({x->m_name,var});
    }
    for (auto & x : block->m_vars)
    {
        for (auto & y : x->m_decls)
        {
            VarCell * var = new VarCell(y->m_name);
            m_varCells.insert({y->m_name,var});
        }
    }
}

Andersen::~Andersen()
{
    for (auto & x : m_varCells)
        delete x.second;
    for (auto & x :m_allocCells)
        delete x;
}

void Andersen::GetResult(map<string,vector<string>> & res)
{
    for (auto & x : m_varCells)
    {
        auto it = res.insert({x.first,vector<string>()});
        for (auto & y : x.second->m_sol)
            it.first->second.push_back(y->m_name);
    }
}

void Andersen::Start()
{
    FindCells(m_stmts);
    m_num_alloc = 0;
    CreateConstraints(m_stmts);
}

void Andersen::GetExpr(Expr * expr)
{
    if (VarRef * n = dynamic_cast<VarRef*>(expr))
    {
        VarCell * c = m_varCells.find(n->m_id->m_name)->second;
        m_refVars.insert(c);
    }
    else if (dynamic_cast<Alloc*>(expr))
    {
        AllocCell * c = new AllocCell(m_num_alloc++);
        m_allocCells.push_back(c);
    }
}

void Andersen::FindCells(vector<StmtInNestedBlock*> & block)
{
    for (auto & x : block)
    {
        if (IfStmt * node = dynamic_cast<IfStmt*>(x))
        {
            FindCells(node->m_thenBranch->m_body);
            if (node->m_elseBranch != nullptr)
                FindCells(node->m_elseBranch->m_body);
        }
        else if (WhileStmt * node = dynamic_cast<WhileStmt*>(x))
        {
            FindCells(node->m_block->m_body);
        }
        else if (AssignStmt * node = dynamic_cast<AssignStmt*>(x))
        {
            GetExpr(node->m_left);
            GetExpr(node->m_right);
        }
    }
}

void Andersen::CreateConstraints(vector<StmtInNestedBlock*> & block)
{
    for (auto & x : block)
    {
        if (IfStmt * node = dynamic_cast<IfStmt*>(x))
        {
            CreateConstraints(node->m_thenBranch->m_body);
            if (node->m_elseBranch != nullptr)
                CreateConstraints(node->m_elseBranch->m_body);
        }
        else if (WhileStmt * node = dynamic_cast<WhileStmt*>(x))
        {
            CreateConstraints(node->m_block->m_body);
        }
        else if (AssignStmt * node = dynamic_cast<AssignStmt*>(x))
        {
            if (Identifier * n1 = dynamic_cast<Identifier*>(node->m_left))
            {
                VarCell * c1 = m_varCells.find(n1->m_name)->second;
                if (Identifier * n2 = dynamic_cast<Identifier*>(node->m_right))
                {
                    VarCell * c2 = m_varCells.find(n2->m_name)->second;
                    solver.ConsAddE(c2,c1);
                }
                else if (dynamic_cast<Alloc*>(node->m_right))
                {
                    AllocCell * c2 = m_allocCells[m_num_alloc++];
                    solver.ConsAddT(c2,c1);
                }
                else if (VarRef * n2 = dynamic_cast<VarRef*>(node->m_right))
                {
                    VarCell * c2 = m_varCells.find(n2->m_id->m_name)->second;
                    solver.ConsAddT(c2,c1);
                }
                else if (Deref * n2 = dynamic_cast<Deref*>(node->m_right))
                {
                    Identifier * id2 = dynamic_cast<Identifier*>(n2->m_pointer);
                    VarCell * c2 = m_varCells.find(id2->m_name)->second;
                    for (auto & x: m_refVars)
                        solver.ConsImpl(x,c2,x,c1);
                    for (auto & x: m_allocCells)
                        solver.ConsImpl(x,c2,x,c1);
                }
            }
            else if (Deref * n1 = dynamic_cast<Deref*>(node->m_left))
            {
                Identifier * id1 = dynamic_cast<Identifier*>(n1->m_pointer);
                Identifier * id2 = dynamic_cast<Identifier*>(node->m_right);
                VarCell * c1 = m_varCells.find(id1->m_name)->second;
                VarCell * c2 = m_varCells.find(id2->m_name)->second;
                for (auto & x: m_refVars)
                    solver.ConsImpl(x,c1,c2,x);
                for (auto & x: m_allocCells)
                    solver.ConsImpl(x,c1,c2,x);
            }
        }
    }
}
