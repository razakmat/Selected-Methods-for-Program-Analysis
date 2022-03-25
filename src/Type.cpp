#include "Type.h"
#include <iostream>
#include "AST.h"
#include <typeinfo>

char TFresh::m_now1 = 'A';

TFresh::TFresh(){
}

string TRecord::Print() 
{
    string str = "{";
    int size = m_fields.size();
    for(auto const & i : m_fields)
    {
        str += i.first + ":";
        str += i.second->Print();
        if (--size > 0)
            str += ",";
    }
    str += "}";
    return str;
}

string TAbsentField::Print() 
{
    return "◇";
}

string TRecursive::Print() 
{
    return "μ" + m_fresh->Print() +
            "." + m_inner->Print();
}

TPointer::TPointer(shared_ptr<Type> t)
: m_ref(t)
{
}

string TPointer::Print() 
{
    return "↑" + m_ref->Print();
}

TTerm::TTerm(ASTNode * e)
: m_expr(e)
{
}

string TTerm::Print() 
{
    string str = typeid(*m_expr).name();
    str = str.substr(1);
    str += "[";
    str += to_string(m_expr->m_loc.m_line);
    str += ",";
    str += to_string(m_expr->m_loc.m_col);
    str += "]";
    return str;
}

string TFunction::Print() 
{
    string str = "(";
    for (uint32_t i = 0; i < m_params.size(); i++)
    {
        str += m_params[i]->Print();
        if (i != m_params.size() - 1)
            str += ",";
    }
    str += ") -> " + m_ret->Print();
    return str;
}

TVar::TVar(const string & s)
: m_name(s)
{
}

string TVar::Print() 
{
    return m_name;
}

string TFresh::Print() 
{
    if (m_name.empty())
    {
        m_name.push_back(m_now1++);
    }
    return m_name;
}



string TInt::Print() 
{
    return "int";
}
