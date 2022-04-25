#include "SignLattice.h"

    const int SignLattice::plusTable[5][5] = {{0,0,0,0,0},{0,1,2,3,4},{0,2,2,4,4},{0,3,4,3,4},{0,4,4,4,4}};
    const int SignLattice::minusTable[5][5] = {{0,0,0,0,0},{0,1,3,2,4},{0,2,4,2,4},{0,3,3,4,4},{0,4,4,4,4}};
    const int SignLattice::timesTable[5][5] = {{0,0,0,0,0},{0,1,1,1,1},{0,1,3,2,4},{0,1,2,3,4},{0,1,4,4,4}};
    const int SignLattice::divTable[5][5] = {{0,0,0,0,0},{0,0,1,1,4},{0,0,4,4,4},{0,0,4,4,4},{0,0,4,4,4}};
    const int SignLattice::gtTable[5][5] = {{0,0,0,0,0},{0,1,3,1,4},{0,1,4,1,4},{0,3,3,4,4},{0,4,4,4,4}};
    const int SignLattice::eqqTable[5][5] = {{0,0,0,0,0},{0,3,1,1,4},{0,1,4,1,4},{0,1,1,4,4},{0,4,4,4,4}};

SignLattice::SignLattice()
{
    
}

SignLattice::SignLattice(int i)
{
    m_val = i;
}

SignLattice SignLattice::plus(SignLattice & i, SignLattice & j) 
{
    return plusTable[i.m_val][j.m_val];
}

SignLattice SignLattice::minus(SignLattice & i, SignLattice & j) 
{ 
    return minusTable[i.m_val][j.m_val];
}

SignLattice SignLattice::times(SignLattice & i, SignLattice & j)
{
    return timesTable[i.m_val][j.m_val];
}

SignLattice SignLattice::div(SignLattice & i, SignLattice & j)
{
    return divTable[i.m_val][j.m_val];
}

SignLattice SignLattice::gt(SignLattice & i, SignLattice & j)
{
    return gtTable[i.m_val][j.m_val];
}

SignLattice SignLattice::eqq(SignLattice & i, SignLattice & j)
{
    return eqqTable[i.m_val][j.m_val];
}

SignLattice SignLattice::num(int n)
{
    if (n > 0) 
        return 3; 
    if (n < 0) 
        return 2; 
    return 1;
}

void SignLattice::Top()
{
    m_val = 4;
}

void SignLattice::Bot()
{
    m_val = 0;
}

bool SignLattice::operator==(const SignLattice & a) const
{
    return m_val == a.m_val;
}

string SignLattice::GetName()
{
    if (m_val == 0)
        return "⊥";
    else if (m_val == 4)
        return "⊤";
    else if (m_val == 1)
        return "0";
    else if (m_val == 2)
        return "-";
    else
        return "+";
}

void SignLattice::Lub(SignLattice & a, SignLattice & b)
{
    if (a.m_val == 4 || b.m_val == 4) m_val = 4;
    else if (a.m_val == 0) m_val = b.m_val;
    else if (b.m_val == 0) m_val = a.m_val;
    else if (a == b) m_val = a.m_val;
    else m_val = 4;
}


