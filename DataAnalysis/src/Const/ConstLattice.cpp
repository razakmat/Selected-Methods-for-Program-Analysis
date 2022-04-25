#include "ConstLattice.h"

using namespace std;
ConstLattice::ConstLattice()
{
    
}

ConstLattice::ConstLattice(int i)
{
    m_val = i;
}

void ConstLattice::Lub(ConstLattice & a, ConstLattice & b)
{
    if (a.m_top || b.m_top){
        m_bot = false;
        m_top = true;
    }
    else if (a.m_bot){
        m_top = b.m_top;
        m_bot = b.m_bot;
        m_val = b.m_val;
    }
    else if (b.m_bot){
        m_top = a.m_top;
        m_bot = a.m_bot;
        m_val = a.m_val;
    }
    else if (a == b){}
    else
    {
        m_bot = false;
        m_top = true;
    }
}

bool ConstLattice::operator==(const ConstLattice& a) const
{
    if (m_bot == !a.m_bot)
        return false;
    if (m_top == !a.m_top)
        return false;
    if (m_top && a.m_top)
        return true;
    if (m_bot && a.m_bot)
        return true;
    return m_val == a.m_val;
}

string ConstLattice::GetName()
{
    if (m_bot)
        return "⊥";
    if (m_top)
        return "⊤";
    return to_string(m_val);
}

ConstLattice ConstLattice::plus(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
        c.m_val = i.m_val + j.m_val;
    return c;
}

ConstLattice ConstLattice::minus(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
        c.m_val = i.m_val - j.m_val;
    return c;
}

ConstLattice ConstLattice::times(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
    {
        if (j.m_val != 0)
            c.m_val = i.m_val * j.m_val;
        else
            c.m_bot = true;
    }
    return c;
}

ConstLattice ConstLattice::div(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
        c.m_val = i.m_val / j.m_val;
    return c;
}

ConstLattice ConstLattice::gt(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
        c.m_val = i.m_val > j.m_val;
    return c;
}

ConstLattice ConstLattice::eqq(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c = Apply(i,j);
    if (!c.m_bot && !c.m_top)
        c.m_val = i.m_val == j.m_val;
    return c;
}

ConstLattice ConstLattice::num(int n)
{
    return n;
}

void ConstLattice::Top()
{
    m_bot = false;
    m_top = true;
}

void ConstLattice::Bot()
{
    m_top = false;
    m_bot = true;
}

ConstLattice ConstLattice::Apply(ConstLattice & i, ConstLattice & j)
{
    ConstLattice c;
    if (i.m_bot || j.m_bot){
        c.Bot();
    }
    else if (i.m_top || j.m_top){
        c.Top();
    }
    return c;
}
