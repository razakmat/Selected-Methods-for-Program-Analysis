#include "Steensgaard.hpp"

int FreshTerm::m_num = 1;

VarTerm::VarTerm(const string & name)
{
    m_name = name;
}

PointerTerm::PointerTerm(Term * ref)
{
    m_name = "-->" + ref->m_name;
    m_ref = ref;
}

FreshTerm::FreshTerm()
{
    m_name = "_" + to_string(m_num++);
}

AllocTerm::AllocTerm(int num)
{
    m_name = "alloc-";
    m_name += to_string(num);
}


void Unification::Unify(Term * t1, Term * t2)
{
    MakeSet(t1);
    Term * ptr1 = Find(t1);
    MakeSet(t2);
    Term * ptr2 = Find(t2);

    if (ptr1 != ptr2)
    {
        if (dynamic_cast<PointerTerm*>(ptr1) &&
            dynamic_cast<PointerTerm*>(ptr2))
        {
            Union(ptr1,ptr2);
            Unify(dynamic_cast<PointerTerm*>(ptr1)->m_ref,
                  dynamic_cast<PointerTerm*>(ptr2)->m_ref);
        }
        else if (dynamic_cast<Term*>(ptr1) &&
            dynamic_cast<PointerTerm*>(ptr2))
        {
            Union(ptr1,ptr2);
        }
        else if (dynamic_cast<PointerTerm*>(ptr1) &&
            dynamic_cast<Term*>(ptr2))
        {
            Union(ptr2,ptr1);
        }
        else
        {
            Union(ptr1,ptr2);
        }
    }

}

void Unification::Close()
{
    for (auto & x : m_map)
    {
        x.second = Find(x.first);
    }
}

Term * Unification::GetSecond(Term * t)
{
    return m_map.find(t)->second;
}

void Unification::MakeSet(Term * t)
{
    m_map.insert({t,t});
}

Term * Unification::Find(Term * t)
{
    auto it = m_map.find(t);
    if (it->first != it->second)
        it->second = Find(it->second);
    return it->second;
}

void Unification::Union(Term * t1, Term * t2)
{
    Term * ptr1 = Find(t1);
    Term * ptr2 = Find(t2);
    if (ptr1 != ptr2)
        m_map.find(ptr1)->second = ptr2;
}


Steensgaard::Steensgaard(Program * program)
{
    if (program->m_funs.size() != 1)
        throw "Error : Analysis is only intraprocedural.";
    FunDecl * fun  = program->m_funs[0];
    FunBlockStmt * block = fun->m_block;
    m_stmts = block->m_stmts;
    for (auto & x : fun->m_params)
    {
        VarTerm * var = new VarTerm(x->m_name);
        m_vars.insert({x->m_name,var});
    }
    for (auto & x : block->m_vars)
    {
        for (auto & y : x->m_decls)
        {
            VarTerm * var = new VarTerm(y->m_name);
            m_vars.insert({y->m_name,var});
        }
    }
}

Steensgaard::~Steensgaard()
{
    
}

void Steensgaard::Start()
{
    CreateConstraints(m_stmts);
    solver.Close();
}

void Steensgaard::GetResult(map<string,vector<string>> & res)
{
    for (auto & x : m_vars)
    {
        auto it = res.insert({x.first,vector<string>()});
        Term * one = solver.GetSecond(x.second);
        for (auto & y : m_pointers)
        {
            Term * two = solver.GetSecond(y);
            if (one == two)
                it.first->second.push_back(y->m_ref->m_name);
        }
        
    }
}

void Steensgaard::CreateConstraints(vector<StmtInNestedBlock*> & block)
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
                VarTerm * c1 = m_vars.find(n1->m_name)->second;
                if (Identifier * n2 = dynamic_cast<Identifier*>(node->m_right))
                {
                    VarTerm * c2 = m_vars.find(n2->m_name)->second;
                    solver.Unify(c1,c2);
                }
                else if (dynamic_cast<Alloc*>(node->m_right))
                {
                    AllocTerm * c2 = new AllocTerm(m_num_alloc++);
                    PointerTerm * p2 = new PointerTerm(c2);
                    m_pointers.push_back(p2);
                    solver.Unify(c1,p2);
                }
                else if (VarRef * n2 = dynamic_cast<VarRef*>(node->m_right))
                {
                    VarTerm * c2 = m_vars.find(n2->m_id->m_name)->second;
                    PointerTerm * p2 = new PointerTerm(c2);
                    m_pointers.push_back(p2);
                    solver.Unify(c1,p2);
                }
                else if (Deref * n2 = dynamic_cast<Deref*>(node->m_right))
                {
                    Identifier * id2 = dynamic_cast<Identifier*>(n2->m_pointer);
                    VarTerm * c2 = m_vars.find(id2->m_name)->second;
                    FreshTerm * f = new FreshTerm();
                    PointerTerm * p2 = new PointerTerm(f);
                    solver.Unify(c2,p2);
                    solver.Unify(c1,f);
                }
            }
            else if (Deref * n1 = dynamic_cast<Deref*>(node->m_left))
            {
                Identifier * id1 = dynamic_cast<Identifier*>(n1->m_pointer);
                Identifier * id2 = dynamic_cast<Identifier*>(node->m_right);

                VarTerm * c1 = m_vars.find(id1->m_name)->second;
                VarTerm * c2 = m_vars.find(id2->m_name)->second;
                FreshTerm * f = new FreshTerm();
                PointerTerm * p1 = new PointerTerm(f);
                solver.Unify(c1,p1);
                solver.Unify(c2,f);
            }
        }
    }
}

